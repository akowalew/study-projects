/*
 * vtGui.cpp
 *
 *  Created on: 5 cze 2016
 *      Author: dicker
 */
#include "vtGui.h"

const char * const WRONG_KEY_ERR = "WRONG KEY";
static uint8_t errorFlag = 0;

inline void guiDrawDisplayBox()
{
	int8_t x;
	const char *const boxVtPos = "\x1b[12;1H";

	usartSendStr(boxVtPos); // set cursor to (1,12)
	usartSendStr(VT_CURSOR_SAVE);

	// prepare vertical line
	char line[DISPLAY_WIDTH+3]; char *p = line+1;
	line[0] = line[DISPLAY_WIDTH+1] = '+';
	line[DISPLAY_WIDTH+2] = '\0';
	for(x = DISPLAY_WIDTH ; x > 0 ; --x)
		*(p++) = '-';
	usartSendStr(line);

	usartSendStr(VT_CURSOR_RESTORE);
	usartSendStr(VT_CURSOR_DOWN);

	// left line
	for(x = DISPLAY_HEIGHT ; x > 0; --x)
	{
		usartSendChr('|');
		usartSendStr(VT_CURSOR_DOWN);
		usartSendStr(VT_CURSOR_LEFT);
	}

	usartSendStr(line); // downline

	for(x = DISPLAY_HEIGHT ; x > 0; --x)
	{
		usartSendStr(VT_CURSOR_LEFT);
		usartSendStr(VT_CURSOR_UP);
		usartSendChr('|');
	}

}

void guiDisplayAll()
{
	usartSendStr(VT_RESET);
	usartSendStr(VT_CLR_SCR);
	usartSendStr(VT_HOME);
	usartSendStr(VT_CURSOR_OFF);

	const char * const beginString =
			"Hi, here you can configure\r\n"
			"your display.\r\n"
			"\n"
			"Available modes:\r\n"
			"(1) - INSERT - just type your text\r\n"
			"(2) - MOVING - use arrows to move string\r\n"
			"\n"
			"In every mode: <ENTER> to confirm, <CTRL> + <R> to undo\r\n"
			"\n"
			"Your display:\r\n";

	usartSendStr(beginString);
	guiDrawDisplayBox();

	// place current text in the box
	usartSendStr(textDefaultVtPos); // vtSetCursor(currTextX, currTextY);
	usartSendStr(VT_CURSOR_SAVE);
	usartSendStr(textStr);
}

void guiSetStatusStr(const char * const statusStr)
{
	usartSendStr(GUI_INFO_VT_POS);
	usartSendStr(statusStr);
}

void guiSetError(const char * const errorMsg)
{
	usartSendStr(GUI_ERROR_POS_S); // set position
	usartSendStr(VT_CLR_LN); // clear line
	usartSendStr(VT_SET_RED_FG); // red color
	usartSendStr(errorMsg);
	usartSendStr(VT_RESET_ATTRS);
	errorFlag = 1;
	P1OUT = 0XFF;
	// MAYBE BUZZER?
	usartSendStr(VT_CURSOR_RESTORE);
}

void guiFatalError(const char * const errorMsg)
{
	usartSendStr(GUI_ERROR_POS_S); // set position
	usartSendStr(VT_CLR_LN); // clear line
	usartSendStr(VT_SET_RED_FG); // red color
	usartSendStr(errorMsg);
	usartSendStr(VT_RESET_ATTRS);
	errorFlag = 1;
	P1OUT = 0xFF;
	// GIMME BUZZ BUZZ BUZZ!!
	usartSendStr(VT_CURSOR_RESTORE);
}

void guiClearError()
{
	usartSendStr(GUI_ERROR_POS_S); // set position
	usartSendStr(VT_CLR_LN); // clear lilne
	errorFlag = 0;
	P1OUT = 0;
	usartSendStr(VT_CURSOR_RESTORE);
}

uint8_t guiWasError()
{
	return errorFlag;
}
