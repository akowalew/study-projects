#include "header.h"
#include "insertMode.h"
#include "movingMode.h"
#include "usart.h"
#include "virtualTerminal.h"
#include "vtGui.h"

const F_PTR modeKeysFunctions[MODE_KEYS_N] =
{
		enterInsertMode,
		enterMovingMode ,
		guiDisplayAll,
};

const uint8_t modeKeys[MODE_KEYS_N] =
{
		'1', // INSERT MODE
		'2', // MOVING MODE
		VT_ESC_KEY // DISPLAY ALL
};

const char wrongKeyErr[] = "WRONG KEY";

char currText[DISPLAY_WIDTH+1] = { '<', 's', 'a', 'm', 'p', 'l', 'e', '>', '\0' };
uint8_t currTextLen = DEFAULT_TEXT_LEN;
uint8_t currTextX = DEFAULT_TEXT_X, currTextY = DEFAULT_TEXT_Y;
char currTextVtPos[10] = DEFAULT_TEXT_VT_POS;

inline void initClock()
{
	// XT2 ON, LFXT1 LOW FREQ, ACLK DIV = 0, RSEL = 0
	BCSCTL1 = 0;
	BCSCTL2 = 0;
	// MCLK = XT2, SMCLK = XT2CLK, DIV = 0
	// XT2S - 3-16MHz, LFXT1S = 32768HZ
	uint16_t i;
	do { // Wait for XT2 to stablize
		IFG1 &= ~OFIFG; // Clear OSCFault flag
		for (i = 0xFFF; i > 0; i--); // Time for flag to set
	} while (IFG1 & OFIFG);    // OSCFault flag still
	BCSCTL2 = SELM1+SELS;
}

static inline void initProgram()
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	initClock();
}

int main(void) {
	initProgram();
	guiDisplayAll();

	uint8_t rxData;
	int8_t i;
    while(1)
    {
    	_DINT();
    	if(!usartIsCharAvailable())
    	{
    		_EINT();
    		goSleep();
    	}
    	else
    	{
    		_EINT();
    		rxData = usartGetChar();
    		for(i = MODE_KEYS_N ; i > 0 ; i--)
    			if(rxData == modeKeys[(uint8_t)i])
    				break;
    		if(i == -1)
    			guiSetError(wrongKeyErr);
    		else
    		{
    			if(guiWasError()) // jeśli poprzednio źle wklepano
    				guiClearError(); // skasuj czerwony napis na dole
    			modeKeysFunctions[(uint8_t)i]();
    		}
    	}
    }
}
