/*
 * movingMode.cpp
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */
#include "movingMode.h"

#define ARROW_UP_KEY 'A'
#define ARROW_DOWN_KEY 'B'
#define ARROW_RIGHT_KEY 'C'
#define ARROW_LEFT_KEY 'D'
#define ARROW_PREFIX  '['
const ModeKey arrowKeys[] = {
		{ ARROW_UP_KEY, movingUp },
		{ ARROW_DOWN_KEY, movingDown },
		{ ARROW_RIGHT_KEY, movingRight },
		{ ARROW_LEFT_KEY, movingLeft },
};

inline uint8_t isKeyCorrect(uint8_t key)
{
	return ((key >= ARROW_UP_KEY) && (key <= ARROW_LEFT_KEY));
}

void makeTextBold()
{
	const char * const seq = ( VT_CURSOR_RESTORE VT_SET_GREEN );
	usartSendStr(seq);
	usartSendStr(textStr);
}

void makeTextUnbold()
{
	const char * const controlSequence = (
			VT_CURSOR_RESTORE
			VT_RESET_ATTRS
			VT_CURSOR_SAVE
			);
	usartSendStr(controlSequence);
	usartSendStr(textStr);
}

void movingUp()
{
	if(textY == (DISPLAY_Y))
	{
		usartSendError();
		return;
	}

	usartSendStr(VT_CURSOR_RESTORE);
	uint8_t i;
	for(i = textLen ; i > 0 ; i--) // kasuj tekst
		usartSendChr(' ');

	const char * const moveSequence = (
			VT_CURSOR_RESTORE
			VT_SET_GREEN
			VT_CURSOR_UP
			VT_CURSOR_SAVE
			);
	usartSendStr(moveSequence);
	usartSendStr(textStr);
	--textY;
}

void movingDown()
{
	if(textY == (DISPLAY_Y + DISPLAY_HEIGHT - 1))
	{
		usartSendError();
		return;
	}

	usartSendStr(VT_CURSOR_RESTORE);
	uint8_t i;
	for(i = textLen ; i > 0 ; i--) // kasuj tekst
		usartSendChr(' ');

	const char * const moveSequence = (
			VT_CURSOR_RESTORE
			VT_SET_GREEN
			"\n"
			VT_CURSOR_SAVE
			);
	usartSendStr(moveSequence);
	usartSendStr(textStr);

	++textY;
}

void movingLeft()
{
	if(textX == DISPLAY_X)
	{
		usartSendError();
		return;
	}

	const char * const moveSequence = (
			VT_CURSOR_RESTORE
			VT_SET_GREEN
			VT_CURSOR_LEFT
			VT_CURSOR_SAVE
			);
	usartSendStr(moveSequence);
	usartSendStr(textStr);
	usartSendChr(' ');

	--textX;
}
void movingRight()
{
	if((textX + textLen) == (DISPLAY_X + DISPLAY_WIDTH))
	{
		usartSendError();
		return;
	}
	const char * const moveSequence = (
			VT_CURSOR_RESTORE
			VT_SET_GREEN
			" "
			VT_CURSOR_SAVE
			);
	usartSendStr(moveSequence);
	usartSendStr(textStr);
	++textX;
}

void enterMovingMode()
{
	if(textLen == 0)
		usartSendError();
	else
	{
		makeTextBold();

		uint8_t rxData; uint8_t cmdI = 0;
		uint8_t command[3]; // command max size : 3
		while(1)
		{
			rxData = usartGetChar_b();
			if(rxData == VT_KEY_ENTER)
			{
				makeTextUnbold(); // save configuration2
				return;
			}
			else
			{
				command[cmdI++] = rxData;
				if(cmdI == 3) // command completed?
				{
					if((command[0] == VT_KEY_ESC) && (command[1] == ARROW_PREFIX)
							&& isKeyCorrect(rxData))
						arrowKeys[rxData-ARROW_UP_KEY].keyFunction(); // command is okay, we have an arrow
					else // wrong key
						usartSendError(); // and... discard all other chars?

					cmdI = 0;
				}
			}
		}
	}
}



