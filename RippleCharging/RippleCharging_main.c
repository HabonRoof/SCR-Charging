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
 * The CN0510 hat can measure the Lithium battery impedance.
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
 * Date: 2022-01-04
 * License: GPL-3.0
 *
 * Please reference to Lab/實驗室研究資料區/陳俊霖/Lab105(NAS)/弦波充電 for more PCB detail
 * PCB editor convert to KiCad for fully open source access.
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

#define Relay1  5       // Relay for CC charging
#define Relay2  58      // Relay for CV charging
#define Relay3  25      // Relay for EIS measuring


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

bool batRst = false;        // Battery reset flag
bool ccFlag = false;        // CC charging flag
bool cvFlag = false;        // CV charging flag
unsigned int rstTmr = 0;    // Battery reset timer after charging or discharging

double current = 0.0f;
double batPos = 0.0f;
double batNeg = 0.0f;
double batVolt = 0.0f;

circular_handle_t curr_cbuf;
circular_handle_t batPos_cbuf;
circular_handle_t batNeg_cbuf;

enum BATTERY_MODE{          // Battery charging status
    BAT_SRC_CHARGE,
    BAT_CV_CHARGE,
    BAT_RESET
};

volatile static uint16_t rxCmdCtr;          // received data array address
char recvData[64];      // Data from SCIA RX
char recvBuff[64];      // receive buffer

batData_t batDataSet[MAX_DATA_NUM];      // battery impedance data array
double DSFreq[MAX_DSFREQ_NUM]={100.0, 310.0, 3000.0, 4000.0};     // dual-slope searching first test frequencies

bool frecvFreq, frecvImp, fmeasDone, NL, isNextFreq, isNextImp;
// Function Prototypes ----------------------------------------------------------------------------

__interrupt void CPUTimer0ISR(void);
__interrupt void SCIBRxISR(void);

void batChgMod(int mode);

// Main --------------------------------------------------------------------------------------------
void main(void)
{
    // Decide where the code will store into, RAM or Flash memory
#if __FLASH_RAM

    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t) &RamfuncsLoadSize);

#endif

    // Initialize the system, the parameter or more module need to start refer to "Init.c"
    // to add more initialization process
    InitSysCtrl();
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
    batRst = 0;

    // Map ISR functions

    EALLOW;

    PieVectTable.TIMER0_INT = &CPUTimer0ISR;// Declare cpuTimer0 ISR
    PieVectTable.SCIB_RX_INT = &SCIBRxISR;  // Declare SCIB ISR
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;      // Enable cpuTimer0
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;      // Enable SCIB RX
    PieCtrlRegs.PIEIER9.bit.INTx4 = 1;      // Enable SCIB TX
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Clear INT GROUP1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9; // Clear INT GROUP9 flag

    IER |= M_INT1;  // Enable group 1 interrupts, for Timer0
    IER |= M_INT9;  // Enable group 9 interrupts, for SCIA

    EDIS;

    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    EINT;
    ERTM;

    // Initialize SCI interface
    InitSCIB();  // Initialize the SCIB

    // Set waveform frequency to 430Hz(Re) 1198Hz(Z) temp=25C
    AD9833_SetFrequency(1198);
    // Set waveform type
    AD9833_SetWaveform(SINE_WAVE);

    // Enable AD9833 output
    AD9833_OutputEn(true);
    // Reset relays
    GPIO_WritePin(Relay1, 0);
    GPIO_WritePin(Relay2, 0);
    GPIO_WritePin(Relay3, 0);

    GPIO_WritePin(LED4, 0);
    GPIO_WritePin(LED5, 0);

    // Set DAC output value
    DAC_setShadowValue(DACA_BASE, 1700);

    batChgMod(BAT_RESET);               // Default mode is reset
    ccFlag = 1;

    init_batDataSet();       // Initial battery impedance data set

    // Storage the impedance for DS search as first four element
    int i = 0;
    for(i = 0; i < MAX_DSFREQ_NUM; i++)
        batDataSet[i].frequency = DSFreq[i];

//    while(strcmp(recvData,AD5940_Init_Done)!= 0)   // wait for 3029 init done
//        ;
    memset((void*)recvData, 0, sizeof(recvData));                    // Clear recvData array
    getBatImpedance(100.45);
    getBatImpedance(200.45);
    getBatImpedance(300.45);
    getBatImpedance(400.45);
    // Charging loop
    while (1)
    {
            getBatImpedance(7463.45);
            if (batRst == 0)        // If battery not rest
            {
                if (batVolt > 3540 || cvFlag == 1)
                {     // If battery reached 4.2V
                    cvFlag = 1;     // Latch CV mode for ADC jitter
                    ccFlag = 0;
                    batChgMod(BAT_CV_CHARGE);    // Set relay to CV mode
                    if (current < 2141) //@@@@ Rmin = 2141 Zmin = 2159
                    {      // If charging current < 130mA(0.05C)
                        cvFlag = 0;
                        ccFlag = 1;
                        batRst = 1;
                        batChgMod(BAT_RESET);   // Reset battery
                    }
                }
                else if (ccFlag == 1)
                {   // battery voltage 2.5V ~ 4.2V
                    cvFlag = 0;     // Latch CC mode for ADC jitter
                    ccFlag = 1;
                    batChgMod(BAT_SRC_CHARGE);   // CC mode
                }
            }
            else
            {
                batChgMod(3);      // Change mode to reset
                rstTmr++;
            }

//Testing Code
    }
}


// SCIB interrupt service routine
__interrupt void SCIBRxISR(void)
{
    unsigned char c;
    c = SCI_readCharBlockingFIFO(SCIB_BASE);     // read FIFO char
    recvBuff[rxCmdCtr] = c;     // put char into string array
    rxCmdCtr++;
    switch(c){
        case('F'):                          // receive frequency char
                frecvFreq = true;
                break;
        case('Z'):
                frecvImp = true;            // receive impedance char
                break;
        case('\n'):
                NL = true;                  // receive new line char
                strcpy(recvData,recvBuff);
                if(isNextFreq){             // process frequency data
                    batDataSet[5].frequency = atof(recvBuff);
                    isNextFreq = false;
                    NL = false;
                }
                else if(isNextImp){         // process impedance data
                    batDataSet[5].impedance = atof(recvBuff);
                    isNextImp = false;
                    NL = false;
                }
                memset((void*)recvBuff, 0, sizeof(recvBuff));   // Clear receive buffer
                rxCmdCtr = 0;               // Reset array address
                break;
        default:
                break;
    }
    if(frecvFreq && NL){                    // if receive "F\n" string, raise isNextFreq flag
        isNextFreq = true;
        frecvFreq = false;
        NL = false;
    }
    else if(frecvImp && NL){                // if receive "Z\n" string, raise isNextImp flag
        isNextImp = true;
        frecvImp = false;
        NL = false;
    }

    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;    // Clear Interrupt flag
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

// Timer0 for ADC trigger
__interrupt void CPUTimer0ISR(void)
{
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
    DAC_setShadowValue(DACA_BASE, batNeg);

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void batChgMod(int mode)
{
    switch (mode)
    {
    case BAT_SRC_CHARGE:             // status 0: SRC charging
        GPIO_WritePin(Relay1, 1);             // 1 for CC
        GPIO_WritePin(Relay2, 0);             // 1 for CV
        GPIO_WritePin(Relay3, 0);             // 1 for RST
        GPIO_WritePin(LED4, 0);               // 1 for RED LED
        GPIO_WritePin(LED5, 1);               // 1 for GREEN LED
        break;

    case BAT_CV_CHARGE:             // status 1: CV charging
        GPIO_WritePin(Relay1, 0);             // 1 for CC
        GPIO_WritePin(Relay2, 1);             // 1 for CV
        GPIO_WritePin(Relay3, 0);             // 1 for RST
        GPIO_WritePin(LED4, 1);               // 1 for RED LED
        GPIO_WritePin(LED5, 0);               // 1 for GREEN
        break;

    case BAT_RESET:             // status 2: RST
        GPIO_WritePin(Relay1, 0);             // 1 for CC
        GPIO_WritePin(Relay2, 0);             // 1 for CV
        GPIO_WritePin(Relay3, 0);             // 1 for RST
        GPIO_WritePin(LED4, 1);               // 1 for RED LED
        GPIO_WritePin(LED5, 1);               // 1 for GREEN
        break;

    default:             // default battery not connect
        GPIO_WritePin(Relay1, 0);
        GPIO_WritePin(Relay2, 0);
        GPIO_WritePin(Relay3, 0);
        GPIO_WritePin(LED4, 1);               // 1 for RED LED
        GPIO_WritePin(LED5, 1);               // 1 for GREEN
        break;
    }
}
