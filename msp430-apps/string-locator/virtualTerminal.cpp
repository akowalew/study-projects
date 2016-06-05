#include "virtualTerminal.h"

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
