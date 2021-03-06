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

uint8_t usartGetChar_b()
{
	uint8_t ret ;
	usartRxDint();
	if(cbIsEmpty(&rxBuff))
	{
		cbNotModified(&rxBuff);
		while(1) {
			_DINT();
			usartRxEint();
			goSleep(); // wait for at least 1 char
			usartRxDint();
			if(cbIsModified(&rxBuff))
				break;
		}
	}
	ret = cbPop(&rxBuff);
	usartRxEint();
	return ret;
}

void usartSendChr(const char data)
{
	usartTxDint();
	if(cbIsEmpty(&txBuff))
		IFG1 |= UTXIFG0; // force TX ISR on
	if(cbIsFull(&txBuff))
	{
		cbNotModified(&txBuff);
		while(1) {
			_DINT();
			usartTxEint();
			goSleep(); // wait for at least 1 char
			usartTxDint();
			if(cbIsModified(&txBuff))
				break;
		}
	}
	cbPush(&txBuff, data);
	usartTxEint();
}

void usartSendStr(const char * data)
{
	// BLOCKING
	// wait for available buffer space
	char c;
	usartTxDint();
	if(cbIsEmpty(&txBuff))
		IFG1 |= UTXIFG0; // force TX ISR on
	while((c = *(data++)))
	{
		if(cbIsFull(&txBuff))
		{
			cbNotModified(&txBuff);
			while(1) {
				_DINT();
				usartTxEint();
				goSleep(); // wait for at least 1 char
				usartTxDint();
				if(cbIsModified(&txBuff))
					break;
			}
		}
		cbPush(&txBuff, c);
	}
	usartTxEint();
}

volatile uint8_t errorsToSend = 0;
#pragma vector=USART0TX_VECTOR
__interrupt void usartTxIsr()
{
	if(cbIsEmpty(&txBuff))
	{
		if(errorsToSend > 0)
		{
			U0TXBUF = '\a';
			--errorsToSend;
		}
		return;
	}

	U0TXBUF = cbPop(&txBuff);
	if(!cbIsModified(&txBuff))
	{
		cbModified(&txBuff);
		wakeUp();
	}
}

#pragma vector=USART0RX_VECTOR
__interrupt void usartRxIsr()
{
	uint8_t tmp = U0RXBUF; // should it working ??? below
	if((U0RCTL & RXERR) || (cbIsFull(&rxBuff)))
	{
		if(cbIsEmpty(&txBuff))
			IFG1 |= UTXIFG0;
		errorsToSend = 3;
	}
	else
	{
		cbPush(&rxBuff, U0RXBUF);
		if(!cbIsModified(&rxBuff))
		{
			cbModified(&rxBuff);
			wakeUp();
		}
	}
}

void initUsart()
{
	cbInit(&rxBuff, rxArray, USART_RX_BUFF_SZ);
	cbInit(&txBuff, txArray, USART_TX_BUFF_SZ);

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
