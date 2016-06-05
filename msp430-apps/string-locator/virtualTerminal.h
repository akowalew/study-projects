/*
 * serialScreen.h
 *
 *  Created on: 5 cze 2016
 *      Author: dicker
 */

#ifndef VT100_
#define VT100_

#include "header.h"
#include "usart.h"

void guiSetError(const char * const);
void guiClearError();
void vtDisplayAll();

void vtSetCursor(uint8_t x, uint8_t y);

#define VT_CLR_LN "\x1b[2K"
#define VT_CLEAR_ALL ("\x1b[2J" "\x1b[0m" "\x1b?25l" "\x1b[H") // clear screen and attributes, hide cursor, go to home

#define VT_HOME	"\x1b[H"
#define VT_CURSOR_OFF "\x1b[?25l"
#define VT_CURSOR_ON "\x1b[?25h"
#define VT_CURSOR_SAVE "\x1b[s"
#define VT_CURSOR_RESTORE "\x1b[u"
#define VT_CURSOR_UP	"\x1b[A"
#define VT_CURSOR_DOWN	"\x1b[B"
#define VT_CURSOR_RIGHT	"\x1b[C"
#define VT_CURSOR_LEFT	"\x1b[D"

#define VT_SET_BOLD	"\x1b[1m"
#define VT_RESET_ATTRS "\x1b[0m"
#define VT_SET_RED_FG "\x1b[31m"
#define VT_SET_CYAN_FG "\x1b[36m"

#define VT_ESC_KEY 0x1B

#define VT_MAX_CURSOR 80

#endif /* VT100_ */
