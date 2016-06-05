/*
 * vtGui.h
 *
 *  Created on: 5 cze 2016
 *      Author: dicker
 */

#ifndef VTGUI_H_
#define VTGUI_H_

#include "header.h"

#define DISPLAY_WIDTH 21
#define DISPLAY_HEIGHT 5

#define BOX_X 1
#define BOX_Y 12
#define BOX_VT_POS "\x1b[1;9"

#define DISPLAY_X (BOX_X+1) // 2
#define DISPLAY_Y (BOX_Y+1) // 13

#define BOX_WIDTH (DISPLAY_WIDTH+2)
#define BOX_HEIGHT (DISPLAY_HEIGHT+2)

#define DEFAULT_TEXT "<sample>"
#define DEFAULT_TEXT_LEN (sizeof(DEFAULT_TEXT) - 1) // 8
#define DEFAULT_TEXT_X (DISPLAY_X + (DISPLAY_WIDTH/2) - (DEFAULT_TEXT_LEN/2)) // 2+10-8=4
#define DEFAULT_TEXT_Y (DISPLAY_Y + (DISPLAY_HEIGHT/2)) // 15
#define DEFAULT_TEXT_VT_POS	"\x1b[4;15"

#define STATUS_INFO_X 1
#define STATUS_INFO_Y (BOX_Y+DISPLAY_HEIGHT+4) // 21
#define STATUS_INFO_VT_POS "\x1b[1;21"

#define STATUS_ERROR_X 1
#define STATUS_ERROR_Y (STATUS_INFO_Y+1)
#define STATUS_ERROR_VT_POS "\x1b[1;22"

void guiDisplayAll();
uint8_t guiWasError();

#endif /* VTGUI_H_ */
