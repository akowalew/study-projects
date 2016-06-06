/*
 * movingMode.cpp
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */

#include "movingMode.h"

const char ARROW_LEFT_KEY = 'A';
const char ARROW_DOWN_KEY = 'B';
const char ARROW_RIGHT_KEY = 'C';
const char ARROW_UP_KEY = 'D';

const ModeKey arrowKeys[] = {
		{ ARROW_DOWN_KEY, movingDown },
		{ ARROW_RIGHT_KEY, movingRight },
		{ ARROW_LEFT_KEY, movingLeft },
		{ ARROW_UP_KEY, movingUp },
};
const uint8_t ARROW_PREFIX = '[';

inline uint8_t isKeyCorrect(uint8_t key)
{
	return ((key >= ARROW_LEFT_KEY) && (key <= ARROW_RIGHT_KEY));
}

void makeTextBold()
{

}


void makeTextUnbold()
{

}


void saveMoveConfig()
{

}

void restoreMoveConfig()
{

}

void movingUp()
{

}
void movingDown()
{

}
void movingLeft()
{

}
void movingRight()
{

}

void enterMovingMode()
{
	if(textLen == 0)
		guiSetError("TEXT IS EMPTY");
	else
	{
		saveMoveConfig();// first, save current configuretion;
		makeTextBold();

		uint8_t rxData; uint8_t cmdI;
		uint8_t command[3]; // command max size : 3
		while(1)
		{
			usartRxDint();
			if(usartIsCharAvailable())
			{
				usartRxEint();
				rxData = usartGetChar();

				if(rxData == VT_KEY_ENTER)
				{
					// save configuration
					makeTextUnbold();
					return;
				}
				else if(rxData == VT_KEY_CTRL_R)
				{
					restoreMoveConfig(); // we have to restore to original configuration
					cmdI = 0; // and discard previous data
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
			else
			{
				usartRxEint();
				goSleepEint();
			}
		}
	}
}



