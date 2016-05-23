/*
 * buttons.h
 *
 *  Created on: May 23, 2016
 *      Author: student
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "header.h"

#define BTN_RIGHT	0x01
#define BTN_LEFT	0x02
#define BTN_N	2
#define BTN_ALL (BTN_RIGHT | BTN_LEFT)
#define BOUNCE_SAMPLES_N    21
#define DEBOUNCE_TCCR	7199

#define BTN_IN	P1IN
#define BTN_DIR	P1DIR
#define BTN_IE	P1IE
#define BTN_IES	P1IES
#define BTN_IFG	P1IFG
#define BTN_VECTOR	PORT1_VECTOR

void initButtons();
uint8_t isDebouncingNow();
uint8_t isButtonEvent(uint8_t buttonMask);
void clearButtonEvent(uint8_t buttonMask);

void disableButtonsInt();
void restoreButtonsInt();

#endif /* BUTTONS_H_ */
