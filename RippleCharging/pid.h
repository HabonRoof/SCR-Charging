/*
 * pid.h
 *
 */

#ifndef PID_H_
#define PID_H_

#include "F28x_Project.h"



typedef struct PID_STRUCT{
    float  RefSetPoint;         // Input: Reference input
    float  Feedback_AdcPoint;         // Input: Feedback input

    float  Error;         // Variable: Error
    float  Error_1;
    float  Error_2;

    float  Kp;          // Parameter: Proportional gain
    float  Ki;          // Parameter: Integral gain
    float  Kd;          // Parameter: Derivative gain

    float  LastOutput;
    float  incOutput;   // Variable: Pre-saturated output
    float  OutputMax;      // Parameter: Maximum output
    float  OutputMin;      // Parameter: Minimum output
    float  Output;         // Output: PID output



//    void  (*calc)();      // Pointer to calculation function
}PID_STRUCT;


extern PID_STRUCT* Init_pid(void);
extern void pid_process(PID_STRUCT* psPID);




#endif /* PID_H_ */
