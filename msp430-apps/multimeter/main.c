#include <msp430.h>
#include <signal.h>
#include <stdlib.h>
#include "LCD.h"

inline void analogInit()
{
	P4DIR = 0;
	float a = 3.142 ;
	float b = 5.4;
	a = (float)(P4IN) + 0.01 ;
	a = 3.145;
	P5OUT = (uint8_t)(a+b);
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    displayInit();
    analogInit();

	displaySetPos(0,0);
	displayString("3.123 ");
	displayChar('>');
	displayString(" 6.432");

	displaySetPos(0,1);
	displayString("(MAX)MIN AVG ");
	displayString("256");

	while(1)
	{

	}
}
