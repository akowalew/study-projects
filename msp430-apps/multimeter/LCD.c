#include "LCD.h"

volatile uint16_t i;
#pragma vector=TIMERA0_VECTOR
__interrupt void timerA0_ISR()
{
	if(--i)
		TACCR0 += LCD_DELAY_DCO;
	else
	{
		TACTL = TASSEL1 | TACLR;
		wakeUp();
	}
}

inline void lcdDelay(uint16_t delay)
{
	i = delay;
	TACCR0 = LCD_DELAY_DCO;
	_DINT();
	TACTL |= MC1;
	while(1)
	{
		goSleep();
		_DINT();
		if(i == 0)
			break;
	}
	_EINT();
}

void displayInit(){
	TACTL |= (TASSEL1 | TACLR);
	TACCTL0 = CCIE;
	TACCR0 = LCD_DELAY_DCO;

	LCD_DATA_DIR = 0xFF;
	LCD_CTRL_DIR = 0xFF;
	
	lcdDelay(800);
	displayCommand(LCD_FUNCTION_SET);

	lcdDelay(84);
	displayCommand(LCD_FUNCTION_SET);

	lcdDelay(3);
	displayCommand(LCD_FUNCTION_SET);

	displayCommand(LCD_FUNCTION_SET);
	displayCommand(LCD_DISPLAY_OFF);
	displayCommand(LCD_CLEAR);
	displayCommand(LCD_ENTRY_MODE);
	displayCommand(LCD_DISPLAY_ON);
}

void displayChar(char c){
	LCD_CTRL_OUT = RS | E;
	LCD_DATA_OUT = c;
	LCD_CTRL_OUT &= ~E;
	lcdDelay(20);
}

void displayString(char* s){
	LCD_CTRL_OUT = RS;
	while(*s){
		LCD_DATA_OUT = *s;
		LCD_CTRL_OUT |=E; LCD_CTRL_OUT &=~E;
		lcdDelay(20);
		s++;
	}
}

inline void displayCommand(char command)
{
	LCD_CTRL_OUT &= ~RS;
	LCD_DATA_OUT = command;
	LCD_CTRL_OUT |=E; LCD_CTRL_OUT &=~E;
	lcdDelay(20);
}

inline void displaySetPos(uint8_t x, uint8_t y)
{
	displayCommand(y ? (0xC0 | x) : (0x80 | x));
}
