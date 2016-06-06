/*
 * insertMode.cpp
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */
#include "insertMode.h"

inline void strcpy(char *dest, const char *src)
{
	while(*(dest++) = *(src++));
}

void enterInsertMode()
{
	usartSendStr(VT_CURSOR_ON);
	usartSendStr(textVtPos); // GENERALLY, PLACE CURSOR AT THE END OF STRING

	char copy[DISPLAY_WIDTH+1];
	uint8_t origLen = textLen;
	strcpy(copy, textStr);

	uint8_t rxData;
	while(1)
	{
		usartRxDint();
		if(usartIsCharAvailable())
		{
			usartRxEint();
			rxData = usartGetChar();

			if((rxData >= 0x20) && (rxData <= 0x7E)) // All printable chars (with space)
			{
				if( (textX + textLen) == (DISPLAY_WIDTH + DISPLAY_X) ) // dalej nie mozemy
					guiSetError("OUT OF BOUNDS");
				else
				{
					textStr[textLen++] = rxData;
					usartSendChr(rxData);
				}
			}
			else if(rxData == VT_KEY_BACKSPACE)
			{
				if(textLen > 0)
				{
					--textLen; // delete char from string
					usartSendStr("\b \b"); // update string on screen (backspace, space, backspace)
				}
				else
					guiSetError("EMPTY STRING");
			}
			else if(rxData == VT_KEY_ENTER)
			{
				// save configuration
				usartSendStr(VT_CURSOR_OFF);
				return;
			}
			else if(rxData == VT_KEY_CTRL_R)
			{ // restore configuration
				for(int8_t i = textLen ; i > 0 ; i--)
					usartSendStr("\b \b");

				char c; char *p = textStr;
				char *pc = copy;
				while((c = *(pc++)))
				{
					usartSendChr(c);
					*(p++) = c;
				}

				textLen = origLen;
			}
			else
				guiSetError(WRONG_KEY_ERR);
		}
		else
		{
			usartRxEint();
			goSleepEint();
		}
	}


}
