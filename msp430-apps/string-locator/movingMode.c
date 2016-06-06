/*
 * movingMode.cpp
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */
#include "movingMode.h"

#define ARROW_LEFT_KEY 'A'
#define ARROW_DOWN_KEY 'B'
#define ARROW_RIGHT_KEY 'C'
#define ARROW_UP_KEY 'D'
#define ARROW_PREFIX  '['

const ModeKey arrowKeys[] = {
		{ ARROW_DOWN_KEY, movingDown },
		{ ARROW_RIGHT_KEY, movingRight },
		{ ARROW_LEFT_KEY, movingLeft },
		{ ARROW_UP_KEY, movingUp },
};

inline uint8_t isKeyCorrect(uint8_t key)
{
	return ((key >= ARROW_LEFT_KEY) && (key <= ARROW_RIGHT_KEY));
}

void makeTextBold()
{
	usartSendStr(VT_CURSOR_RESTORE); // go to text position
	usartSendStr(VT_SET_BOLD);
	usartSendStr(textStr);
}

void makeTextUnbold()
{
	usartSendStr(VT_CURSOR_RESTORE);// usartSendStr(textVtPos);
	usartSendStr(VT_RESET_ATTRS);
	usartSendStr(textStr);
}

void movingUp()
{
	if(textY == (DISPLAY_Y + 1))
	{
		guiSetError("WRONG");
		return;
	}

	usartSendStr(VT_CURSOR_RESTORE);
	for(uint8_t i = textLen ; i > 0 ; i--) // kasuj tekst
		usartSendChr(' ');

	usartSendStr(VT_CURSOR_RESTORE);
	usartSendStr(VT_CURSOR_UP);
	usartSendStr(VT_CURSOR_SAVE);
	usartSendStr(textStr);

	--textY;
}

void movingDown()
{
	if(textY == (DISPLAY_Y + DISPLAY_HEIGHT - 1))
	{
		guiSetError("WRONG");
		return;
	}

	usartSendStr(VT_CURSOR_RESTORE);
	for(uint8_t i = textLen ; i > 0 ; i--) // kasuj tekst
		usartSendChr(' ');

	usartSendStr(VT_CURSOR_RESTORE);
	usartSendStr(VT_CURSOR_DOWN);
	usartSendStr(VT_CURSOR_SAVE);
	usartSendStr(textStr);

	++textY;
}

void movingLeft()
{
	if(textX == DISPLAY_X)
	{
		guiSetError("WRONG");
		return;
	}

	usartSendStr(VT_CURSOR_RESTORE);
	usartSendStr(VT_CURSOR_LEFT);
	usartSendStr(VT_CURSOR_SAVE);
	usartSendStr(textStr);
	usartSendChr(' ');

	--textX;
}
void movingRight()
{
	if((textX + textLen) == (DISPLAY_X + DISPLAY_WIDTH))
	{
		guiSetError("WRONG");
		return;
	}

	usartSendStr(VT_CURSOR_RESTORE);
	usartSendChr(' ');
	usartSendStr(VT_CURSOR_SAVE);
	usartSendStr(textStr);
	++textX;
}

void enterMovingMode()
{
	if(textLen == 0)
		guiSetError("TEXT IS EMPTY");
	else
	{
		makeTextBold();

		uint8_t rxData; uint8_t cmdI;
		uint8_t command[3]; // command max size : 3
		while(1)
		{
			rxData = usartGetCharBlock();

			if(rxData == VT_KEY_ENTER)
			{
				// save configuration
				makeTextUnbold();
				return;
			}
			else
			{
				command[cmdI++] = rxData;
				if(cmdI == 3) // command completed?
				{
					if((command[0] == VT_KEY_ESC) && (command[1] == ARROW_PREFIX)
							&& isKeyCorrect(rxData))
						arrowKeys[cmdI].keyFunction(); // command is okay, we have an arrow
					else // wrong key
						guiSetError(WRONG_KEY_ERR); // and... discard all other chars?
					cmdI = 0;
				}
			}
		}
	}
}



