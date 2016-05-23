#include <msp430.h> 
#include "header.h"
#include <signal.h>

/**
 * 	kierunek : wejscie, przerwania, zbocze opadajace
 */
void initButtons()
{
	BTN_DIR &= ~(  BTN_RIGHT | BTN_LEFT) ;
	BTN_IES |= (  BTN_RIGHT | BTN_LEFT ) ;
	if(BTN_IN & BTN_LEFT)
		buttonStates |= BTN_LEFT;
	if(BTN_IN & BTN_RIGHT)
		buttonStates |= BTN_RIGHT;
	BTN_IE |= ( BTN_RIGHT | BTN_LEFT) ;
}

void initClock()
{
	// XT2 ON, LFXT1 LOW FREQ, ACLK DIV = 0, RSEL = 0
	BCSCTL1 = 0 ;
	// MCLK = DCOCLK, SMCLK = XT2CLK, DIV = 0
	BCSCTL2 = SELS ;
	// XT2S - 3-16MHz, LFXT1S = 32768HZ
	//BCSCTL3 = (1 << 7) ;
}

void initTimers()
{
	/* 	TIMER_A
		-	Odswiezanie wyswietlacza - 400Hz
		-	Debounce - 1024Hz

		TIMER_B
		-	Przesuwanie kursora - 2Hz
	*/

	// SMCLK, DIVIDER=1, CONTINUOUS, INT ENABLE
	TACTL = (TASSEL_2 | ID_0 /* |  MC_2 */ );
	// NO CAPTURE, INT ENABLE
	TACCTL0 =  0 /* CCIE */ ;
	TACCR0 = DISPLAY_TCCR;
	TACCTL1 = 0 /* CCIE */    ;
	TACCR1 = DEBOUNCE_TCCR;

	// SMCLK, UP_MODE, INT ENABLE
	TBCTL = (TBSSEL_1 /* | MC_1 */ );
	TBCCTL0 = CCIE;
	TBCCR0 = GAME_SHIFT_TCCR;
}

void initDisplay()
{
	DISP_DIGITS_DIR = 0xFF;
	DISP_SEGS_DIR = 0xFF;
    isDisplayEmpty = true;
}

/**
 * Przerwanie TACCR0, Odswiezanie wyswietlacza
 */
uint8_t digitMask = 0x01;
uint8_t digitIndex = 0;
uint8_t *outSegsPtr = &displayArray[0];
uint8_t displayArray[8];
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0(void)
{
	TACCR0 += DISPLAY_TCCR;

	DISP_DIGITS_OUT = 0xFF; // Turn off
	DISP_SEGS_OUT = *(outSegsPtr++);
	DISP_DIGITS_OUT = ~digitMask;

	digitMask <<= 1;
	if(digitMask == 0)
    {
		digitMask = 0x01;
        outSegsPtr = &displayArray[0];
    }
}

/**
 * Przerwanie TACCR1
 * -	Sprawdzanie debouncingu
 */
volatile uint8_t debounceFlags;
volatile uint8_t buttonStates;
volatile uint8_t buttonEvents;

const uint8_t buttonMasks[BTN_N] = {BTN_LEFT, BTN_RIGHT};
volatile uint8_t counts0[BTN_N];
volatile uint8_t counts1[BTN_N];
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{

	switch(TAIV)
	{
	case 2:
		TACCR1 += DEBOUNCE_TCCR;

        bool hasToWakeUp = false;
        for(uint8_t i = 0 ; i < BTN_N ; i++)
        {
            uint8_t mask = buttonMasks[i];
            if(debounceFlags & mask) // jeśli trzeba debouncować
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
                        hasToWakeUp = true; // wychodizmy ze snu tylko na nacisniecie
                    } 
                    else if( (!(buttonStates & mask)) && (counts1[i] > counts0[i]) )
                    {
                        buttonStates |= mask;
                    }

                    BTN_IE |= mask; // przywróć przerwania przycisku
                    counts0[i] = counts1[i] = 0;
                }
            }
        }

        if(hasToWakeUp)
            _BIC_SR_IRQ(LPM1_bits);

		if(!debounceFlags)	// nie licz juz wiecej
        {
			TACCTL1 &= ~CCIE; // Turn off TACCR1
            if(isDisplayEmpty) // nothing to do at all
                TACTL &= ~MC_2; // disable TimerA
        }

		break;
	}
}

/**
 * Przerwanie TCCR0B, Przesuwanie pociskow
 */
volatile bool isDisplayEmpty;
uint8_t leftGamer = 0, rightGamer = 0;
uint8_t boomVar = 0;
int8_t leftLastBulletPos = 7, rightLastBulletPos = 0;
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B0(void)
{
	boomVar = 0;
	uint8_t boomFlags = 0;

	if(rightGamer)
	{
		rightGamer <<= 1;
		rightLastBulletPos++;

		if(rightLastBulletPos == 8)
		{
			boomFlags |= 0x01 ; // RIGHT BOOM
			boomVar |= 0x80 ;
		}
	}

	if(leftGamer)
	{
		leftGamer >>= 1;
		leftLastBulletPos--;

		if(leftLastBulletPos == -1)
		{
			boomFlags |= 0x02; // LEFT BOOM
			boomVar |= 0x01;
		}
	}

	if(leftGamer && rightGamer)
	{
		if(leftLastBulletPos <= rightLastBulletPos)
		{
			boomVar |= (1 << leftLastBulletPos) | (1 << rightLastBulletPos);
			leftGamer &= ~(1 << leftLastBulletPos);
			rightGamer &= ~(1 << rightLastBulletPos);
			boomFlags = 0x03;
		}
	}

	if(boomFlags & 0x01) // RIGHT BOOM
	{
		rightLastBulletPos--;
		while(rightLastBulletPos > 0)
		{
			if(rightGamer & (1 << rightLastBulletPos))
				break ;
			rightLastBulletPos--;
		}
	}

	if(boomFlags & 0x02) // LEFT BOOM
	{
		leftLastBulletPos++;
		while(leftLastBulletPos < 7)
		{
			if(leftGamer & (1 << leftLastBulletPos))
				break;
			leftLastBulletPos++;
		}
	}

    uint8_t *ptr = displayArray, mask = 0x01;
    while(mask)
    {
    	uint8_t outSegs;
	    if(boomVar & mask)
		    outSegs = GAME_BOOM;
        else if(leftGamer & mask)
            outSegs = GAME_BULLET_LEFT;
        else if(rightGamer & mask)
            outSegs = GAME_BULLET_RIGHT ;
        else
            outSegs = GAME_NONE;
       *(ptr++) = outSegs;
        mask <<= 1 ; 
    }

    if(!(leftGamer || rightGamer || boomVar))
    {
        isDisplayEmpty = true;
        TACCTL0 = 0;
        TBCTL &= ~MC_1;
    }
    else
    {
        isDisplayEmpty = false;
        TACCTL0 = CCIE;
        TACTL |= MC_2; 
        TBCTL |= MC_1;
    }
}

#pragma vector=BTN_VECTOR
__interrupt void BtnInt(void)
{
	TACTL |= MC_2; // turn on TimerA
	TACCTL1 = CCIE; // turn on debunce compare int
    
    uint8_t mask;
    for(uint8_t i = 0 ; i < BTN_N ; i++)
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

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initClock();
    initTimers();
    initDisplay();
    initButtons();
    _EINT();

    uint16_t tmpTACCTL1State;
    while(1)
    {
    	_BIS_SR(LPM1_bits); // go sleep

        P1IE = 0x00; // disable PORT1 ints
    	tmpTACCTL1State = TACCTL1;
    	TACCTL1 &= ~CCIE;
    	if(buttonEvents & BTN_LEFT)
    	{
		    buttonEvents &= ~BTN_LEFT;
    		leftGamer |= 0x80;
            TBCTL |= MC_1;
    	}

    	if(buttonEvents & BTN_RIGHT)
    	{
    		buttonEvents &= ~BTN_RIGHT;
    		rightGamer |= 0x01;
            TBCTL |= MC_1;
    	}
		TACCTL1 = tmpTACCTL1State;
        P1IE = BTN_ALL;

		//TACTL |= MC_2;
		//TBCTL |= MC_1;
		//TACCTL0 = CCIE;
		//TBCCTL0 = CCIE;
   }

	return 0;
}
