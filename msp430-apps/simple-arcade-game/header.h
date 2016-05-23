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

#define BTN_RIGHT	0x01
#define BTN_LEFT	0x02
#define BTN_N	2
#define BTN_ALL (BTN_RIGHT | BTN_LEFT)
#define BOUNCE_SAMPLES_N    323

#define BTN_IN	P1IN
#define BTN_DIR	P1DIR
#define BTN_IE	P1IE
#define BTN_IES	P1IES
#define BTN_IFG	P1IFG
#define BTN_VECTOR	PORT1_VECTOR

#define DISP_DIGITS_DIR	P2DIR
#define DISP_DIGITS_OUT	P2OUT
#define DISP_SEGS_DIR	P3DIR
#define DISP_SEGS_OUT	P3OUT

#define DISPLAY_TCCR	9215//18431
#define DEBOUNCE_TCCR	7199
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
