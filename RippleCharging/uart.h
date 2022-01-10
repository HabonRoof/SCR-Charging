/*
 * uart.h
 *
 *  Created on: 2021¦~12¤ë28¤é
 *      Author: johnson
 */
//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C" {
#endif

#ifndef UART_H_
#define UART_H_

extern void transmitSCIBMessage(char*);
extern void transmitSCIBChar(char);


#endif /* UART_H_ */
