/*
 * header.h
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <msp430.h>
#include <stdint.h>
#include <signal.h>

#include "virtualTerminal.h"

typedef void (* const F_PTR)(void);
typedef struct
{
	const uint8_t key;
	F_PTR keyFunction;
} ModeKey ;

#define goSleepEint() (_BIS_SR(LPM0_bits | GIE))
#define wakeUp() (_BIC_SR_IRQ(LPM0_bits))

#define DISPLAY_WIDTH 21
#define DISPLAY_HEIGHT 5

extern uint8_t textX;
extern uint8_t textY;
extern char textStr[DISPLAY_WIDTH+1];
extern int8_t textLen;
extern const char textDefaultVtPos[10];

#endif /* HEADER_H_ */
