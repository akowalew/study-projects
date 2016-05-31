/*
 * game.c
 *
 *  Created on: 23 maj 2016
 *      Author: dicker
 */
#include "game.h"

uint8_t leftGamer, rightGamer;
uint8_t boomVar;
int8_t leftLastBulletPos, rightLastBulletPos;
volatile uint8_t isTimerCycled;
volatile uint8_t isBuzzerBuzzing;

#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{
	switch(TAIV)
	{
	case 2: // Buzzer timeout int
		TACCR1 += GAME_SHIFT_TCCR;

		isTimerCycled = 1;
		_BIC_SR_IRQ(SLEEP_BITS); // wake up!

		break;
	case 4: // Game next cycle (shift) int
		GAME_BUZZER_OUT &= ~GAME_BUZZER;
		TACCTL2 = 0; // only one int
		isBuzzerBuzzing = 0;

		break;
	}
}

void gameGoNextCycle()
{
	TACCTL1 = 0;
	isTimerCycled = 0;
	TACCTL1 = CCIE;

	boomVar = 0;
	if((!rightGamer) && (!leftGamer))
		return; // nothing to do

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
	uint8_t i = 0, mask = 0x01, outSegs;
    while(mask)
    {
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

    if(boomVar) // we have to turn once the buzzer!
    {
    	uint16_t timerVal = TAR;
    	while(timerVal != TAR) // get stable value of TAR
    		timerVal = TAR;

    	TACCTL2 = 0;
       	TACCR2 = timerVal + GAME_BUZZER_TCCR;
    	GAME_BUZZER_OUT |= GAME_BUZZER;
    	isBuzzerBuzzing = 1;
    	TACCTL2 = CCIE;
    }

    if(!(leftGamer || rightGamer || boomVar || isBuzzerBuzzing)) //nothing to display
    {
        timerATurnOff();
        displayTurnOff();
    }
    else if(!timerAIsRunning())
	{
		uint16_t timerVal = TAR; // timer is powered down - only one read
		TACCR0 = TAR + DISPLAY_TCCR; // correct compare registers
		TACCR1 = TAR + GAME_SHIFT_TCCR;
		timerATurnOn();
	}
}

void gameInit()
{
	GAME_BUZZER_OUT &= ~GAME_BUZZER;
	GAME_BUZZER_DIR |= GAME_BUZZER;

	leftGamer = rightGamer = boomVar = 0;
	leftLastBulletPos = 7; rightLastBulletPos = 0;
	isTimerCycled = 0;
}


uint8_t gameBulletLeftAdd()
{
	if(leftGamer & 0x80)
		return 0;
	else
	{
		leftGamer |= 0x80;
		return 1;
	}
}

uint8_t gameBulletRightAdd()
{
	if(rightGamer & 0x01)
		return 0;
	else
	{
		rightGamer |= 0x01;
		return 1;
	}
}

inline uint8_t gameIsNextCycle() { return isTimerCycled; }
inline void gameResume() { TACCTL1 = CCIE ; }
inline void gamePause() { TACCTL1 = 0 ; }
