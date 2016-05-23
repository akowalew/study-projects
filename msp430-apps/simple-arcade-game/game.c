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
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B0(void)
{
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

    gameUpdate();
}

void gameUpdate()
{
	gamePause();

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

    _DINT();
    if(!(leftGamer || rightGamer || boomVar)) //nothing to display
    {
    	displayTurnOff(); // turn off display refreshing
        if(!isDebouncingNow())
        	TACTL &= ~MC_2;
        gamePause(); // no bullet to shift
    }
    else
    {
        displayTurnOn(); // turn on display refreshing
        TACTL |= MC_2; // turn on timerA
        gameResume(); // turn on shifting
    }
    _EINT();

    gameResume();
}

inline void gameResume() { TBCTL |= MC_1; }
inline void gamePause() { TBCTL &= ~MC_1; }
inline void gameBulletLeftAdd() { leftGamer |= 0x80; }
inline void gameBulletRightAdd() { rightGamer |= 0x01; }
