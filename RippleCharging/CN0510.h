/*
 * CN0510.h
 *
 *  Created on: 2021¦~10¤ë12¤é
 *      Author: johnson
 */

#ifndef CN0510_H_
#define CN0510_H_
//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define MAX_DSFREQ_NUM  4       // Maximum dual-slope frequency test point, normal 4 points
#define MAX_DATA_NUM    30      // Maximum EIS searching test point

extern double DSFreq [MAX_DSFREQ_NUM];  // DS search frequency set
extern char *TI_ACK;                    // sent TI_ACK to 3029
extern char *ADI_ACK;                   // receive ADI_ACK from 3029
extern char *AD5940_Init_Done;          // receive initial done signal from 3029
extern char *AD5940_Measure;            // sent start measure signal to 3029
extern char *AD5940_Measure_Done;       // receive measure done signal from 3029

// Each battery impedance data contain optimal frequency flag, frequency and impedance value
typedef struct{
    bool    isOptFreq;
    double  frequency;
    double  impedance;
} batData_t;

typedef batData_t* pbatData;

extern batData_t batDataSet[MAX_DATA_NUM];       // Declare an data set store impedance data handler

// Initial battery impedance data set
void init_batDataSet(void);

// Get battery impedance at specific frequency
void getBatImpedance(float freq);

// Dual-slope tangent searching helper, calculate slope and return optimize frequency
float DS_slope_calculate(batData_t dataSet[MAX_DATA_NUM]);

#endif /* CN0510_H_ */
