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

uint8_t usartGetChar_b();
void usartSendChr(const char data);
void usartSendStr(const char * const data);
void initUsart();

#define USART_RX_BUFF_SZ 32
#define USART_TX_BUFF_SZ 64

static inline void usartRxEint() { IE1 |= URXIE0; }
static inline void usartRxDint() { IE1 &= ~URXIE0 ; }
static inline void usartTxDint() { IE1 &= ~UTXIE0 ; }
static inline void usartTxEint() { IE1 |= UTXIE0 ; }

#endif /* USART_H_ */
