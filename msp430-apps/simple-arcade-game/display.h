/*
 * display.h
 *
 *  Created on: 23 maj 2016
 *      Author: dicker
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "header.h"

#define DISP_DIGITS_DIR	P4DIR
#define DISP_DIGITS_OUT	P4OUT
#define DISP_SEGS_DIR	P3DIR
#define DISP_SEGS_OUT	P3OUT

#define DISPLAY_TCCR	18431//18431//9215//18431

void displayInit();
void displaySetDigit(uint8_t digitNumber, uint8_t digitSegments);

uint8_t displayIsRunning();
void displayTurnOff();
void displayTurnOn();

#endif /* DISPLAY_H_ */
