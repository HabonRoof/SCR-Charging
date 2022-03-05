/**
 * @file RippleCharging_main.c
 * @brief Main firmware of SRC board
 *
 * It charge, discharge, and measure EIS of battery.
 * Also shows how to control AD9833 DDS signal generator
 * using SPIA interface.
 * The AD9833 DDS chip SPI timing diagram determine the SPI mode, which is MODE2
 * Set the SPI mode in "f28004x_spi.c", InitSpi section.
 * Communicate to the EIS measurement board through ADICuP 3029 dev board.
 * The CN0510 hat can measure the Lithium battery impedance UART using UART protocol.
 * Also, the hardware connection is as below:
 * F280049      LaunchPad pin        AD9833          ADuCM 3029
 * SPIA_STE     Pin19               FNC             x
 * SPIA_MOSI    Pin15               SPI_MISO        x
 * SPIA_CLK     Pin7                CLK             x
 * SCIA_RX      Pin10               x               TX
 * SCIA_TX      Pin9                x               RX
 *
 * Notice that the original LaunchPad card in box has a lot of editing error
 * please make sure the pin out is as like the F280049 datasheet shows, include the GPIO mux.
 *
 * @author: Chun-Lin Chen johnson35762@gmail.com
 * @bugs:
 *
 * Date: 2022-01-25
 * License: GPL-3.0
 *
 * Please reference to Lab/實驗室研究資料區/陳俊霖/Lab105(NAS)/弦波充電 for more PCB detail
 * PCB editor changed to KiCad for fully open source access.
 */

#include "F28x_Project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Init.h"       // Include initial setup
#include "AD9833.h"     // Include AD9833 lib
#include "CN0510.h"     // Include CN0510 lib
#include "spi.h"        // Include SPI lib
#include "uart.h"       // Include UART driver
#include "sci.h"        // Include SCI lib
#include "dac.h"        // Include DAC lib
#include "FIRfilter.h"  // Include FIR lib

#define LED4    23      // Build in LED 4
#define LED5    34      // Build in LED 5

#define CCSwitch  5       // Relay for CC charging
#define CVSwitch  58      // Relay for CV charging
#define EISSwitch  30      // Relay for EIS measuring


// -----------------------------------------------------------------------------------------------
// __Flash_RAM = 1 , the code will storage into flash memory
// __Flash_RAM = 0 , the code will storage into RAM
// Set project property to corresponding cmd file.

#define __FLASH_RAM 1
#if __Flash_RAM

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

#endif

// Variables -------------------------------------------------------------------------------------
unsigned int curBuff = 0;
unsigned int batPosBuff = 0;
unsigned int batNegBuff = 0;
unsigned int maCtr = 0;

uint16_t currentBuffer[FILTER_ORDER_NUM];
uint16_t batPosBuffer[FILTER_ORDER_NUM];
uint16_t batNegBuffer[FILTER_ORDER_NUM];

float current   = 0.0f;
float batPos    = 0.0f;
float batNeg    = 0.0f;
float batVolt   = 0.0f;

circular_handle_t curr_cbuf;
circular_handle_t batPos_cbuf;
circular_handle_t batNeg_cbuf;

bool batRst = false;        // Battery reset flag
bool ccFlag = false;        // CC charging flag
bool cvFlag = false;        // CV charging flag
bool EISMeasDone = false;   // EIS measure done flag
unsigned int rstTmr = 0;    // Battery reset timer after charging

int freqStus = 0;

enum BATTERY_MODE{          // Battery charging status
    BAT_SRC_CHARGE,
    BAT_CV_CHARGE,
    BAT_EIS_MEASURE,
    BAT_RESET
};

enum SEARCH_MODE{           // Search algorithm
    INIT_SEARCH,
    DS_SLOPE_SEARCH,
    PnO_SEARCH,
    DONE_SEARCH
} searchMode;

volatile static uint16_t rxBufIdx;          // received data array address
char recvData[32];                          // Data from SCIA RX
char recvBuff[32];                          // receive buffer

extern batData_t batDataSet[MAX_DATA_NUM];  // Battery impedance data set
extern float DSFreq[MAX_DSFREQ_NUM];        // Dual slope searching frequency set
int     impIdx      = 0;                    // Impedance measurement counter
int     PnOCtr      = 0;                    // P&O convergence counter
float   Fopt        = 0.0;                  // Optimize frequency
float   stepsize    = 200.0;                // P&O step size
float   oldImp      = 0.0;                  // P&O old impedance
float   newImp      = 0.0;                  // P&O new impedance
float   oldFreq     = 0.0;                  // P&O old frequency
float   newFreq     = 0.0;                  // P&O new frequency
float   minImp      = 0.03;                 // P&O stop threshold
volatile bool NL, isNextFreq, isNextImp, isMeasDone; // flags

float totalCap = 0.0f;                      // Total battery capacity in mAh
float block0 = 0.0f;                        // Block0 for prevent overflow
float block1 = 0.0f;                        // Block1 for prevent overflow


// Function Prototypes ----------------------------------------------------------------------------
__interrupt void CPUTimer0ISR(void);
__interrupt void SCIBRxISR(void);

void PnOSearch(batData_t* dataSet);
void batChgMod(int mode);
void uartCmdProcess(void);
void batChargeCtrl(void);

// Main --------------------------------------------------------------------------------------------
void main(void)
{
    // Decide where the code will store into, RAM or Flash memory
#if __FLASH_RAM
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t) &RamfuncsLoadSize);
#endif

    InitSysCtrl();                          // Initialize the system, the parameter or more module need to start refer to "Init.c" to add more initialization process
    InitGPIO();
    InitSPI();
    InitPIE();
    InitADC();
    InitDAC();
    InitADCSOC();
    InitCPUTimer();
    curr_cbuf = circular_buf_init(currentBuffer,FILTER_ORDER_NUM);
    batPos_cbuf = circular_buf_init(batPosBuffer,FILTER_ORDER_NUM);
    batNeg_cbuf = circular_buf_init(batNegBuffer,FILTER_ORDER_NUM);

    // Map ISR functions
    EALLOW;
    PieVectTable.TIMER0_INT = &CPUTimer0ISR;// Declare cpuTimer0 ISR
    PieVectTable.SCIB_RX_INT = &SCIBRxISR;  // Declare SCIB ISR
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;      // Enable cpuTimer0
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;      // Enable SCIB RX
    PieCtrlRegs.PIEIER9.bit.INTx4 = 1;      // Enable SCIB TX
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Clear INT GROUP1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9; // Clear INT GROUP9 flag
    IER |= M_INT1;                          // Enable group 1 interrupts, for Timer0
    IER |= M_INT9;                          // Enable group 9 interrupts, for SCIA
    EDIS;
    EINT;                                   // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    ERTM;
    InitSCIB();                             // Initialize the SCIB
    AD9833_SetFrequency(1021.98);           // Set waveform frequency to 430Hz(Re) 1198Hz(Z) temp=25C
    AD9833_SetWaveform(SINE_WAVE);          // Set waveform type
    AD9833_OutputEn(true);                  // Enable AD9833 output
    GPIO_WritePin(CCSwitch, 0);             // Reset relays
    GPIO_WritePin(CVSwitch, 0);
    GPIO_WritePin(EISSwitch, 0);
    GPIO_WritePin(LED4, 0);
    GPIO_WritePin(LED5, 0);
    DAC_setShadowValue(DACA_BASE, 1700);    // Set DAC output value for MOSFET bias
    batRst          = false;                // Battery not need for reset
    batChgMod(BAT_RESET);                   // Default mode is reset disconnect every switch
    ccFlag          = true;                 // Set charging mode to CC mode
    EISMeasDone     = false;
    EISMeasDone     = true;
    init_batDataSet();                      // Initial battery impedance data set

    batChgMod(BAT_RESET);               // Change mode to EIS measure
//    DELAY_US(30000000);
//    while(strcmp(recvData,AD5940_Init_Done)!= 0);       // wait for 3029 initialize done
    memset((void*)recvData, 0, sizeof(recvData));       // Clear recvData array
    searchMode = INIT_SEARCH;


    while (1){
//        switch(searchMode){
//        case INIT_SEARCH :
//            getBatImpedance(DSFreq[impIdx]);    // Get impedance from DSFreq data set
//            if (impIdx > MAX_DSFREQ_NUM){
//                impIdx --;
//                Fopt = DSFoptCalc(batDataSet);  // Calculate optimize frequency using dual slope method
//                searchMode = DS_SLOPE_SEARCH;
//            }
//            break;
//        case DS_SLOPE_SEARCH:                   // Get impedance corresponding Fopt
//            getBatImpedance(Fopt);
//            oldFreq = Fopt;
//            newFreq = oldFreq + stepsize;
//            searchMode = PnO_SEARCH;
//            break;
//        case PnO_SEARCH:
//            PnOSearch(batDataSet);              // P&O search find best frequency
//            break;
//        case DONE_SEARCH:
//            if(!EISMeasDone){
//                AD9833_SetFrequency(Fopt);              // Set waveform frequency to 430Hz(Re) 1198Hz(Z) temp=25C
//                AD9833_SetWaveform(SINE_WAVE);          // Set waveform type
//                AD9833_OutputEn(true);                  // Enable AD9833 output
//                EISMeasDone = true;
//            }
//            break;
//        }
        if(current > 100){
            if(block0 > 2563200.0){
                 block1 ++;                     // 1000mAh reached
                 block0 = block0 - 2563200.0f;  // clear block0
            }
//            if(block1 == 0 && block0 > 2050560 && freqStus == 0){
//                AD9833_SetFrequency(1063.9);           // Set waveform frequency
//                AD9833_SetWaveform(SINE_WAVE);          // Set waveform type
//                AD9833_OutputEn(true);                  // Enable AD9833 output
//                freqStus = 1;
//            }
//            if(block1 == 1 && block0 > 1537920 && freqStus == 1){
//                AD9833_SetFrequency(1105.89);           // Set waveform frequency
//                AD9833_SetWaveform(SINE_WAVE);          // Set waveform type
//                AD9833_OutputEn(true);                  // Enable AD9833 output
//                freqStus = 2;
//            }
            else{
                 block0 = block0 + current;     // Accumulate current-time product
            }
        }
        batChargeCtrl();                        // Control relay for battery charging pattern
        DELAY_US(1000000);                      // Change battery charging mode every second
    }
}

/*
 * @brief perturb and observe search method implementation
 * @param batDataSet: the data set of battery impedance data
 * @return f: intersection frequency, next measurement point
 */
void PnOSearch(batData_t* dataSet){
    getBatImpedance(newFreq);
    DELAY_US(3000);
    getBatImpedance(newFreq);
    impIdx --;
    newImp = dataSet[impIdx].impedance;         // New frequency impedance data
    oldImp = dataSet[impIdx - 1].impedance;     // Old frequency impedance data

    if (fabs(oldImp - newImp) < minImp || PnOCtr > 50){
        PnOCtr = 0;
        searchMode = DONE_SEARCH;
    }
    if (newImp < oldImp){                  // Find minimal impedance frequency
        Fopt = newFreq;                         // Update Fopt
        newFreq = Fopt + stepsize;
        PnOCtr++;
    }
    else if (newImp > oldImp){
        newFreq = Fopt - stepsize;
        PnOCtr++;
    }
}

// SCIB interrupt service routine
__interrupt void SCIBRxISR(void){
    unsigned char c;
    c = SCI_readCharBlockingFIFO(SCIB_BASE);// read FIFO char
    recvBuff[rxBufIdx] = c;                 // put char into string array
    rxBufIdx++;
    if(c == '\n'){
        NL = true;                          // receive new line char
        memset((void*) recvData, 0, sizeof(recvData));   // Clear receive data
        strcpy(recvData, recvBuff);
        if (isNextFreq){                    // process frequency data
            isNextFreq = false;
            batDataSet[impIdx].frequency = atof(recvData);
        }
        else if (isNextImp){                // process impedance data
            isNextImp = false;
            batDataSet[impIdx].impedance = atof(recvData);
            if(impIdx > (MAX_DATA_NUM - 1))
                impIdx = 5;
            impIdx ++;                      // Increase data set index
            isMeasDone = true;
        }
        memset((void*) recvBuff, 0, sizeof(recvBuff));   // Clear receive buffer
        rxBufIdx = 0;                       // Reset array address
    }
    if(recvData[0] == 'F' && recvData[1] == '\n')       // If string is "F\n"
        isNextFreq = true;
    if(recvData[0] == 'Z' && recvData[1] == '\n')       // If string is "Z\n"
        isNextImp = true;
    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;    // Clear Interrupt flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// Timer0 for ADC trigger
__interrupt void CPUTimer0ISR(void){
    curBuff = AdcbResultRegs.ADCRESULT0;
    batPosBuff = AdcaResultRegs.ADCRESULT0;
    batNegBuff = AdccResultRegs.ADCRESULT0;

    circular_buf_put(curr_cbuf, curBuff);
    circular_buf_put(batPos_cbuf, batPosBuff);
    circular_buf_put(batNeg_cbuf, batNegBuff);

    current = filter_process(curr_cbuf, FIRfilter);
    batPos = filter_process(batPos_cbuf, FIRfilter);
    batNeg = filter_process(batNeg_cbuf, FIRfilter);

    batVolt = batPos - batNeg;

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



void batChgMod(int mode){
    switch (mode){
    case BAT_SRC_CHARGE:                // SRC charging
        GPIO_WritePin(CCSwitch,     1); // 1 for SRC
        GPIO_WritePin(CVSwitch,     0); // 1 for CV
        GPIO_WritePin(EISSwitch,    0); // 1 for EIS
        GPIO_WritePin(LED4, 0);         // 1 for RED LED
        GPIO_WritePin(LED5, 1);         // 1 for GREEN LED
        break;
    case BAT_CV_CHARGE:                 // CV charging
        GPIO_WritePin(CCSwitch,     0); // 1 for SRC
        GPIO_WritePin(CVSwitch,     1); // 1 for CV
        GPIO_WritePin(EISSwitch,    0); // 1 for EIS
        GPIO_WritePin(LED4, 1);         // 1 for RED LED
        GPIO_WritePin(LED5, 0);         // 1 for GREEN
        break;
    case BAT_EIS_MEASURE:               // EIS measuring
        GPIO_WritePin(CCSwitch,     0); // 1 for SRC
        GPIO_WritePin(CVSwitch,     0); // 1 for CV
        GPIO_WritePin(EISSwitch,    1); // 1 for EIS
        GPIO_WritePin(LED4, 0);         // 1 for RED LED
        GPIO_WritePin(LED5, 0);         // 1 for GREEN
        break;
    case BAT_RESET:                     // Reset
        GPIO_WritePin(CCSwitch,     0); // 1 for SRC
        GPIO_WritePin(CVSwitch,     0); // 1 for CV
        GPIO_WritePin(EISSwitch,    0); // 1 for EIS
        GPIO_WritePin(LED4, 1);         // 1 for RED LED
        GPIO_WritePin(LED5, 1);         // 1 for GREEN
        break;
    default:                            // Default battery not connect
        GPIO_WritePin(CCSwitch,     0);
        GPIO_WritePin(CVSwitch,     0);
        GPIO_WritePin(EISSwitch,    0);
        GPIO_WritePin(LED4, 1);         // 1 for RED LED
        GPIO_WritePin(LED5, 1);         // 1 for GREEN
        break;
    }
}

void batChargeCtrl(void){
    if(EISMeasDone){
        if (batRst){                            // If battery need reset
            batChgMod(BAT_RESET);               // Change mode to reset
            rstTmr++;
            if(totalCap == 0)
                totalCap = block1 + (block0/712)/3600;  // Total capacity in mAh
        }
        else{
            if (batVolt > 3530 || cvFlag == 1){ // If battery reached 4.2V
                cvFlag = true;                  // Latch CV mode for ADC jitter
                ccFlag = false;
                batChgMod(BAT_CV_CHARGE);      // Set relay to CV mode
                if (current < 97){
                    cvFlag = false;             // If charging current < 50mA
                    ccFlag = true;
                    batRst = true;
                    batChgMod(BAT_RESET);       // Reset battery
                }
            }
            else if (ccFlag){                   // If battery voltage 2.5V ~ 4.2V
                cvFlag = false;                 // Latch CC mode for ADC jitter
                ccFlag = true;
                batChgMod(BAT_SRC_CHARGE);      // CC mode
            }
        }
    }
    else
        batChgMod(BAT_EIS_MEASURE);      // Set relay to Measure EIS
}
