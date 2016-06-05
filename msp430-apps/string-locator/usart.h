/*
 * usart.h
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */

#ifndef USART_H_
#define USART_H_

#include "header.h"

uint8_t usartIsCharAvailable();
uint8_t usartGetChar();
void usartSendChr(const char data);
void usartSendStr(const char * const data);

#endif /* USART_H_ */
