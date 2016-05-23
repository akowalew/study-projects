/*
 * game.h
 *
 *  Created on: 23 maj 2016
 *      Author: dicker
 */

#ifndef GAME_H_
#define GAME_H_

#include "header.h"
#include "buttons.h"
#include "display.h"

#define GAME_SHIFT_TCCR	16383

#define GAME_BULLET_LEFT	0xFB
#define GAME_BULLET_RIGHT	0xFA
#define GAME_BOOM			0xF8
#define GAME_NONE			0xFF

void gamePause();
void gameResume();

void gameBulletLeftAdd();
void gameBulletRightAdd();
void gameUpdate();

uint8_t gameIsNextCycle();
void gameGoNextCycle();

#endif /* GAME_H_ */
