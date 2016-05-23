/*
 * header.h
 *
 *  Created on: May 17, 2016
 *      Author: student
 */

#ifndef HEADER_H_
#define HEADER_H_

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t ;

#include "buttons.h"

#define DISP_DIGITS_DIR	P4DIR
#define DISP_DIGITS_OUT	P4OUT
#define DISP_SEGS_DIR	P3DIR
#define DISP_SEGS_OUT	P3OUT

#define DISPLAY_TCCR	18431//18431//9215//18431

#define GAME_SHIFT_TCCR	16383

#define GAME_BULLET_LEFT	0xFB
#define GAME_BULLET_RIGHT	0xFA
#define GAME_BOOM			0xF8
#define GAME_NONE			0xFF


#define turnOffTimerAInt() (TACTL &= ~MC_2)
#define turnOnTimerAInt() (TACTL |= MC_2)
#define turnOffDebounceInt() (TACCTL1 &= ~CCIE)
#define turnOnDebounceInt() (TACCTL1 |= CCIE)

void initButtons();
void initTimers();
void initClock();
void initDisplay();

#endif /* HEADER_H_ */

