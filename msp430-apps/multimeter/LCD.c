#include "LCD.h"

/*
 * Sample display view:
 *
 * ******************
 * *3.142=3.100     *
 * * MIN MAX[AVG] 16*
 * ******************
 */

void lcdInit(){
	LCD_DATA_DIR = 0xFF;
	LCD_CTRL_DIR = 0xFF;
	
	blockDelay(800);
	lcdSendCmd(LCD_FUNCTION_SET);

	blockDelay(84);
	lcdSendCmd(LCD_FUNCTION_SET);

	blockDelay(3);
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
	blockDelay(5);
}

void lcdSendStr(const char* s){
	LCD_CTRL_OUT = LCD_PIN_RS;
	while(*s){
		LCD_DATA_OUT = *s;
		LCD_CTRL_OUT |=LCD_PIN_E; LCD_CTRL_OUT &=~LCD_PIN_E;
		blockDelay(5);
		s++;
	}
}

inline void lcdSendCmd(char command)
{
	LCD_CTRL_OUT &= ~LCD_PIN_RS;
	LCD_DATA_OUT = command;
	LCD_CTRL_OUT |=LCD_PIN_E; LCD_CTRL_OUT &=~LCD_PIN_E;
	blockDelay(5);
}

void lcdSetPos(uint8_t x, uint8_t y)
{
	lcdSendCmd(y ? (0xC0 | x) : (0x80 | x));
}
