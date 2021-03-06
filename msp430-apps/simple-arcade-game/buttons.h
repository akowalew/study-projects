/*
 * buttons.h
 *
 *  Created on: May 23, 2016
 *      Author: student
 */
#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "header.h"

#define BTN_RIGHT 0x01
#define BTN_LEFT 0x80
#define BTN_N	2
#define BTN_ALL (BTN_RIGHT | BTN_LEFT)
#define BOUNCE_SAMPLES_N 25
#define DEBOUNCE_TCCR 31

#define BTN_IN	P1IN
#define BTN_DIR	P1DIR
#define BTN_IE	P1IE
#define BTN_IES	P1IES
#define BTN_IFG	P1IFG
#define BTN_VECTOR	PORT1_VECTOR

void buttonsInit();
uint8_t isButtonPressed(uint8_t buttonMask);

#endif /* BUTTONS_H_ */
