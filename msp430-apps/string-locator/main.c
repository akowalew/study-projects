#include "header.h"
#include "insertMode.h"
#include "movingMode.h"
#include "usart.h"
#include "virtualTerminal.h"
#include "vtGui.h"

#pragma NOINIT(textStr)
char textStr[DISPLAY_WIDTH+1];
int8_t textLen;
uint8_t textX;
uint8_t textY;

void __low_level_init(void)
{

}

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

int main(void)
{
	initProgram();

	__enable_interrupt();
	guiDisplayAll();

	int8_t i; uint8_t rxData;
    while(1)
    {
    	rxData = usartGetChar_b();
		for(i = MODE_KEYS_N-1 ; i >= 0 ; i--)
			if(rxData == modeKeys[(uint8_t)i].key)
				break;
		if(i == -1)
			usartSendError();
		else
			modeKeys[(uint8_t)i].keyFunction(); // execute its function
    }
}
