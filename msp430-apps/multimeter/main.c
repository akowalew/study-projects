#include <msp430.h>
#include <signal.h>
#include <stdlib.h>
#include "LCD.h"
#include "mmeter.h"

volatile uint8_t programFlags = 0;
volatile uint16_t adcmem0 = 0, adcmem1 = 0;
volatile uint8_t delayRemain = 0;
volatile uint8_t debounceDelayRemain = 0;

inline void initTimer()
{
	TACTL |= (TASSEL0 | TACLR);
	TACCTL0 = CCIE;
	TACCR0 = TIMERA0_DELAY;
}

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

inline void buttonsInit()
{
	P1DIR = 0x00;
	P1OUT = 0x00;
	P1IE = (BTN_0 | BTN_1);
	P1IES = (BTN_0 | BTN_1);
}

#pragma vector=TIMERA0_VECTOR
__interrupt void timerA0_ISR()
{
	uint8_t isWorking = 0x03;

	TACCR0 += TIMERA0_DELAY;
	if(delayRemain && (!--delayRemain))
	{
		wakeUp();
		isWorking &= ~0x01;
	}

	if(debounceDelayRemain && (!(--debounceDelayRemain)))
	{
		programFlags |= FLAG_DEBOUNCE;
		wakeUp();
		isWorking &= ~0x02;
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
	TACTL |= MC1;
	debounceDelayRemain = DEBOUNCE_CYCLES;
	P1IE &= ~(BTN_1 | BTN_2 | BTN_0);
	P1IFG &= ~(BTN_1 | BTN_2 | BTN_0);
}

inline uint8_t scanButtons()
{
	uint8_t ret = 3;
	uint8_t mask = BTN_2;
	uint8_t i = 2;
	do
	{
		if(!(P1IN & mask))
		{
			if(ret == 3)
				ret = mask;
			else {
				ret = 0xFF;
				break;
			}
		}
		mask >>= 1; --i;
	}while(mask);

	P1IES = P1IN;
	P1IE |= (BTN_1 | BTN_2 | BTN_0);
	return ret ;
}

void blockDelay(uint16_t delay)
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

inline void switchMode()
{
	Mmeter::nextMode();
	char nibble[4] = { ' ', ' ', ' ', ' ' };
	uint8_t i = Mmeter::getMode();
	nibble[i] = '[' ; nibble[i+1] = ']';

	i = 4 ; uint8_t x = 12 ;
	do
	{
		lcdSetPos(x, 1);
		lcdSendChr(nibble[i]);
		x -= 4;
	}while(--i);
}

inline void switchSize()
{
	Mmeter::nextCounts();
	lcdSetPos(14,1);
	uint8_t i = Mmeter::getCounts();
	if(i == 16)
		lcdSendStr("16");
	else
	{
		lcdSendChr(' ');
		lcdSendChr('0' + i);
	}
}

inline void switchRunning()
{
	// Mmeter::nextMode(); is it necessary?
}

inline void displayGui()
{
	char bottomText[17] = " MIN MAX AVG    " ;
	uint8_t i = Mmeter::getMode();
	bottomText[i*4] = '[';
	bottomText[(i*4)+4] = ']';

	i = Mmeter::getCounts();
	if(Mmeter::getCounts() == 16)
	{
		bottomText[15] = '1';
		bottomText[16] = '6';
	}
	else
	{
		bottomText[16] = '0' + i;
	}

	lcdSetPos(0, 1);
	lcdSendStr(bottomText);

	Mmeter::getStr(bottomText);
	lcdSetPos(0,0);
	lcdSendStr(bottomText);
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    initClock();
    lcdInit();
    adcInit();
    buttonsInit();
    initTimer();

    displayGui();

    char measurments[Mmeter::STR_LEN];
    ADC12CTL1 |= ADC12SC | ENC ; // start conversion
	while(1)
	{
		goSleep(); // After wake up, GIE is cleared

		do {
			if(programFlags & FLAG_ADC_COMPLETE)
			{
				programFlags &= FLAG_ADC_COMPLETE;
				_EINT();

				Mmeter::addMeasure(adcmem0, adcmem1);
				if(Mmeter::isReady())
					Mmeter::getStr(measurments);

				lcdSetPos(0, 0);
				lcdSendStr(measurments);

				ADC12CTL1 |= ADC12SC | ENC ; // start conversion
				_DINT();
			}

			if(programFlags & FLAG_DEBOUNCE)
			{
				programFlags &= ~FLAG_DEBOUNCE;
				_EINT();

				uint8_t buttons = scanButtons();

				void (* const functionArray[BTN_N])() =
				{
					switchMode,
					switchSize,
					switchRunning
				};

				if(!(buttons == 0xFF)) // No buttons or multiple
					functionArray[buttons]();

				_DINT();
			}
		} while(programFlags);
	}
}
