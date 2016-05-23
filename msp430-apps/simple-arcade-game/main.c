#include <msp430.h> 
#include "header.h"
#include <signal.h>

void initClock()
{
	// XT2 ON, LFXT1 LOW FREQ, ACLK DIV = 0, RSEL = 0
	BCSCTL1 = 0 ;
	// MCLK = DCOCLK, SMCLK = XT2CLK, DIV = 0
	// XT2S - 3-16MHz, LFXT1S = 32768HZ
	uint16_t i;
	do { // Wait for XT2 to stablize
		IFG1 &= ~OFIFG;                     // Clear OSCFault flag
		for (i = 0xFFF; i > 0; i--);        // Time for flag to set
	} while ((IFG1 & OFIFG) == OFIFG);    // OSCFault flag still
	BCSCTL2 |= SELM1+SELS;
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

/**
 * Przerwanie TACCR0, Odswiezanie wyswietlacza
 */
volatile uint8_t isDisplayEmpty;
uint8_t digitMask = 0x01;
uint8_t digitIndex = 0;
uint8_t displayArray[8];
uint8_t *outSegsPtr = &displayArray[0];
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0(void)
{
	TACCR0 += DISPLAY_TCCR;

	DISP_SEGS_OUT = 0xFF;
	DISP_DIGITS_OUT = ~digitMask;
	DISP_SEGS_OUT = *(outSegsPtr++);

	digitMask <<= 1;
	if(digitMask == 0)
    {
		digitMask = 0x01;
        outSegsPtr = &displayArray[0];
    }
}

void initDisplay()
{
	DISP_DIGITS_DIR = 0xFF;
	DISP_SEGS_DIR = 0xFF;
    isDisplayEmpty = 1;
}



/**
 * Przerwanie TCCR0B, Przesuwanie pociskow
 */
uint8_t leftGamer = 0, rightGamer = 0;
uint8_t boomVar = 0;
int8_t leftLastBulletPos = 7, rightLastBulletPos = 0;
void updateDisplay();
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

    updateDisplay();
}

void updateDisplay()
{
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

    _DINT();
    if(!(leftGamer || rightGamer || boomVar)) //nothing to display
    {
        isDisplayEmpty = 1;
        TACCTL0 = 0; // turn off display refreshing
        if(!isDebouncingNow())
        	TACTL &= ~MC_2;
        TBCTL &= ~MC_1; // no bullet to shift
    }
    else
    {
        isDisplayEmpty = 0;
        TACCTL0 = CCIE; // turn on display refreshing
        TACTL |= MC_2; // turn on timerA
        TBCTL |= MC_1; // turn on shifting
    }
    _EINT();
}





int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initClock();
    initTimers();
    initDisplay();
    initButtons();
    _EINT();

    uint16_t tmpTACCTL1State;
    uint8_t isDisplayUpdate = 0;
    while(1)
    {
    	_BIS_SR(LPM1_bits); // go sleep

    	isDisplayUpdate = 0;

        P1IE = 0x00; // disable PORT1 ints
    	tmpTACCTL1State = TACCTL1;
    	TACCTL1 &= ~CCIE;
    	if(isButtonEvent(BTN_LEFT))
    	{
    		clearButtonEvent(BTN_LEFT);
    		leftGamer |= 0x80;
    		isDisplayUpdate = 1;
            TBCTL |= MC_1;
    	}

    	if(isButtonEvent(BTN_RIGHT))
    	{
    		clearButtonEvent(BTN_RIGHT);
    		rightGamer |= 0x01;
    		isDisplayUpdate = 1;
            TBCTL |= MC_1;
    	}

    	if(isDisplayUpdate)
    		updateDisplay();

		TACCTL1 = tmpTACCTL1State;
        P1IE = BTN_ALL;
   }
}

