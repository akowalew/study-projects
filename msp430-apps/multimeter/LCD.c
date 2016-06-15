#include "LCD.h"

void lcdInit(){
	LCD_DATA_DIR = 0xFF;
	LCD_CTRL_DIR = 0xFF;
	
	waitDelay(800);
	lcdSendCmd(LCD_FUNCTION_SET);

	waitDelay(84);
	lcdSendCmd(LCD_FUNCTION_SET);

	waitDelay(3);
	lcdSendCmd(LCD_FUNCTION_SET);

	lcdSendCmd(LCD_FUNCTION_SET);
	lcdSendCmd(LCD_DISPLAY_OFF);
	lcdSendCmd(LCD_CLEAR);
	lcdSendCmd(LCD_ENTRY_MODE);
	lcdSendCmd(LCD_DISPLAY_ON);
}

void lcdSendChr(char c){
	LCD_CTRL_OUT = LCD_PIN_RS | LCD_PIN_E;
	LCD_DATA_OUT = c;
	LCD_CTRL_OUT &= ~LCD_PIN_E;
	waitDelay(5);
}

void lcdSendStr(const char* s){
	LCD_CTRL_OUT = LCD_PIN_RS;
	while(*s){
		LCD_DATA_OUT = *s;
		LCD_CTRL_OUT |=LCD_PIN_E; LCD_CTRL_OUT &=~LCD_PIN_E;
		waitDelay(5);
		s++;
	}
}

void lcdSendCmd(char command)
{
	LCD_CTRL_OUT &= ~LCD_PIN_RS;
	LCD_DATA_OUT = command;
	LCD_CTRL_OUT |=LCD_PIN_E; LCD_CTRL_OUT &=~LCD_PIN_E;
	waitDelay(5);
}
