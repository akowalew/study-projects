#ifndef LCD_H_
#define LCD_H_

#include "header.h"

#define LCD_DELAY_DCO 39

#define E 0x01
#define RS 0x02

#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#define LCD_ENTRY_MODE	0x06 //increment, no display shift
#define LCD_FUNCTION_SET 0x38
#define LCD_DISPLAY_ON 0x0E
#define LCD_DISPLAY_OFF 0x08

#define LCD_DATA_OUT	P2OUT
#define LCD_DATA_DIR	P2DIR

#define LCD_CTRL_OUT	P3OUT
#define LCD_CTRL_DIR	P3DIR

void displayInit();
void displayChar(char c);
void displayString(char* s);
void displayCommand(char command);
void displaySetPos(uint8_t x, uint8_t y);

#endif /* BUTTONS_H_ */
