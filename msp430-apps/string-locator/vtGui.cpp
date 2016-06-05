/*
 * vtGui.cpp
 *
 *  Created on: 5 cze 2016
 *      Author: dicker
 */
#include "vtGui.h"

extern uint8_t currTextX;
extern uint8_t currTextY;
extern char currText[DISPLAY_WIDTH+1];
extern uint8_t currTextLen;
extern char currTextVtPos[10];

uint8_t errorFlag = 0;

const char beginString[] =
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


inline void guiDrawDisplayBox()
{
	int8_t x;
	usartSendStr(BOX_VT_POS); //vtSetCursor(BOX_X, BOX_Y);
	usartSendStr(VT_CURSOR_SAVE);

	// upper line
	usartSendChr('+');
	for(x = DISPLAY_WIDTH ; x > 0 ; --x)
		usartSendChr('-');
	usartSendChr('+');

	usartSendStr(VT_CURSOR_RESTORE);
	usartSendStr(VT_CURSOR_DOWN);
	// left line
	for(x = DISPLAY_HEIGHT ; x > 0; --x)
	{
		usartSendChr('|');
		usartSendStr(VT_CURSOR_DOWN);
		usartSendStr(VT_CURSOR_LEFT);
	}

	// lower line
	usartSendChr('+');
	for(x = DISPLAY_WIDTH ; x > 0 ; --x)
		usartSendChr('-');
	usartSendChr('+');

	usartSendStr(VT_CURSOR_UP);
	// right line
	for(x = DISPLAY_HEIGHT ; x > 0; --x)
	{
		usartSendChr('|');
		usartSendStr(VT_CURSOR_UP);
		usartSendStr(VT_CURSOR_LEFT);
	}
}

void guiDisplayAll()
{
	usartSendStr(VT_CLEAR_ALL);
	usartSendStr(VT_HOME);

	usartSendStr(beginString);
	guiDrawDisplayBox();

	// place current text in the box
	usartSendStr(currTextVtPos); // vtSetCursor(currTextX, currTextY);
	usartSendStr(currText);
}

void guiSetStatusStr(const char * const statusStr)
{
	usartSendStr(STATUS_INFO_VT_POS);
	usartSendStr(statusStr);
}

void guiSetError(const char * const errorMsg)
{
	usartSendStr(STATUS_ERROR_VT_POS); // set position
	usartSendStr(VT_CLR_LN); // clear line
	usartSendStr(VT_SET_RED_FG); // red color
	usartSendStr(errorMsg);
	usartSendStr(VT_RESET_ATTRS);
	errorFlag = 1;

	// MAYBE BUZZER?
}

#pragma FUNC_ALWAYS_INLINE
void guiClearError()
{
	usartSendStr(STATUS_ERROR_VT_POS); // set position
	usartSendStr(VT_CLR_LN); // clear lilne
	errorFlag = 0;
}

#pragma FUNC_ALWAYS_INLINE
uint8_t guiWasError()
{
	return errorFlag;
}

