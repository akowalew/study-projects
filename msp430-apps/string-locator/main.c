#include <msp430.h> 
#include <stdint.h>
#include <signal.h>
#include "usartBuffers.h"

void initClock()
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



void initTimer()
{
	TACTL = (TASSEL0 | MC0); // UP MODE, ACLK
	TACCTL0 = CCIE;
	TACCR0 = 32767;
}

volatile char tablica[8];
volatile uint8_t i;
volatile uint8_t c = 0;

#pragma vector=TIMERA0_VECTOR
__interrupt void timerA0Int(void)
{
	/*
	uint8_t j;

	for(j = 1 ; j < 7 ; j++)
		tablica[j] = 'a';
	tablica[7] = 'a'+(c++);
	if(c == 25)
		c = 0;

	i = 0;
	U0TXBUF = 0x0C; */
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    initClock();
    initUsart();
    initTimer();

    P1DIR = 0xFF;
    P1OUT = 0;

    _EINT();

    uint8_t data;
    while(1)
    {
    	if(!(usartStateRx() & USART_BUFFER_EMPTY))
    	{
    		data = usartPop(USART_RX);
    		usartPush(USART_TX, data);
    	}
    }
}
