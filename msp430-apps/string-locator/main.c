#include "header.h"
#include "insertMode.h"
#include "movingMode.h"
#include "usart.h"
#include "virtualTerminal.h"
#include "vtGui.h"

char textStr[DISPLAY_WIDTH+1] = { '<', 's', 'a', 'm', 'p', 'l', 'e', '>', '\0' };
int8_t textLen = 8;
uint8_t textX = (DISPLAY_X + (DISPLAY_WIDTH/2) - (8/2)); // 2+10-8=4
uint8_t textY = (DISPLAY_Y + (DISPLAY_HEIGHT/2) - (1/2)); // 15
char textVtPos[10] = "\x1b[4;15";

const ModeKey modeKeys[] = {
		 '1', enterInsertMode ,
		 '2', enterMovingMode ,
		 VT_KEY_CTRL_R, guiDisplayAll
};
const uint8_t MODE_KEYS_N = (sizeof(modeKeys) / sizeof(ModeKey));

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
	initUsart();
}

int main(void) {
	initProgram();
	guiDisplayAll();
	usartRxEint();
	uint8_t rxData;
	int8_t i;

	_EINT();
    while(1)
    {
    	usartRxDint();
    	if(!usartIsCharAvailable())
    	{
    		usartRxEint();
    		goSleepEint();
    	}
    	else
    	{
    		rxData = usartGetChar();
    		usartRxEint();

    		for(i = MODE_KEYS_N ; i > 0 ; i--)
    			if(rxData == modeKeys[(uint8_t)i].key)
    				break;
    		if(i == -1)
    			guiSetError("WRONG KEY");
    		else
    		{
    			if(guiWasError()) // jeśli poprzednio źle wklepano
    				guiClearError(); // skasuj czerwony napis na dole
    			modeKeys[(uint8_t)i].keyFunction();
    		}
    	}
    }
}
