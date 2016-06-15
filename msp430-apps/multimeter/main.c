#include <msp430.h>
#include <signal.h>
#include <stdlib.h>
#include "LCD.h"
#include "system.h"
#include "multimeter.h"

int main(void)
{
    initAll();

    ADC12CTL0 |= ADC12SC | ENC ; // start conversion
	while(1)
	{
		goSleep(); // After wake up, GIE is cleared
		// _DINT();
		do {
			if(programFlags & FLAG_ADC_COMPLETE)
			{
				programFlags &= ~FLAG_ADC_COMPLETE;
				_EINT();
				multimeterNewMeasure();
				_DINT();
			}

			if(programFlags & FLAG_DEBOUNCE)
			{
				programFlags &= ~FLAG_DEBOUNCE;
				_EINT();
				buttonsChanged();
				_DINT();
			}
		} while(programFlags);
	}
}
