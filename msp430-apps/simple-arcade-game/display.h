/*
 * display.h
 *
 *  Created on: 23 maj 2016
 *      Author: dicker
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "header.h"

#define DISP_DIGITS_DIR	P2DIR
#define DISP_DIGITS_OUT	P2OUT
#define DISP_SEGS_DIR	P3DIR
#define DISP_SEGS_OUT	P3OUT

#define DISPLAY_TCCR	80//67(~480Hz)//40(~800Hz)

void displayInit();
void displaySetDigit(uint8_t digitNumber, uint8_t digitSegments);
void turnOffDisplay();

#endif /* DISPLAY_H_ */
