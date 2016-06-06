/*
 * vtGui.h
 *
 *  Created on: 5 cze 2016
 *      Author: dicker
 */

#ifndef VTGUI_H_
#define VTGUI_H_

#include "header.h"

#define BOX_X 1
#define BOX_Y 12

#define DISPLAY_X (BOX_X+1) // 2
#define DISPLAY_Y (BOX_Y+1) // 13

#define GUI_INFO_X 1
#define GUI_INFO_Y (BOX_Y+DISPLAY_HEIGHT+4) // 21
#define GUI_INFO_VT_POS "\x1b[1;21"

#define GUI_ERROR_X 1
#define GUI_ERROR_Y (STATUS_INFO_Y+1)
#define GUI_ERROR_POS_S "\x1b[1;22"

extern const char * const WRONG_KEY_ERR;

void guiDisplayAll();
uint8_t guiWasError();
void guiSetError(const char * const);

void guiFatalError(const char * const);

void guiClearError();

#endif /* VTGUI_H_ */
