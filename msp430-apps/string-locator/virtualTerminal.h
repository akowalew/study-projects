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

#define VT_CLR_SCR "\x1b[2J"
#define VT_CLR_CHR "\b \b"

#define VT_HOME "\x1b[H"
#define VT_CURSOR_OFF "\x1b[?25l"
#define VT_CURSOR_ON "\x1b[?25h"
#define VT_CURSOR_SAVE "\x1b[s"
#define VT_CURSOR_RESTORE "\x1b[u"
#define VT_CURSOR_UP 	"\x1b[1A"
#define VT_CURSOR_DOWN 	"\x1b[1B"
#define VT_CURSOR_RIGHT "\x1b[1C"
#define VT_CURSOR_LEFT 	"\x1b[1D"

#define VT_RESET_ATTRS  "\x1b[0m"
#define VT_SET_GREEN  "\x1b[32m"

#define VT_KEY_ESC 0x1B
#define VT_KEY_CTRL_R 0x12
#define VT_KEY_ENTER 0x0D
#define VT_KEY_BACKSPACE 127

#endif /* VT100_ */
