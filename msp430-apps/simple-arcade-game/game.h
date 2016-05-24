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

#define GAME_SHIFT_TCCR	4095

#define GAME_BULLET_LEFT	0xFB
#define GAME_BULLET_RIGHT	0xFA
#define GAME_BOOM			0xF8
#define GAME_NONE			0xFF

#define GAME_BUZZER_TCCR 255
#define GAME_BUZZER_DIR	P4DIR
#define GAME_BUZZER_OUT	P4OUT
#define GAME_BUZZER 0x01

void gamePause();
void gameResume();

uint8_t gameBulletLeftAdd();
uint8_t gameBulletRightAdd();
void gameUpdate();
void gameInit();

uint8_t gameIsNextCycle();
void gameGoNextCycle();

#endif /* GAME_H_ */
