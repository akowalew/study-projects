#include "virtualTerminal.h"

const char* const VT_CLR_LN = "\x1b[2K";
const char* const VT_CLR_SCR = "\x1b[2J";
const char* const VT_RESET = "\x1b\x63";
const char* const VT_CLR_CHR = "\b \b";

const char* const VT_HOME =	"\x1b[H";
const char* const VT_CURSOR_OFF = "\x1b[?25l";
const char* const VT_CURSOR_ON = "\x1b[?25h";
const char* const VT_CURSOR_SAVE = "\x1b[s";
const char* const VT_CURSOR_RESTORE = "\x1b[u";
const char* const VT_CURSOR_UP =	"\x1b[1A";
const char* const VT_CURSOR_DOWN =	"\x1b[1B";
const char* const VT_CURSOR_RIGHT =	"\x1b[1C";
const char* const VT_CURSOR_LEFT =	"\x1b[1D";

const char* const VT_SET_BOLD =	"\x1b[1m";
const char* const VT_RESET_ATTRS = "\x1b[0m";
const char* const VT_SET_GREEN = "\x1b[32m";
const char* const VT_SET_RED_FG  ="\x1b[31m";
const char* const VT_SET_CYAN_FG = "\x1b[36m";

inline void simpleItoaU8(uint8_t src, char *dest)
{
	if(src > 199) // 255..200
	{
		src -= 200;
		*(dest++) = '2';
	} else if(src > 99) // 199..100
	{
		src -= 100;
		*(dest++) = '1';
	}

	if(src > 9) // 99..10
	{
		uint8_t tmp = src;
		src /= 10;
		*(dest++) = '0' + (tmp - (src*10));
	}

	*(dest++) = '0' + src;
}

void vtSetCursor(uint8_t x, uint8_t y)
{ // format : [ <x> ; <y> H
	char str[4];
	usartSendStr("\x1b[");
	simpleItoaU8(x, str);
	usartSendStr(str);
	usartSendChr(';');
	simpleItoaU8(y, str);
	usartSendStr(str);
	usartSendChr('H');
}
