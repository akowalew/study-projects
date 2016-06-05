/*
 * usart.cpp
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */
#include "usart.h"

#pragma FUNC_ALWAYS_INLINE
 uint8_t usartIsCharAvailable()
{
	// return ~(FLAGS & EMPTY_FLAGS)
	P1OUT = 0xFF;
	return P1IN;
}

#pragma FUNC_ALWAYS_INLINE
uint8_t usartGetChar()
{
	P1OUT = 0xFF;
	return 0;
}

#pragma FUNC_ALWAYS_INLINE
void usartSendChr(const char data)
{
	// BLOCKING
	// wait for available buffer space
	// insert char
	P1OUT = 0xFF;
}

#pragma FUNC_ALWAYS_INLINE
void usartSendStr(const char * const data)
{
	// BLOCKING
	// wait for available buffer space
	// insert str
}
