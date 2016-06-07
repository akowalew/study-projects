/*
 * insertMode.cpp
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */
#include "insertMode.h"

void enterInsertMode()
{
	usartSendStr(VT_CURSOR_RESTORE); // FIRST, PLACE CURSOR AT THE END OF STRING
	uint8_t i;
	for(i = textLen ; i > 0 ; i--)
		usartSendStr(VT_CURSOR_RIGHT);

	usartSendStr(VT_CURSOR_ON);
	uint8_t rxData;
	while(1)
	{
		rxData = usartGetChar_b();

		if((rxData >= 0x20) && (rxData <= 0x7E)) // All printable chars (with space)
		{
			if( (textX + textLen) == (DISPLAY_WIDTH + DISPLAY_X) ) // dalej nie mozemy
				guiSetError(NORMAL_ERROR);
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
				usartSendStr(VT_CLR_CHR); // update string on screen (backspace, space, backspace)
			}
			else
				guiSetError(NORMAL_ERROR);
		}
		else if(rxData == VT_KEY_ENTER)
		{
			textStr[textLen] = '\0';
			usartSendStr(VT_CURSOR_OFF);
			return;
		}
		else
			guiSetError(NORMAL_ERROR);
	}
}
