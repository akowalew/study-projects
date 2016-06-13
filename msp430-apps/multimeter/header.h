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

#define goSleep() (_BIS_SR(LPM0_bits | GIE))
#define wakeUp() (_BIC_SR_IRQ(LPM0_bits))

#endif /* HEADER_H_ */
