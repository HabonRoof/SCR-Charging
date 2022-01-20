/*
 * CN0150.c
 *
 *  Created on: 2021¦~10¤ë12¤é
 *      Author: johnson
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "F28x_Project.h"
#include "uart.h"
#include "CN0510.h"
#include "sci.h"

char *TI_ACK = "A5A5\n";                  // sent TI_ACK to 3029
char *ADI_ACK = "5A5A\n";                 // receive ADI_ACK from 3029
char *AD5940_Init_Done = "ID\n";          // receive initial done signal from 3029
char *AD5940_Measure = "MS\n";            // sent start measure signal to 3029
char *AD5940_Measure_Done = "MD\n";       // receive measure done signal from 3029

batData_t batDataSet[MAX_DATA_NUM];      // battery impedance data array
float DSFreq[MAX_DSFREQ_NUM]={1000.5, 8000.5, 10000.5};     // dual-slope searching first test frequencies


/*
 * @brief Initialize battery impedance data set
 * @param MAX_DATA_NUM: maximum number of data set
 * @return address of batDataSet[]
 */
void init_batDataSet(void){
    int i = 0;
    for(i = 0; i < MAX_DATA_NUM; i++){          // Initial MAX_DATA_NUM elements in batDataSet[]
        batDataSet[i].isOptFreq = false;   // Each element store the address of batData
        batDataSet[i].frequency = 0.0;
        batDataSet[i].impedance = 0.0;
    }
}

/*
 * @brief get battery impedance(Z) at specific frequency
 * @param freq: frequency to test impedance
 * @return imp: impedance of specify frequency
 */
void getBatImpedance(float freq){
    int i ;
    char str[5];
    transmitSCIBMessage("f\n");
    for (i = 0; i < 4; i++)
        str[i] = __byte((int*) &freq , i);  // Separate 32-bit float into four 16-bit char, each char LSB is 8-bit effective
    str[4] = '\n';
    for(i = 0; i < 5; i++)                  // Transmit frequency data to 3029 using char because there will be 0x00 at float type at first
        transmitSCIBChar(str[i]);
    transmitSCIBMessage("\n\n\0");          // Shift out uart tx FIFO, put two dummy data ensure next transmit is complete command
}

linearEqu linearFunCalc(float x1, float x2, float y1, float y2){
    linearEqu func;
    func.slope = (y2-y1)/(x2-x1);
    func.coeff = y2 - (func.slope * x2);
    return func;
}

/*
 * @brief calculate dual-slope search method intersection of two line
 * @param batDataSet: the data set of battery impedance data
 * @return f: intersection frequency, next measurement point
 */
float DSFoptCalc(batData_t* dataSet){
    linearEqu Func1, Func2 = {0.0,0.0};
    float fopt = 0.0;
    Func1 = linearFunCalc(dataSet->frequency,(dataSet+1)->frequency,dataSet->impedance,(dataSet+1)->impedance);
    Func2 = linearFunCalc((dataSet+2)->frequency,(dataSet+3)->frequency,(dataSet+2)->impedance,(dataSet+3)->impedance);
    fopt = (Func2.coeff - Func1.coeff)/(Func1.slope - Func2.slope);
    return fopt;
}

