/*
 * usartBuffers.c
 *
 *  Created on: May 31, 2016
 *      Author: student
 */

#include "usartBuffers.h"
#include <stdint.h>

uint8_t rxArray[USART_RX_BUFF_SIZE];
uint8_t txArray[USART_TX_BUFF_SIZE];

CycleBuffer rxBuffer = {0, 0, USART_RX_BUFF_SIZE, rxArray, USART_BUFFER_EMPTY};
CycleBuffer txBuffer = {0, 0, USART_TX_BUFF_SIZE, txArray, USART_BUFFER_EMPTY};

volatile uint8_t isDataSending;
volatile uint8_t isDataWaiting;

void usartPush(uint8_t bufferID, uint8_t data)
{
	CycleBuffer* ptr = (bufferID == USART_RX ? &rxBuffer : &txBuffer);

	if(ptr->flags & USART_BUFFER_FULL)
		return;
	else
	{
		ptr->pBuffer[ptr->wPos] = data;
		ptr->wPos = (ptr->wPos + 1) & (USART_RX_BUFF_SIZE-1);
		if(ptr->wPos == ptr->rPos)
			ptr->flags |= USART_BUFFER_FULL;
		ptr->flags &= ~USART_BUFFER_EMPTY;

		if((bufferID == USART_TX) && (U0TCTL & TXEPT))
			IFG1 |= UTXIFG0;
	}
}

uint8_t usartPop(uint8_t bufferID)
{
	CycleBuffer* ptr = (bufferID == USART_RX ? &rxBuffer : &txBuffer);

	if(ptr->flags & USART_BUFFER_EMPTY)
		return 1;
	else
	{
		uint8_t ret = ptr->pBuffer[ptr->rPos];
		ptr->rPos = (ptr->rPos + 1) & (USART_RX_BUFF_SIZE-1);
		if(ptr->rPos == ptr->wPos)
			ptr->flags |= USART_BUFFER_EMPTY ;
		ptr->flags &= ~USART_BUFFER_FULL;
		return ret;
	}
}

void initUsart()
{
	U0CTL = (PENA | PEV | CHAR ); // PARITY EVEN 8BIT
	U0TCTL = (SSEL1 | SSEL0); // SMCLK = XT2
	U0RCTL = (URXEIE) ;
	U0BR1 = 0;
	U0BR0 = 64; // clk = 7372800Hz -> baud = 115200
	U0MCTL = 0;
	P3SEL = 0x30;
	ME1 |= (UTXE0 | URXE0);
	IE1 |= (UTXIE0 | URXIE0);
}

#pragma vector=USART0TX_VECTOR
__interrupt void usart0TxInt(void)
{

}

#pragma vector=USART0RX_VECTOR
__interrupt void usart0RxInt(void)
{
	if(usartStateRx() & USART_BUFFER_FULL)
	{
		P1OUT |= 0x01;
	}
	else
	{
		usartPush(USART_RX, U0RXBUF);
		if(isDataWaiting == 0)
			isDataWaiting = 1;
	}
}


inline uint8_t usartStateRx()
{
	return rxBuffer.flags;
}

inline uint8_t usartStateTx()
{
	return txBuffer.flags;
}
