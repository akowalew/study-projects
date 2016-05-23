/*
 * buttons.c
 *
 *  Created on: May 23, 2016
 *      Author: student
 */
#include "buttons.h"

volatile uint8_t debounceFlags;
volatile uint8_t buttonStates;
volatile uint8_t buttonEvents;

const uint8_t buttonMasks[BTN_N] = {BTN_LEFT, BTN_RIGHT};
volatile uint8_t counts0[BTN_N];
volatile uint8_t counts1[BTN_N];

void buttonsInit() 	// kierunek : wejscie, przerwania, zbocze opadajace
{
	BTN_DIR &= ~BTN_ALL ;
	BTN_IES |= BTN_ALL ;
	buttonStates = BTN_ALL ; // na starcie wszystkie sa puszczone(trafne spostrzeżenie)
	BTN_IE |= BTN_ALL ;
}

#pragma vector=BTN_VECTOR
__interrupt void BtnInt(void)
{
	timerBTurnOn(); // also : TBCCTL0 = CCIE; // turn on debunce compare int

    uint8_t mask;
    int8_t i = BTN_N-1;
    for(i = BTN_N-1 ; i > 0 ; i--)
    {
        mask = buttonMasks[i];
        if(BTN_IFG & mask)
        {
            BTN_IE &= ~mask;
            debounceFlags |= mask; // turn on debounce of btn
            BTN_IES ^= mask; // invert edge
            BTN_IFG &= ~mask;
        }
    }
}

/**
 * Przerwanie TACCR1
 * -	Sprawdzanie debouncingu
 */
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B0(void)
{
	_EINT(); // umożliwiamy, by w tym czasie mógł odświeżać wyświetlacz(ryzykowne)

	uint8_t hasToWakeUp = 0;
	uint8_t i;
	for(i = BTN_N-1 ; i > 0 ; i--)
	{
		uint8_t mask = buttonMasks[i];
		if(debounceFlags & mask) // jesli trzeba debouncowac
		{
			if(BTN_IN & mask) // pobieramy probke
				counts1[i]++;
			else
				counts0[i]++;

			if((counts0[i] > BOUNCE_SAMPLES_N/2) || (counts1[i] > BOUNCE_SAMPLES_N/2))
			{
				debounceFlags &= ~mask; // konczymy debouncowanie
				if((buttonStates & mask) && (counts0[i] > counts1[i]))
				{
					buttonStates &= ~mask; // nacisniecie przycisku
					buttonEvents |= mask;
					hasToWakeUp = 1; // wychodizmy ze snu tylko na nacisniecie
				}
				else if( (!(buttonStates & mask)) && (counts1[i] > counts0[i]) )
					buttonStates |= mask;

				BTN_IE |= mask; // przywroc przerwania przycisku
				counts0[i] = counts1[i] = 0;
			}
		}
	}

	if(hasToWakeUp)
		_BIC_SR_IRQ(SLEEP_BITS);
	if(!debounceFlags)	// nie licz juz wiecej
		timerBTurnOff(); //TBCCTL0 = 0; // Turn off TBCCR0
}

uint8_t isButtonPressed(uint8_t buttonMask)
{
	TBCCTL0 = 0; // sekcja krytyczna
	uint8_t ret = (buttonEvents & buttonMask);
	if(ret)
		buttonEvents &= ~buttonMask;
	TBCCTL0 = CCIE;
	return ret;
}
