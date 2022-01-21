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

#define MAX_DSFREQ_NUM  3       // Maximum dual-slope frequency test point, normal 4 points (1+3)
#define MAX_DATA_NUM    30      // Maximum EIS searching test point

extern float DSFreq [MAX_DSFREQ_NUM];  // DS search frequency set
extern char *TI_ACK;                    // sent TI_ACK to 3029
extern char *ADI_ACK;                   // receive ADI_ACK from 3029
extern char *AD5940_Init_Done;          // receive initial done signal from 3029
extern char *AD5940_Measure;            // sent start measure signal to 3029
extern char *AD5940_Measure_Done;       // receive measure done signal from 3029

// Each battery impedance data contain optimal frequency flag, frequency and impedance value
typedef struct{
    bool    isOptFreq;
    float   frequency;
    float   impedance;
} batData_t;

typedef struct{
    float   slope;
    float   coeff;
} linearEqu;
typedef batData_t* pbatData;

extern batData_t batDataSet[MAX_DATA_NUM];       // Declare an data set store impedance data handler

// Initial battery impedance data set
void init_batDataSet(void);

// Get battery impedance at specific frequency
void getBatImpedance(float freq);

// Calculate linear function solution
linearEqu linearFunCalc(float x1, float x2, float y1, float y2);

// Dual-slope tangent searching helper, calculate slope and return optimize frequency
float DSFoptCalc(batData_t* dataSet);


#endif /* CN0510_H_ */
