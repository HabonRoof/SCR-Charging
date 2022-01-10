/*
 * uart.c
 *
 *  Created on: 2021¦~12¤ë28¤é
 *      Author: johnson
 */

#include "F28x_Project.h"
#include <string.h>


// transmitSCIBChar - Transmit a character from the SCIA
void transmitSCIBChar(char a)
{
    while (ScibRegs.SCIFFTX.bit.TXFFST != 0)
    {

    }
    ScibRegs.SCITXBUF.all = a;
}

// transmitSCIBMessage - Transmit message via SCIA

void transmitSCIBMessage(char *msg)
{
    int i;
    i = 0;
    while (msg[i] != '\0')
    {
        transmitSCIBChar(msg[i]);
        i++;
    }
}



