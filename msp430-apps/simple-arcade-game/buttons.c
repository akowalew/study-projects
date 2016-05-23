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

volatile uint8_t btnIntState;
/**
 * 	kierunek : wejscie, przerwania, zbocze opadajace
 */
void buttonsInit()
{
	BTN_DIR &= ~(  BTN_RIGHT | BTN_LEFT) ;
	BTN_IES |= (  BTN_RIGHT | BTN_LEFT ) ;
	if(BTN_IN & BTN_LEFT)
		buttonStates |= BTN_LEFT;
	if(BTN_IN & BTN_RIGHT)
		buttonStates |= BTN_RIGHT;
	BTN_IE |= ( BTN_RIGHT | BTN_LEFT) ;
}

#pragma vector=BTN_VECTOR
__interrupt void BtnInt(void)
{
	TBCTL |= MC_2; // turn on TimerB
	TBCCTL0 = CCIE; // turn on debunce compare int

    uint8_t mask;
    uint8_t i;
    for(i = 0 ; i < BTN_N ; i++)
    {
        mask = buttonMasks[i];
        if(P1IFG & mask)
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
	uint8_t hasToWakeUp = 0;
	uint8_t i;
	for(i = 0 ; i < BTN_N ; i++)
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
		TBCCTL0 &= ~CCIE; // Turn off TACCR1
}

uint8_t isButtonPressed(uint8_t buttonMask)
{
	TBCCTL0 = 0;
	uint8_t ret = (buttonEvents & buttonMask);
	if(ret)
		buttonEvents &= ~buttonMask;
	TBCCTL0 = CCIE;
	return ret;
}

inline void disableButtonsInt()
{
	btnIntState = TBCCTL0;
	BTN_IE = 0;
	TBCCTL0 &= ~CCIE;
}

inline void restoreButtonsInt()
{
	TBCCTL0 = btnIntState;
	BTN_IE = BTN_ALL;
}
