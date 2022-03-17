/*
 * pid.c
 *
 */

#include    "pid.h"

static PID_STRUCT sPID;

PID_STRUCT* Init_pid(void)
{
    // Set PID Coefficients
    sPID.Kp = 0.1;

    sPID.Ki = 0.0005;

    sPID.Kd = 0;

    // Set PID Setpoint
    sPID.RefSetPoint = 1871.5;            // Default output current average 2.6A

    sPID.OutputMax = 1750;
    sPID.OutputMin = 1650;
    sPID.Error_2 = 0;
    sPID.Error_1 = 0;
    sPID.LastOutput = 0.0;
    return &sPID;
}

void pid_process(PID_STRUCT *psPID)
{
    // Compute the error
    psPID->Error = -(psPID->RefSetPoint - psPID->Feedback_AdcPoint);

    // Compute the proportional output
    //psPID->Up = psPID->Kp * psPID->Error;
    psPID->incOutput = psPID->Kp * (psPID->Error - psPID->Error_1);

    // Compute the integral output
    //psPID->Ui = psPID->Ui + (psPID->Ki * psPID->Up) + (psPID->Kc * psPID->SatErr);
    psPID->incOutput += psPID->Ki * (psPID->Error + psPID->Error_1);

    // Compute the derivative output
    //psPID->Ud = psPID->Kd * (psPID->Up - psPID->Up1);
    psPID->incOutput += psPID->Kd * (psPID->Error - 2 * psPID->Error_1 + psPID->Error_2);

    psPID->Output = psPID->LastOutput + psPID->incOutput;

    // Saturate the output
    if (psPID->Output > psPID->OutputMax)
        psPID->Output = psPID->OutputMax;

    else if (psPID->Output < psPID->OutputMin)
        psPID->Output = psPID->OutputMin;

    // Update the previous output
    psPID->LastOutput = psPID->Output;

    // Update last error
    psPID->Error_2 = psPID->Error_1;
    psPID->Error_1 = psPID->Error;

}

