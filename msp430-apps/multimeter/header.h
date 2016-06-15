/*
 * header.h
 *
 *  Created on: Jun 13, 2016
 *      Author: student
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <msp430.h>
#include <signal.h>
#include <stdint.h>

#define FLAG_ADC_COMPLETE 0x01
#define FLAG_DEBOUNCE 0x02
#define FLAG_DELAY_COMPLETE 0x04

#define TIMERA0_DELAY 31 // approx 1ms
#define DEBOUNCE_CYCLES 50

#define BTN_0 0x01
#define BTN_1 0x02

#define goSleep() _BIS_SR(LPM0_bits | GIE)
#define wakeUp()  _BIC_SR_IRQ(LPM0_bits | GIE)

void waitDelay(uint16_t delay);

#endif /* HEADER_H_ */
