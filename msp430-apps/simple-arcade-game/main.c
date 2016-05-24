#include "header.h"
#include "game.h"
#include "buttons.h"
#include "display.h"

void initClock()
{
	// XT2 ON, LFXT1 LOW FREQ, ACLK DIV = 0, RSEL = 0
	BCSCTL1 = 0 ;
	// MCLK = XT2, SMCLK = XT2CLK, DIV = 0
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
	/* 	TIMER_A ( Taktowanie 32768Hz )
		-	Odswiezanie wyswietlacza -> 400Hz(~404,5Hz) : TACCR0 = 81 - 1
		- 	Przesuwanie kursora -> 2Hz : 2^14 - 1
		TIMER_B ( Taktowanie 32768Hz )
		-	Debounce - 1024Hz : TACCR1 = 32 - 1
	*/
	// ACLK, DIVIDER=1, CONTINUOUS, INT ENABLE
	TACTL = (TASSEL_1 | ID_0 /* |  MC_2 */ );
	// NO CAPTURE, INT ENABLE
	TACCTL0 =  CCIE /* CCIE */ ;
	TACCR0 = DISPLAY_TCCR;
	TACCTL1 = CCIE  /* CCIE */ ;
	TACCR1 = GAME_SHIFT_TCCR;

	// SMCLK, UP_MODE, INT ENABLE
	TBCTL = (TBSSEL_1 /* | MC_1 */ );
	TBCCTL0 = CCIE;
	TBCCR0 = DEBOUNCE_TCCR;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initClock();
    initTimers();
    displayInit();
    buttonsInit();
    gameInit();
    _EINT();

    uint8_t isGameChanged;
    while(1)
    {
    	_BIS_SR(SLEEP_BITS); // go sleep

    	while(1)
    	{
			isGameChanged = 0;
			if(isButtonPressed(BTN_LEFT))
				if(gameBulletLeftAdd())
					isGameChanged = 1;

			if(isButtonPressed(BTN_RIGHT))
				if(gameBulletRightAdd())
					isGameChanged = 1;

			if(gameIsNextCycle())
			{
				gameGoNextCycle();
				isGameChanged = 1;
			}

			if(isGameChanged)
				gameUpdate();
			else
				break;
    	}
   }
}

