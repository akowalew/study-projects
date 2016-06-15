/*
 * multimeter.c
 *
 *  Created on: 15 cze 2016
 *      Author: dicker
 */

#include "multimeter.h"

/*
 * Sample display view:
 *
 * ******************
 * *3.142=3.100     *
 * * MIN MAX[AVG] 16*
 * ******************
 */

const uint8_t counts[MMETER_COUNTS_N] = { 1, 2, 4, 8, 16 };

struct
{
	uint8_t measureMode;
	uint8_t readyFlag;
	uint8_t countsIndex ;
	uint8_t elapsedCounts;

	uint16_t v0 , v1;
	uint16_t readyV0, readyV1 ;
} mmeter;


#pragma DATA_SECTION( defaultMeasMode, ".infoB" );
int defaultMeasMode = 0;

#pragma DATA_SECTION( defaultCountsIndex, ".infoB" );
int defaultCountsIndex = 0;
void multimeterSaveToFlash()
{
	// save to flash
	WDTCTL = WDTPW | WDTHOLD;
	_DINT();

	FCTL2 = FWKEY | FSSEL0 | 24 ;
	FCTL3 = FWKEY;
	FCTL1 = FWKEY | ERASE;
	defaultMeasMode = 0x00;

	FCTL1 = FWKEY | WRT;
	defaultMeasMode = mmeter.measureMode;

	FCTL1 = FWKEY | WRT;
	defaultCountsIndex = mmeter.countsIndex;
	FCTL3 = FWKEY | LOCK;
	_EINT();
	// reenable WDT
}

char * mySimpleItoa(uint16_t val, char dest[MMETER_STR_LEN])
{ // it is NOT inserting an '\0' char at the end!!!
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

void multimeterGetMeasStr(char dest[MMETER_STR_LEN])
{
	dest = mySimpleItoa(mmeter.readyV0, dest);
	*(dest++) = (mmeter.readyV0 > mmeter.readyV1 ? '>' :
			(mmeter.readyV0 < mmeter.readyV1 ? '<' : '='));
	dest = mySimpleItoa(mmeter.readyV1, dest);
	*(dest) = '\0';
}

void multimeterDisplayAll()
{
	char text[17] = " MIN MAX AVG    " ;
	uint8_t i = mmeter.measureMode;

	text[i*4] = '[';
	text[(i*4)+4] = ']';

	if(mmeter.countsIndex == 16)
	{
		text[14] = '1';
		text[15] = '6';
	}
	else
	{
		text[15] = '0' + mmeter.countsIndex;
	}
	text[16] = '\0';

	lcdSetPos(0, 1);
	lcdSendStr(text);

	multimeterGetMeasStr(text);
	lcdSetPos(0,0);
	lcdSendStr(text);
}

void multimeterUpdateData()
{
	if(mmeter.measureMode == MMETER_MIN)
		mmeter.v0 = mmeter.v1 = 0xFFFF;
	else
		mmeter.v0 = mmeter.v1 = 0;

	mmeter.elapsedCounts = counts[mmeter.countsIndex];
}

void switchMode()
{
	if(mmeter.measureMode == MMETER_AVG)
		mmeter.measureMode = MMETER_MIN;
	else
		++mmeter.measureMode;
	multimeterUpdateData();
	multimeterSaveToFlash();

	char nibble[4] = { ' ', ' ', ' ', ' ' };
	uint8_t i = mmeter.measureMode;
	nibble[i] = '[' ; nibble[i+1] = ']';

	i = 3 ; uint8_t x = 12 ;
	do
	{
		lcdSetPos(x, 1);
		lcdSendChr(nibble[i]);
		x -= 4;
	}while(i--);
}

void switchSize()
{
	if(mmeter.countsIndex == MMETER_COUNTS_N-1)
		mmeter.countsIndex = 0;
	else
		++mmeter.countsIndex;
	multimeterUpdateData();
	multimeterSaveToFlash();

	lcdSetPos(14,1);
	uint8_t i = mmeter.countsIndex;
	if(i == 16)
		lcdSendStr("16");
	else
	{
		lcdSendChr(' ');
		lcdSendChr('0' + i);
	}
}

void multimeterAddMeasure(uint16_t m0, uint16_t m1)
{
	switch(mmeter.measureMode)
	{
	case MMETER_MIN: // MIN
		if(m0 < mmeter.v0)
			mmeter.v0 = m0;
		if(m1 < mmeter.v1)
			mmeter.v1 = m1;
		break;
	case  MMETER_MAX: // MAX
		if(m0 > mmeter.v0)
			mmeter.v0 = m0;
		if(m1 > mmeter.v1)
			mmeter.v1 = m1;
		break;
	case  MMETER_AVG:
		mmeter.v0 += m0;
		mmeter.v1 += m1;
		break;
	default:
		__never_executed();
	}

	if(!--mmeter.elapsedCounts)
	{
		uint32_t volt0 = (uint32_t)(mmeter.v0), volt1 = (uint32_t)(mmeter.v1);
		if(mmeter.measureMode == MMETER_AVG)
		{
			mmeter.readyV0 = ((volt0 * MMETER_VREF) >> mmeter.countsIndex) >> 12;
			mmeter.readyV1 = ((volt1 * MMETER_VREF) >> mmeter.countsIndex) >> 12;
		}
		else
		{
			mmeter.readyV0 = (volt0 * MMETER_VREF) >> 12; // / 4096
			mmeter.readyV1 = (volt1 * MMETER_VREF) >> 12; // / 4096
		}

		multimeterUpdateData();
		mmeter.readyFlag = 1;
	}
}

void multimeterNewMeasure()
{
	multimeterAddMeasure(adcArray[0], adcArray[1]);

	if(mmeter.readyFlag)
	{
		mmeter.readyFlag = 0;
		char measurments[MMETER_STR_LEN];
		multimeterGetMeasStr(measurments);
		lcdSetPos(0, 0);
		lcdSendStr(measurments);
	}

	ADC12CTL0 |= ADC12SC | ENC ; // start conversion
}



void multimeterInit()
{
	P4OUT = defaultMeasMode;
	P5OUT = defaultCountsIndex;

	mmeter.measureMode = defaultMeasMode;
	mmeter.countsIndex = defaultCountsIndex;

	multimeterDisplayAll();
}
