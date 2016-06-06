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

void vtSetCursor(uint8_t x, uint8_t y);

extern const char* const VT_CLR_LN ;
extern const char* const VT_CLR_SCR;
extern const char* const VT_CLEAR_ALL ; // clear screen and attributes, hide cursor, go to home
extern const char* const VT_RESET;
extern const char* const VT_CLR_CHR ; // Clear previous character (backspace)

extern const char* const VT_HOME;
extern const char* const VT_CURSOR_OFF;
extern const char* const VT_CURSOR_ON ;
extern const char* const VT_CURSOR_SAVE;
extern const char* const VT_CURSOR_RESTORE ;
extern const char* const VT_CURSOR_UP ;
extern const char* const VT_CURSOR_DOWN ;
extern const char* const VT_CURSOR_RIGHT;
extern const char* const VT_CURSOR_LEFT;

extern const char* const VT_SET_BOLD;
extern const char* const VT_RESET_ATTRS;
extern const char* const VT_SET_RED_FG;
extern const char* const VT_SET_CYAN_FG;
extern const char* const VT_CLR_RESET;

#define VT_KEY_ESC 0x1B
#define VT_KEY_CTRL_R 0x12
#define VT_KEY_ENTER 0x0D
#define VT_KEY_BACKSPACE 0x08

#endif /* VT100_ */
