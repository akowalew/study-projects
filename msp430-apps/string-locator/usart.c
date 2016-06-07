/*
 * usart.cpp
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */
#include "usart.h"

volatile uint8_t rxArray[USART_RX_BUFF_SZ];
volatile uint8_t txArray[USART_TX_BUFF_SZ];
volatile CBuffer rxBuff;
volatile CBuffer txBuff;
volatile uint8_t isTxWaiting = 0;
volatile uint8_t isRxWaiting = 0;

uint8_t usartIsCharAvailable()
{
	return (!cbufIsEmpty(&rxBuff));
}

inline uint8_t usartGetChar_b()
{
	uint8_t ret ;
	usartRxDint();
	if(cbufIsEmpty(&rxBuff))
	{
		isRxWaiting = 1;
		usartRxEint();
		while(isRxWaiting)
			goSleep();
		usartRxDint();
	}
	ret = cbufPop(&rxBuff);
	usartRxEint();
	return ret;
}

void usartSendChr(const char data)
{
	usartTxDint();
	if(cbufIsEmpty(&txBuff))
		IFG1 |= UTXIFG0; // force TX ISR on
	if(cbufIsFull(&txBuff))
	{
		isTxWaiting = 1;
		usartTxEint();
		while(isTxWaiting) {
			goSleep(); // wait for at least 1 char
		}
		usartTxDint();
	}
	cbufPush(&txBuff, data);
	usartTxEint();
}

inline void usartSendStr(const char * data)
{
	// BLOCKING
	// wait for available buffer space
	// insert str
	char c;
	usartTxDint();
	if(cbufIsEmpty(&txBuff))
		IFG1 |= UTXIFG0; // force TX ISR on
	while((c = *(data++)))
	{
		if(cbufIsFull(&txBuff))
		{
			isTxWaiting = 1;
			usartTxEint();
			while(isTxWaiting) {
				goSleep();
			}
			usartTxDint();
		}

		cbufPush(&txBuff, c);
	}
	usartTxEint();
}

#pragma vector=USART0TX_VECTOR
__interrupt void usartTxIsr()
{
	if(cbufIsEmpty(&txBuff))
		return;
	U0TXBUF = cbufPop(&txBuff);
	if(isTxWaiting)
	{
		isTxWaiting = 0;
		wakeUp();
	}
}

#pragma vector=USART0RX_VECTOR
__interrupt void usartRxIsr()
{
	if((U0RCTL & RXERR) || (cbufIsFull(&rxBuff)))
		guiFatalError("RX ERR!");
	else
	{
		cbufPush(&rxBuff, U0RXBUF);
		if(isRxWaiting)
		{
			isRxWaiting = 0;
			wakeUp();
		}
	}
}

void initUsart()
{
	cbufInit(&rxBuff, rxArray, USART_RX_BUFF_SZ);
	cbufInit(&txBuff, txArray, USART_TX_BUFF_SZ);

	U0CTL |= SWRST;

	U0CTL = (PENA | PEV | CHAR | SWRST ); // PARITY EVEN 8BIT
	U0TCTL = (SSEL1 | SSEL0); // SMCLK = XT2
	U0RCTL = (URXEIE) ;
	U0BR1 = 0;
	U0BR0 = 64; // clk = 7372800Hz -> baud = 115200
	U0MCTL = 0;
	P3SEL = 0x30;
	ME1 |= (UTXE0 | URXE0);

	U0CTL &= ~SWRST;

	IFG1 &= ~(UTXIFG0 | URXIFG0);
	IE1 |= (UTXIE0 | URXIE0);
}

inline void usartRxEint() { IE1 |= URXIE0; }
inline void usartRxDint() { IE1 &= ~URXIE0 ; }
inline void usartTxDint() { IE1 &= ~UTXIE0 ; }
inline void usartTxEint() { IE1 |= UTXIE0 ; }
