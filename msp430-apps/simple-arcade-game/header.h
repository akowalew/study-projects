/*
 * header.h
 *
 *  Created on: May 17, 2016
 *      Author: student
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <msp430.h>
#include <stdint.h>
#include <signal.h>

#define timerATurnOff() (TACTL &= ~MC_2)
#define timerATurnOn() (TACTL |= MC_2)
#define timerBTurnOn() (TBCTL |= MC_1)
#define timerBTurnOff() (TBCTL &= ~MC_1)
#define SLEEP_BITS	LPM1_bits

#endif /* HEADER_H_ */

