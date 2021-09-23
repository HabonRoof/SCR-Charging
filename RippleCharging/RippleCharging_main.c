/**
 * This file shows how to control AD9833 DDS signal generator
 * using SPIA interface.
 * The AD9833 DDS chip SPI timing diagram determine the SPI mode, which is MODE2
 * Set the SPI mode in "f28004x_spi.c", InitSpi section.
 * Also, the HW connect is as below:
 *
 * F280049      Lunchpad pin        AD9833
 * SPIA_STE     Pin19               FNC
 * SPIA_MOSI    Pin15               SPI_MISO
 * SPIA_CLK     Pin7                CLK
 *
 * Notice that the original Launchpad card in box has a lot of editing error
 * please make sure the pinout is as like the F280049 datasheet shows, include the GPIO mux.
 *
 * Author: Chun-Lin Chen johnson35762@gmail.com
 * Date: 2021-06-14
 * License: GPL-3.0
 *
 * Please reference to Lab/實驗室研究資料區/陳俊霖/Lab105(NAS)/弦波充電 for more PCB detail
 *
 */
#include "F28x_Project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Init.h"
#include "AD9833.h"
#include "spi.h"

#define LED4    23
#define LED5    34
#define Relay1  40
#define Relay2  13

#define len 8           // Moving average window size

typedef enum
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
uint16_t sample1[len];      // Moving average data array
uint16_t sample2[len];
uint16_t sample3[len];

unsigned int chargingFlag = 0;

float current = 0.0;
float batPos = 0.0;
float batNeg = 0.0;
//float realCurrent = 0.0;
//float realBatvoltage = 0.0;
float batVoltage = 0.0;
// Function Prototypes ----------------------------------------------------------------------------

__interrupt void CPUTimer0ISR(void);
float MAfilter(uint16_t adcBuff, uint16_t *samples, uint16_t maCtr,
               float lastAvg);

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

    // Setup board LED as indicator
    GPIO_SetupPinMux(LED4, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(LED4, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(LED5, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(LED5, GPIO_OUTPUT, GPIO_PUSHPULL);
    GPIO_WritePin(LED4, 0);
    GPIO_WritePin(LED5, 0);

    // Setup Relay1 pin
    // Relay1 = 0, Connect to Relay2
    // Relay1 = 1, EIS measurement　(CN0510 board)
    GPIO_SetupPinMux(Relay1, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(Relay1, GPIO_OUTPUT, GPIO_PUSHPULL);

    // Setup Relay2 pin
    // Relay2 = 0, Battery charging
    // Relay2 = 1, Battery discharging
    GPIO_SetupPinMux(Relay2, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(Relay2, GPIO_OUTPUT, GPIO_PUSHPULL);

    InitSPI();
    InitPIE();
    InitADC();
    InitADCSOC();
    InitCPUTimer();
    //
    // Map ISR functions
    //
    EALLOW;
    PieVectTable.TIMER0_INT = &CPUTimer0ISR;
    EDIS;

    // Enable CPU interrupt
    EINT;
    ERTM;

    // Set waveform frequency to 560 Hz
    AD9833_SetFrequency(560);
    // Set waveform type
    AD9833_SetWaveform(SINE_WAVE);

    // Enable AD9833 output
    AD9833_OutputEn(true);
    // Charge battery
    GPIO_WritePin(Relay1, 0);
    GPIO_WritePin(Relay2, 0);

    // Main loop
    while (1)
    {

        // Convert ADC value into real value
//        realCurrent = (current / 1241.2121) * 0.121;
//        realBatvoltage = ((batPos - batNeg) / 1241.2121) * 3.15;
        current = MAfilter(curBuff, &sample1, maCtr, current);
        batPos = MAfilter(batPosBuff, &sample2, maCtr, batPos);
        batNeg = MAfilter(batNegBuff, &sample3, maCtr, batNeg);
        maCtr++;
        batVoltage = batPos - batNeg;

        //
        if (batVoltage >= 1630)
            chargingFlag = 0;
        else if (batVoltage <= 1480)
            chargingFlag = 1;
        //Measure EIS
//        GPIO_WritePin(Relay1, 1);

//        Discharge
        GPIO_WritePin(Relay1, 0);
        GPIO_WritePin(Relay2, 1);

//
//        if ((batVoltage >= 1630) || !chargingFlag)
//        {
//            // Discharge battery
//            GPIO_WritePin(Relay1, 0);
//            GPIO_WritePin(Relay2, 1);
//        }
//        else if ((batVoltage <= 1480) || chargingFlag)
//        {
//            // Charge battery
//            GPIO_WritePin(Relay1, 0);
//            GPIO_WritePin(Relay2, 0);
//        }
        DELAY_US(500000);
    }
}

__interrupt void CPUTimer0ISR(void)
{
    curBuff = AdcaResultRegs.ADCRESULT0;
    batPosBuff = AdcaResultRegs.ADCRESULT1;
    batNegBuff = AdccResultRegs.ADCRESULT0;

    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//
// Moving Average Filter for ADC
// The window size can be modify by 'len' variable
// notice the bigger size you want, the longer data type of 'tot' should be
//

float MAfilter(uint16_t adcBuff, uint16_t *samplePtr, uint16_t maCtr,
               float lastAvg)
{
    uint16_t temp = 0;
    float avg = 0.0;
    uint16_t i = 0;
    double tot = 0.0;

    // When in the first round of moving average, fill the array of ADC data.
    if (maCtr < len)
    {
        *(samplePtr + maCtr) = adcBuff;
        tot = lastAvg * len + *(samplePtr + maCtr);
    }

    // When the array fill of data, shift the old data out and add new one
    else
    {
        // Store the first element to calculate the average value
        temp = *samplePtr;

        // Shift out array
        for (i = 0; i < len - 1; i++)
            *(samplePtr + i) = *(samplePtr + i + 1);

        // Update data
        *(samplePtr + len - 1) = adcBuff;

        // Calculate new total value of array
        tot = lastAvg * len - temp + *(samplePtr + len - 1);
    }
    // Calculate new average
    avg = tot / len;
    return avg;
}
