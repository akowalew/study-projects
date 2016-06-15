/*
 * multimeter.h
 *
 *  Created on: 15 cze 2016
 *      Author: dicker
 */

#ifndef MULTIMETER_H_
#define MULTIMETER_H_

#include "header.h"
#include "system.h"
#include "LCD.h"

#define MMETER_MEMORY_ADDR 0x1080

#define MMETER_COUNTS_N 5
#define MMETER_STR_LEN 12

#define MMETER_MIN  0x00
#define MMETER_MAX  0x01
#define MMETER_AVG  0x02

#define MAX_COUNT  16
#define MMETER_VREF  3300

void multimeterNewMeasure();
void switchMode();
void switchSize();
void multimeterInit();
void multimeterDisplayAll();

#endif /* MULTIMETER_H_ */
