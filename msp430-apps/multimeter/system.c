/*
 * system.c
 *
 *  Created on: 15 cze 2016
 *      Author: dicker
 */
#include "header.h"
#include "LCD.h"
#include "multimeter.h"

volatile uint8_t programFlags = 0;
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

inline void initTimer()
{
	TACTL |= (TASSEL0 | TACLR);
	TACCTL0 = CCIE;
	TACCR0 = TIMERA0_DELAY;
}

inline void adcInit()
{
	P6SEL |= 0x03 ; // A0, A1
	// 32 cycles for sample fo 0:7 channels, multiple sample and convert, vref OFF
	ADC12CTL0 = SHT01 | SHT00 | MSC | ADC12ON;
	// ADC12OSC, Seq-of-channels, no div on CLK, SAMPCON = sampling timer, SHS = ADC12SC, STARTADD = 0
	ADC12CTL1 = SHP | CONSEQ0;
	// VREF = AVCC, A0, NOT_EOS
	ADC12MCTL0 = 0;
	// VREF = AVCC, A1, EOS
	ADC12MCTL1 = EOS | INCH0 ;
	// ADC12IE0, ADC12IE1
	ADC12IE = 0x03 ;
}

inline void buttonsInit()
{
	P1DIR = 0x00;
	//P1OUT = 0x00;
	P1IE = (BTN_0 | BTN_1);
	P1IES = (BTN_0 | BTN_1);
}

volatile uint16_t adcmem0 = 0, adcmem1 = 0;
#pragma vector=ADC12_VECTOR
__interrupt void adc12_ISR()
{
	switch(ADC12IV)
	{
	case 0x06 :
		adcmem0 = ADC12MEM0;
		break;
	case 0x08 :
		adcmem1 = ADC12MEM1;
		programFlags |= FLAG_ADC_COMPLETE;
		wakeUp();
		break;
	default:
		__never_executed(); // tract this switch as an assembler ADD to PC style
	}
}

volatile uint8_t delayRemain = 0;
volatile uint8_t debounceDelayRemain = 0;
#pragma vector=TIMERA0_VECTOR
__interrupt void timerA0_ISR()
{
	uint8_t isWorking = 0;

	TACCR0 += TIMERA0_DELAY;
	if(delayRemain)
	{
		--delayRemain;
		if(!delayRemain)
		{
			wakeUp();
			programFlags |= FLAG_DELAY_COMPLETE;
		}
		else
			isWorking |= 0x01;
	}

	if(debounceDelayRemain)
	{
		--debounceDelayRemain;
		if(!debounceDelayRemain)
		{
			programFlags |= FLAG_DEBOUNCE;
			wakeUp();
		}
		else
			isWorking |= 0x02;
	}

	if(!isWorking)
	{
		TACTL = TASSEL0 | TACLR;
		TACCR0 = TIMERA0_DELAY;
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void port1_ISR()
{
	TACTL |= MC1;//
	debounceDelayRemain = DEBOUNCE_CYCLES;
	P1IE &= ~(BTN_1 | BTN_0);
	P1IFG &= ~(BTN_1 | BTN_0);
}

void initAll()
{
	// Stop watchdog timer, Clear it, Source : ACLK, 1s wait
    WDTCTL = WDTPW | WDTHOLD | WDTCNTCL | WDTSSEL ;

    initClock();
    initTimer();
    adcInit();
    buttonsInit();
    lcdInit();

    if(IFG1 & WDTIFG) // if reset was caused by WDT
    {
    	lcdSetPos(0, 0);
    	lcdSendStr("System fault!");
    	waitDelay(512); // at least half second
    	// something to clear display!
    	IFG1 &= ~WDTIFG;
    }

    multimeterInit();
}

void waitDelay(uint16_t delay)
{
	_DINT();
	TACTL |= MC1;
	delayRemain = delay;
	while(1)
	{
		goSleep(); // after wakeUp GIE is cleared
		if(!delayRemain)
			break;
	}
	_EINT();
}

void buttonsChanged()
{
	if(!(P1IN & 0x01))
	{
		if(P1IN & 0x02) // p2 not pressed
			switchMode();
		WDTCTL = WDTPW | WDTSSEL ;
	}
	else if(!(P1IN & 0x02))
	{
		if(P1IN & 0x01)
			switchSize();
		WDTCTL = WDTPW | WDTSSEL ;
	}
	else
		WDTCTL = WDTPW | WDTCNTCL | WDTHOLD;

	P1IES = P1IN;
	P1IFG = 0;
	P1IE |= (BTN_1 | BTN_0);
}

