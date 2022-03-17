/*
 * FIRFilter.h
 *
 *  Created on: 2021/11/05
 *      Author: johnson
 */

#ifndef FIRFILTER_H_
#define FIRFILTER_H_
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t *buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t max;
    bool full;
} circular_buf_t;

typedef circular_buf_t *circular_handle_t;
/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 4000 Hz

* 0 Hz - 100 Hz
  gain = 1
  desired ripple = 0.1 dB
  actual ripple = 0.015951275916802697 dB

* 500 Hz - 2000 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -53.29720463459193 dB

*/

#define FILTER_ORDER_NUM 32

static double FIRfilter[FILTER_ORDER_NUM] = {
  0.0020905702911241226,
  0.002198329969488436,
  0.0018455839635462378,
  -0.0002461746759199181,
  -0.004339111841987859,
  -0.009832514115041179,
  -0.015051668705739622,
  -0.017394587783182175,
  -0.01391003367109365,
  -0.0021693847835223133,
  0.0187804196099628,
  0.04773759846477984,
  0.08120512774053865,
  0.11393797865072831,
  0.14008236298940452,
  0.15460638939919383,
  0.15460638939919383,
  0.14008236298940452,
  0.11393797865072831,
  0.08120512774053865,
  0.04773759846477984,
  0.0187804196099628,
  -0.0021693847835223133,
  -0.01391003367109365,
  -0.017394587783182175,
  -0.015051668705739622,
  -0.009832514115041179,
  -0.004339111841987859,
  -0.0002461746759199181,
  0.0018455839635462378,
  0.002198329969488436,
  0.0020905702911241226
};



// Initial circular buffer
// @param cbuf circular buffer pointer
// @param size buffer size
circular_handle_t circular_buf_init(uint16_t *buffer, uint16_t size);

// Put in ADC value into circular buffer
// @param cbuf circular buffer pointer
// @param sample ADC read value
extern void circular_buf_put(circular_handle_t cbuf, uint16_t sample);

// Calculate filter output
// @param cbuf
// @param filter
// @return output
extern double filter_process(circular_handle_t cbuf, double *filter);

#endif /* FIRFILTER_H_ */
