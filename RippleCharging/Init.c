/*
 * init.c
 *
 *  Created on: 2021¦~6¤ë11¤é
 *      Author: Johnson
 */

#include "F28x_Project.h"

#include <stdio.h>

// GPIO initialization

void InitGPIO(void)
{
    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    InitGpio();
}

// PIE initialization

void InitPIE(void)
{
    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    InitPieVectTable();

}

// Initialize SPIA setting

void InitSPI(void)
{
    //
    // Initialize SPIA port, the function InitSpi and InitSpiGpio only initialize
    // SPIA, if want to use SPIB, need to modify the code into SPIB register
    //
    InitSpi();
    InitSpiGpio();
}

// initADC - Function to configure and power up ADCA.

void InitADC(void)
{
    // Setup VREF as internal
    // In order to set ADCC REF=3.3V, we need to setup both ADCA,ADCB,ADCC to 3v3.
    SetVREF(ADC_ADCA, ADC_INTERNAL, ADC_VREF3P3);
    SetVREF(ADC_ADCB, ADC_INTERNAL, ADC_VREF3P3);
    SetVREF(ADC_ADCC, ADC_INTERNAL, ADC_VREF3P3);

    EALLOW;

    // Set ADCCLK divider to /4
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;

    // Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    // Power up the ADC
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    // Set ADCCLK divider to /4
    AdccRegs.ADCCTL2.bit.PRESCALE = 6;

    // Set pulse positions to late
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    // Power up the ADC and then delay for 1 ms
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    EDIS;

    DELAY_US(1000);
}

void InitADCSOC(void)
{
    // Select the channels to convert and the end of conversion flag
    EALLOW;

    // ADCASOC0
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;     // SOC0 will convert pin A1
                                           // 0:A0  1:A1  2:A2  3:A3
                                           // 4:A4   5:A5   6:A6   7:A7
                                           // 8:A8   9:A9   A:A10  B:A11
                                           // C:A12  D:A13  E:A14  F:A15
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 19;     // Sample window is 20 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 1;   // Trigger on CPU Timer0 TINT0n

    // ADCASOC1
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;     // SOC1 will convert pin A1
                                           // 0:A0  1:A1  2:A2  3:A3
                                           // 4:A4   5:A5   6:A6   7:A7
                                           // 8:A8   9:A9   A:A10  B:A11
                                           // C:A12  D:A13  E:A14  F:A15
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 19;     // Sample window is 20 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 1;   // Trigger on CPU Timer0 TINT0n
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; // End of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   // Enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // Make sure INT1 flag is cleared

    // ADCCSOC0
    AdccRegs.ADCSOC0CTL.bit.CHSEL = 1;     // SOC0 will convert pin C1
                                           // 0:A0  1:A1  2:A2  3:A3
                                           // 4:A4   5:A5   6:A6   7:A7
                                           // 8:A8   9:A9   A:A10  B:A11
                                           // C:A12  D:A13  E:A14  F:A15
    AdccRegs.ADCSOC0CTL.bit.ACQPS = 19;     // Sample window is 20 SYSCLK cycles
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 1;   // Trigger on CPU Timer0 TINT0n
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0; // End of SOC0 will set INT1 flag
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;   // Enable INT1 flag
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // Make sure INT1 flag is cleared

    EDIS;

}

void InitCPUTimer(void)
{

    //
    // Initialize the Device Peripheral. For this example, only initialize the
    // Cpu Timers.
    //
    InitCpuTimers();
    //
    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 100MHz CPU Freq, 1 second Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 100, 10000);

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in ConfigCpuTimer and InitCpuTimers, the below settings must also be
    // be updated.
    //
    CpuTimer0Regs.TCR.all = 0x4000;

    //
    // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
    // which is connected to CPU-Timer 1, and CPU int 14, which is connected
    // to CPU-Timer 2
    //
    IER |= M_INT1;

    //
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

}

