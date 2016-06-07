/*
 * vtGui.cpp
 *
 *  Created on: 5 cze 2016
 *      Author: dicker
 */
#include "vtGui.h"

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

	const char * const horizontalSeq1 = (
			"|"
			VT_CURSOR_DOWN
			VT_CURSOR_LEFT );
	for(x = DISPLAY_HEIGHT ; x > 0; --x) // left line
		usartSendStr(horizontalSeq1);

	usartSendStr(line); // downline

	const char * const horizontalSeq2 = (
			VT_CURSOR_LEFT
			VT_CURSOR_UP
			"|" );
	for(x = DISPLAY_HEIGHT ; x > 0; --x) // right line
		usartSendStr(horizontalSeq2);
}

void guiDisplayAll()
{
	const char * const resetSeq = ("\x1b\x63" "\x1b[2J" "\x1b[H" "\x1b[?25l");
			// VT_RESET, VT_CLR_SCR, VT_HOME, VT_CURSOR_OFF
	usartSendStr(resetSeq);

	const char * const beginString =
			"Hi, here you can configure\r\n"
			"your display.\r\n"
			"\n"
			"Available modes:\r\n"
			"(1) - INSERT - just type your text\r\n"
			"(2) - MOVING - use arrows to move string\r\n"
			"\n"
			"In every mode: <ENTER> to confirm, <CTRL> + <R> to reset\r\n"
			"\n"
			"Your display:\r\n";

	usartSendStr(beginString);
	guiDrawDisplayBox();

	// default text init
	char *p = textStr; char *p2 = DEFAULT_TEXT;
	while(*(p++) = *(p2++));
	textLen = DEFAULT_TEXT_LEN;
	textX = DISPLAY_X;
	textY = DISPLAY_Y;

	// place current text in the box
	const char textDefaultVtPos[10] = "\x1b[13;2H";
	usartSendStr(textDefaultVtPos);
	usartSendStr(VT_CURSOR_SAVE);
	usartSendStr(textStr);
}


inline void usartSendError()
{
	usartSendChr('\x07');
}

