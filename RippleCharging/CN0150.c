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
    char str[6];
    transmitSCIBMessage("f\n");
    for (i = 0; i < 4; i++)
        str[i] = __byte((int*) &freq , i);  // Separate 32-bit float into four 16-bit char, each char LSB is 8-bit effective
    str[4] = '\n';
    transmitSCIBMessage(str);
    transmitSCIBMessage("\n\n\0");          // Shift out uart tx FIFO, put two dummy data ensure next transmit is complete command
}

/*
 * @brief calculate dual-slope search method intersection of two line
 * @param batDataSet: the data set of battery impedance data
 * @return f: intersection frequency, next measurement point
 */
float DS_slope_calculate(batData_t dataSet[MAX_DATA_NUM]){
    float a1,b1,a2,b2,f = 0.0;
    // Calculate left side line equation
    a1 = dataSet[0].frequency;
    b1 = dataSet[0].impedance;
    a2 = dataSet[1].frequency;
    b2 = dataSet[1].impedance;
    f = (a1-a2)/(b1-b2);
    return f;
}

