/*
 * display.c
 *
 *  Created on: 23 maj 2016
 *      Author: dicker
 */

#include "display.h"

volatile uint8_t digitMask = 0x01;
volatile uint8_t digitIndex = 0;
volatile uint8_t displayArray[8] = { 0 };
volatile uint8_t *outSegsPtr = &displayArray[0];

/**
 * Przerwanie TACCR0, Odswiezanie wyswietlacza
 */
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0(void)
{
	TACCR0 += DISPLAY_TCCR;

	DISP_SEGS_OUT = 0xFF;
	DISP_DIGITS_OUT = ~digitMask;
	DISP_SEGS_OUT = *(outSegsPtr++);

	digitMask <<= 1;
	if(digitMask == 0)
    {
		digitMask = 0x01;
        outSegsPtr = &displayArray[0];
    }
}

void displayInit()
{
	DISP_DIGITS_DIR = 0xFF;
	DISP_SEGS_DIR = 0xFF;
}

inline void displaySetDigit(uint8_t digitNumber, uint8_t digitSegments)
{
	displayArray[digitNumber] = digitSegments;
}
