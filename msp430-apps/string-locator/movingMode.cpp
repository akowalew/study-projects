/*
 * movingMode.cpp
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */

#include "movingMode.h"

const char emptyTextErr[] = "TEXT IS EMPTY";
const char onlyArrowErr[] = "ONLY ARROWS";

const F_PTR arrowsKeysFunctions[] =
{
		movingDown,
		movingRight,
		movingUp,
		movingLeft,
		movingRestore
};
const uint8_t arrowsKeys[] =
{
		'B', // DOWN
		'C', // RIGHT
		'A', // UP
		'D', // LEFT
		VT_ESC_KEY // RESTORE
};
#define ARROW_KEYS_N (sizeof(arrowsKeys) / sizeof(uint8_t))

void enterMovingMode()
{

}



