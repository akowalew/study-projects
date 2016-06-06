/*
 * usart.h
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */

#ifndef USART_H_
#define USART_H_

#include "header.h"
#include "vtGui.h"
#include "cbuffer.h"

uint8_t usartIsCharAvailable();
uint8_t usartGetChar();
void usartSendChr(const char data);
void usartSendStr(const char * const data);
void initUsart();

void usartRxEint();
void usartRxDint();
void usartTxEint();
void usartTxDint();

#define USART_RX_BUFF_SZ 64
#define USART_TX_BUFF_SZ 64

#endif /* USART_H_ */
