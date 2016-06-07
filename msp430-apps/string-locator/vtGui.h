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

void guiDisplayAll();

#define BUZZER 0x01
#define BUZZER_OUT P1OUT
#define BUZZER_DIR P1DIR

#define BUZZER_DELAY	9216

void guiSendError() ;
void guiInit();

#endif /* VTGUI_H_ */
