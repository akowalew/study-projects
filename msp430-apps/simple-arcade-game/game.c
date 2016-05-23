/*
 * game.c
 *
 *  Created on: 23 maj 2016
 *      Author: dicker
 */

#include "game.h"

uint8_t leftGamer = 0, rightGamer = 0;
uint8_t boomVar = 0;
int8_t leftLastBulletPos = 7, rightLastBulletPos = 0;
volatile uint8_t isTimerCycled = 0;

#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{
	switch(TAIV)
	{
	case 2:
		TACCR1 += DEBOUNCE_TCCR;

		isTimerCycled = 1;
		_BIC_SR_IRQ(SLEEP_BITS); // wake up!
		break;
	}
}

void gameGoNextCycle()
{
	TACCTL1 = 0;
	isTimerCycled = 0;
	TACCTL1 = CCIE;

	boomVar = 0;
	uint8_t boomFlags = 0;

	if(rightGamer)
	{
		rightGamer <<= 1;
		rightLastBulletPos++;

		if(rightLastBulletPos == 8)
		{
			boomFlags |= 0x01 ; // RIGHT BOOM
			boomVar |= 0x80 ;
		}
	}

	if(leftGamer)
	{
		leftGamer >>= 1;
		leftLastBulletPos--;

		if(leftLastBulletPos == -1)
		{
			boomFlags |= 0x02; // LEFT BOOM
			boomVar |= 0x01;
		}
	}

	if(leftGamer && rightGamer)
	{
		if(leftLastBulletPos <= rightLastBulletPos)
		{
			boomVar |= (1 << leftLastBulletPos) | (1 << rightLastBulletPos);
			leftGamer &= ~(1 << leftLastBulletPos);
			rightGamer &= ~(1 << rightLastBulletPos);
			boomFlags = 0x03;
		}
	}

	if(boomFlags & 0x01) // RIGHT BOOM
	{
		rightLastBulletPos--;
		while(rightLastBulletPos > 0)
		{
			if(rightGamer & (1 << rightLastBulletPos))
				break ;
			rightLastBulletPos--;
		}
	}

	if(boomFlags & 0x02) // LEFT BOOM
	{
		leftLastBulletPos++;
		while(leftLastBulletPos < 7)
		{
			if(leftGamer & (1 << leftLastBulletPos))
				break;
			leftLastBulletPos++;
		}
	}
}

void gameUpdate()
{
	uint8_t i;
    uint8_t mask = 0x01;

    while(mask)
    {
    	uint8_t outSegs;
	    if(boomVar & mask)
		    outSegs = GAME_BOOM;
        else if(leftGamer & mask)
            outSegs = GAME_BULLET_LEFT;
        else if(rightGamer & mask)
            outSegs = GAME_BULLET_RIGHT ;
        else
            outSegs = GAME_NONE;
	    displaySetDigit(i++, outSegs);
        mask <<= 1 ;
    }

    if(!(leftGamer || rightGamer || boomVar)) //nothing to display
    {
    	/* displayTurnOff(); // turn off display refreshing
        gamePause(); // no bullet to shift */
        timerATurnOff();
    }
    else
    {
        /* displayTurnOn(); // turn on display refreshing
        gameResume(); // turn on shifting */
        timerATurnOn();
    }
}

inline void gameResume() { TACCTL1 = CCIE ; }
inline void gamePause() { TACCTL1 = 0 ; }
inline void gameBulletLeftAdd() { leftGamer |= 0x80; }
inline void gameBulletRightAdd() { rightGamer |= 0x01; }
inline uint8_t gameIsNextCycle() { return isTimerCycled ; }
