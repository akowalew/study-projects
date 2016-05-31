/*
 * usartBuffers.h
 *
 *  Created on: May 31, 2016
 *      Author: student
 */

#ifndef USARTBUFFERS_H_
#define USARTBUFFERS_H_

#include <msp430.h>
#include <stdint.h>
#include <signal.h>

#define USART_TX_BUFF_SIZE	(32)
#define USART_RX_BUFF_SIZE	(16)

#define USART_BUFFER_EMPTY	0x01
#define USART_BUFFER_FULL	0x02

#define USART_TX 0
#define USART_RX 1

typedef struct
{
	uint8_t rPos, wPos;
	const uint8_t buffSz;
	uint8_t *pBuffer;
	uint8_t flags;
} CycleBuffer ;

void usartBuffersInit();

void usartPush(uint8_t bufferID, uint8_t data);
uint8_t usartPop(uint8_t bufferID);

uint8_t usartStateRx();
uint8_t usartStateTx();

void initUsart();

#endif /* USARTBUFFERS_H_ */
