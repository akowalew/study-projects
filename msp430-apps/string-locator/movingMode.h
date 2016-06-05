/*
 * movingMode.h
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */

#ifndef MOVINGMODE_H_
#define MOVINGMODE_H_

#include "header.h"

void enterMovingMode();

#define FN_ARROW_PREFIX 0x5B
void movingUp();
void movingDown();
void movingLeft();
void movingRight();
void movingRestore();

#endif /* MOVINGMODE_H_ */
