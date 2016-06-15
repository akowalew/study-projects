#ifndef LCD_H_
#define LCD_H_

#include "header.h"

#define LCD_PIN_E 0x01
#define LCD_PIN_RS 0x02

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

void lcdInit();
void lcdSendChr(char c);
void lcdSendStr(const char* s);
void lcdSendCmd(char command);

static inline void lcdSetPos(uint8_t x, uint8_t y)
{
	lcdSendCmd(y ? (0xC0 | x) : (0x80 | x));
}


#endif /* BUTTONS_H_ */
