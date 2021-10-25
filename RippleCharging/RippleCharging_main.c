/**
 * This file shows how to control AD9833 DDS signal generator
 * using SPIA interface.
 * The AD9833 DDS chip SPI timing diagram determine the SPI mode, which is MODE2
 * Set the SPI mode in "f28004x_spi.c", InitSpi section.
 * Communicate to the EIS measurement board through ADICuP 3029 dev board.
 * The CN0510 hat can measure the Lithium battery impedance.
 * Also, the hardware connection is as below:
 *
 * F280049      LaunchPad pin        AD9833          ADICuP 3029
 * SPIA_STE     Pin19               FNC             x
 * SPIA_MOSI    Pin15               SPI_MISO        x
 * SPIA_CLK     Pin7                CLK             x
 * SCIA_RX      Pin10               x               TX
 * SCIA_TX      Pin9                x               RX
 *
 * Notice that the original LaunchPad card in box has a lot of editing error
 * please make sure the pin out is as like the F280049 datasheet shows, include the GPIO mux.
 *
 * Author: Chun-Lin Chen johnson35762@gmail.com
 * Date: 2021-09-24
 * License: GPL-3.0
 *
 * Please reference to Lab/實驗室研究資料區/陳俊霖/Lab105(NAS)/弦波充電 for more PCB detail
 * PCB editor convert to KiCad for fully opensource access.
 */
#include "F28x_Project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Init.h"       // Include initial setup
#include "AD9833.h"     // Include AD9833 lib
#include "CN0510.h"     // Include CN0510 lib
#include "spi.h"        // Include SPI lib
#include "sci.h"        // Include SCI lib

#define LED4    23      // Build in LED 4
#define LED5    34      // Build in LED 5

#define Relay1  5      // Relay for EIS measurement
#define Relay2  30      // Relay for charging / discharging
#define Relay3  58       // Relay for signal selecting symmetric/asymmetric
#define Relay4  25      // Relay for CV charging

#define len 8           // Moving average window size

typedef enum            // Ensure no variable name conflict
{
    CHARGE = 0x0000, DISCHARGE = 0x0001, MEASURE = 0x0011
} BAT_STATUS;

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

uint16_t sample1[len];      // Moving average sample array
uint16_t sample2[len];
uint16_t sample3[len];

unsigned int batRst = 0;        // Battery reset flag
unsigned int CCflag = 0;        // CC charging flag
unsigned int CVflag = 0;        // CV charging flag
unsigned int Disflag = 0;       // Discharge flag
unsigned int resetTimer = 0; // Battery reset timer after charging or discharging

float current = 0;
float batPos = 0;
float batNeg = 0;
unsigned int batVoltage = 0;

int batMode = 0;                // Battery charging status
// status 0: Symmetric sine current charging
// status 1: Asymmetric sine current charging
// status 2: Constant voltage charging
// status 3: Discharging
// status 4: EIS measuring

unsigned int uartReceived;      // flag for data received
unsigned char *SCIA_cmd;        // use for SCIA command string

unsigned char SCIA_msg[100];    // UART MUX transmit char array

uint16_t addrRecvTemp;          // received data array address

unsigned char recvTemp[64];     // temp data from RX
unsigned char recvData[64];     // Data from SCIA RX

// Function Prototypes ----------------------------------------------------------------------------

__interrupt void CPUTimer0ISR(void);
__interrupt void SCIARxISR(void);

void batChgMod(int mode);
float MAfilter(uint16_t adcBuff, uint16_t *samples, uint16_t maCtr,
               float lastAvg);

//void adi_InitEIS();
float adi_RcalMeasure(float ImRcal, float ReRcal);
float adi_EISMeasure(float frequency);

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
    InitADCSOC();
    InitCPUTimer();

    // Map ISR functions

    EALLOW;

    PieVectTable.TIMER0_INT = &CPUTimer0ISR;    // Enable cpuTimer0
    PieVectTable.SCIA_RX_INT = &SCIARxISR;

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;      // Enable cpuTimer0
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;      // Enable SCIA RX

    IER |= M_INT1;  // Enable group 1 interrupts, for Timer0
    IER |= M_INT9;  // Enable group 9 interrupts, for SCIA

    EDIS;

    // Enable CPU interrupt
    EINT;
    ERTM;

    // Initialize SCI interface
    initSCIA();  // Initialize the SCIA

    memset(recvTemp, 0, 64);                    // Reset recvData string
    SciaRegs.SCIHBAUD.all = 0x0000;             // Set SCIA baud rate = 115200
    SciaRegs.SCILBAUD.all = 0x001A;

    // Set waveform frequency to 560 Hz
    AD9833_SetFrequency(560);

    // Set waveform type
    AD9833_SetWaveform(SINE_WAVE);

    // Enable AD9833 output
    AD9833_OutputEn(true);

    // Reset relays
    GPIO_WritePin(Relay1, 0);
    GPIO_WritePin(Relay2, 0);
    GPIO_WritePin(Relay3, 0);
    GPIO_WritePin(Relay4, 0);

    GPIO_WritePin(LED4, 0);
    GPIO_WritePin(LED5, 0);

    // System startup
    // adi_initEIS();
    // adi_RcalMeasure();
    // adi_EISMeasure();

    batChgMod(2);                // Default mode is charging
    CCflag = 1;
    // Main loop
    while (1)
    {
        // Get all ADC value calculate moving average
        //current = MAfilter(curBuff, &sample1, maCtr, current);
        //batPos = MAfilter(batPosBuff, &sample2, maCtr, batPos);
        //batNeg = MAfilter(batNegBuff, &sample3, maCtr, batNeg);
        //maCtr++;

        /***********UNSOLVED ISSUE **********
         * Will not go into MV filter sub function
         * It always go into SCIB ISR
         */

        if (batRst == 0)        // If battery not need rest 30min
        {
            if (Disflag == 0)
            {    // If battery is CHARGING
                if (batVoltage > 1740 || CVflag == 1)
                {     // If battery reached 4.2V
                    CVflag = 1;     // Latch CC mode for ADC jitter
                    CCflag = 0;
                    batChgMod(2);    // Set relay to CV mode
                    if (curBuff < 2105)
                    {      // If charging current < 130mA(0.05C)
                        CVflag = 0;
                        CCflag = 1;
                        batRst = 1;
                        batChgMod(4);   // Reset battery
                    }
                }
                else if (CCflag == 1)
                {   // battery voltage 2.5V ~ 4.2V
                    CVflag = 0;     // Latch CC mode for ADC jitter
                    CCflag = 1;
                    batChgMod(1);   // Set relay to CC mode, symmetric sine wave
                }
            }
            else
            {                // If battery is DISCHARGING
                if (batVoltage < 1010)   // If battery voltage < 2.5V
                {
                    batRst = 1;
                    batChgMod(4);
                }
                else
                {
                    CCflag = 1;
                    CVflag = 0;
                    batChgMod(3);   // status 3: Discharging
                }
            }
        }
        else
        {        // If battery need rest for 30min after charging or discharging
            if (resetTimer == 1800)     // If rest 30 min (1800s)
            {
                resetTimer = 0;
                batRst = 0;
                Disflag = !Disflag;
            }
            else                        // Not reach 30min
            {
                batChgMod(4);           // Change mode to reset (EIS measure)
                DELAY_US(1000000);      // Delay 1s
                resetTimer++;
            }
        }

    }
// Testing Code

}

// SCIA connect to the UART MUX
__interrupt void SCIARxISR(void)
{
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    printf("!!!");
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;        // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}

__interrupt void CPUTimer0ISR(void)
{
    curBuff = AdcbResultRegs.ADCRESULT0;
    batPosBuff = AdcaResultRegs.ADCRESULT0;
    batNegBuff = AdccResultRegs.ADCRESULT0;
    batVoltage = batPosBuff - batNegBuff;

// Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

// Moving Average Filter for ADC
// The window size can be modify by 'len' variable
// notice the bigger size you want, the longer data type of 'tot' should be.

//float MAfilter(uint16_t adcBuff, uint16_t *samplePtr, uint16_t maCtr,
//               float lastAvg)
//{
//    uint16_t temp = 0;
//    float avg = 0.0;
//    uint16_t i = 0;
//    double tot = 0.0;
//
//// When in the first round of moving average, fill the array of ADC data.
//    if (maCtr < len)
//    {
//        *(samplePtr + maCtr) = adcBuff;
//        tot = lastAvg * len + *(samplePtr + maCtr);
//    }
//
//// When the array fill of data, shift the old data out and add new one
//    else
//    {
//        // Store the first element to calculate the average value
//        temp = *samplePtr;
//
//        // Shift out array
//        for (i = 0; i < len - 1; i++)
//            *(samplePtr + i) = *(samplePtr + i + 1);
//
//        // Update data
//        *(samplePtr + len - 1) = adcBuff;
//
//        // Calculate new total value of array
//        tot = lastAvg * len - temp + *(samplePtr + len - 1);
//    }
//// Calculate new average
//    avg = tot / len;
//    return avg;
//}

void batChgMod(int mode)
{
    switch (mode)
    {
    case 0:             // status 0: Symmetric sine current charging
        GPIO_WritePin(Relay1, 0);             // 1 for EIS
        GPIO_WritePin(Relay2, 1);             // 1 for charging
        GPIO_WritePin(Relay3, 0);             // 1 for asymmetric
        GPIO_WritePin(Relay4, 0);             // 1 for 4.2 CV;
        break;

    case 1:             // status 1: Asymmetric sine current charging
        GPIO_WritePin(Relay1, 0);             // 1 for EIS
        GPIO_WritePin(Relay2, 1);             // 1 for charging
        GPIO_WritePin(Relay3, 1);             // 1 for asymmetric
        GPIO_WritePin(Relay4, 0);             // 1 for 4.2 CV
        break;

    case 2:             // status 2: Constant voltage charging
        GPIO_WritePin(Relay1, 0);             // 1 for EIS
        GPIO_WritePin(Relay2, 1);             // 1 for charging
        GPIO_WritePin(Relay3, 0);             // 1 for asymmetric
        GPIO_WritePin(Relay4, 1);             // 1 for 4.2 CV
        break;

    case 3:             // status 3: Discharging
        GPIO_WritePin(Relay1, 0);             // 1 for EIS
        GPIO_WritePin(Relay2, 0);             // 1 for charging
        GPIO_WritePin(Relay3, 0);             // 1 for asymmetric
        GPIO_WritePin(Relay4, 0);             // 1 for 4.2 CV
        break;

    case 4:             // status 4: EIS measuring
        GPIO_WritePin(Relay1, 1);             // 1 for EIS
        GPIO_WritePin(Relay2, 0);             // 1 for charging
        GPIO_WritePin(Relay3, 0);             // 1 for asymmetric
        GPIO_WritePin(Relay4, 0);             // 1 for 4.2 CV
        break;

    default:             // default sine charging
        GPIO_WritePin(Relay1, 0);
        GPIO_WritePin(Relay2, 1);
        GPIO_WritePin(Relay3, 0);
        GPIO_WritePin(Relay4, 0);
        break;
    }
}
