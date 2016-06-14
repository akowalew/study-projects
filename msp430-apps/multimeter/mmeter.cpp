/*
 * mmeter.c
 *
 *  Created on: 13 cze 2016
 *      Author: dicker
 */

#include "mmeter.h"

uint8_t const Mmeter::counts[] = { 1, 2, 4, 8, 16} ;
uint8_t Mmeter::measureMode = Mmeter::MMETER_MIN;
uint8_t Mmeter::countsIndex = 0;
uint8_t Mmeter::elapsedCounts = 1;
uint16_t Mmeter::v0 = 0xFFFF, Mmeter::v1 = 0xFFFF;
uint16_t Mmeter::readyV0 = 0, Mmeter::readyV1 = 0;
uint8_t Mmeter::readyFlag = 0;

void Mmeter::nextMode()
{
	if(measureMode == MMETER_AVG)
		measureMode = MMETER_MIN;
	else
		++measureMode;
	restoreMode();
}

void Mmeter::nextCounts()
{
	if(countsIndex == COUNTS_N-1)
		countsIndex = 0;
	else
		++countsIndex;
	restoreMode();
}

void Mmeter::restoreMode()
{
	if(measureMode == MMETER_MIN)
		v0 = v1 = 0xFFFF;
	else
		v0 = v1 = 0;

	elapsedCounts = counts[countsIndex];
}

void Mmeter::addMeasure(uint16_t m0, uint16_t m1)
{
	switch(measureMode)
	{
	case Mmeter::MMETER_MIN: // MIN
		if(m0 < v0)
			v0 = m0;
		if(m1 < v1)
			v1 = m1;
		break;
	case  Mmeter::MMETER_MAX: // MAX
		if(m0 > v0)
			v0 = m0;
		if(m1 < v1)
			v1 = m1;
		break;
	case  Mmeter::MMETER_AVG:
		v0 += m0;
		v1 += m1;
		break;
	default:
		__never_executed();
	}

	if(--elapsedCounts)
	{
		if(measureMode == MMETER_AVG)
		{
			readyV0 = (v0 * MMETER_VREF) >> countsIndex;
			readyV1 = (v1 * MMETER_VREF) >> countsIndex;
		}
		else
		{
			readyV0 = v0;
			readyV1 = v1;
		}

		restoreMode();
		readyFlag = 1;
	}
}


char * mySimpleItoa(uint16_t val, char dest[Mmeter::STR_LEN])
{
	uint8_t i = 0;
	while(val >= 1000)
	{
		val -= 1000;
		++i;
	}
	*(dest++) = '0' + i;
	i = 0;
	*(dest++) = '.';

	while(val >= 100)
	{
		val -= 100;
		++i;
	}
	*(dest++) = '0' + i;
	i = 0;

	while(val >= 10)
	{
		val -= 10;
		++i;
	}
	*(dest++) = '0' + i;
	*(dest++) = '0' + val;

	return dest;
}

void Mmeter::getStr(char dest[Mmeter::STR_LEN])
{
	readyFlag = 0;
	dest = mySimpleItoa(readyV0, dest);
	*(dest++) = (readyV0 > readyV1 ? '>' : (readyV0 < readyV1 ? '<' : '='));
	mySimpleItoa(readyV1, dest);
	*(dest) = '\0';
}
