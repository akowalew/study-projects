/*
 * mmeter.h
 *
 *  Created on: 13 cze 2016
 *      Author: dicker
 */

#ifndef MMETER_H_
#define MMETER_H_

#include "header.h"

class Mmeter
{
public:
	enum
	{
		MMETER_MIN = 0x00,
		MMETER_MAX = 0x01,
		MMETER_AVG = 0x02,

		COUNTS_N = 5,
		STR_LEN = 12,
		MAX_COUNT = 16,
		MMETER_VREF = 3300
	};

	static uint8_t getMode() { return measureMode; }
	static uint8_t getCounts() { return counts[countsIndex]; }
	static void nextMode();
	static void nextCounts();

	static uint8_t isReady() { return readyFlag; }

	static void addMeasure(uint16_t m0, uint16_t m1);
	static void getStr(char dest[STR_LEN]);
private :
	static const uint8_t counts[COUNTS_N];
	static uint8_t measureMode;
	static uint8_t readyFlag;
	static uint8_t countsIndex ;
	static uint8_t elapsedCounts;

	static uint16_t v0 , v1;
	static uint16_t readyV0, readyV1 ;

	static void restoreMode();
};
#endif /* MMETER_H_ */
