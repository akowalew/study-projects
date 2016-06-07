/*
 * cbuffer.h
 *
 *  Created on: 6 cze 2016
 *      Author: dicker
 */

#ifndef CBUFFER_H_
#define CBUFFER_H_

#include <stdint.h>

#define CB_EMPTY 0x01
#define CB_FULL 0x02
typedef struct
{
	volatile uint8_t head;
	volatile uint8_t tail;
	volatile uint8_t flags;
	volatile uint8_t *buffer;
	uint8_t SZ;
} CBuffer ;

static inline void cbufInit(volatile CBuffer *cbuf, volatile uint8_t *buff, uint8_t SZ)
{
	cbuf->flags = CB_EMPTY;
	cbuf->head = cbuf->tail = 0;
	cbuf->buffer = buff;
	cbuf->SZ = SZ-1;
}

static inline void cbufPush(volatile CBuffer *cbuf, uint8_t data)
{
    cbuf->buffer[cbuf->head] = data;
    cbuf->head = (cbuf->head + 1) & (cbuf->SZ);

    cbuf->flags &= ~CB_EMPTY;
    if(cbuf->head==cbuf->tail)
    	cbuf->flags |= CB_FULL;
}


static inline uint8_t cbufPop(volatile CBuffer *cbuf)
{
    uint8_t x = cbuf->buffer[cbuf->tail];
    cbuf->tail = (cbuf->tail + 1) & (cbuf->SZ);

    cbuf->flags &= ~CB_FULL;
    if(cbuf->tail == cbuf->head)
    	cbuf->flags |= CB_EMPTY;

    return x;
}

static inline uint8_t cbufIsFull(volatile CBuffer *cbuf) { return (cbuf->flags & CB_FULL) ; }
static inline uint8_t cbufIsEmpty(volatile CBuffer *cbuf) { return cbuf->flags & CB_EMPTY ; }

/*void cbufInit(volatile CBuffer *cbuf, volatile uint8_t *buff, uint8_t SZ);
void cbufPush(volatile CBuffer *cbuf, uint8_t c);
uint8_t cbufPop(volatile CBuffer *cbuf);
uint8_t cbufIsEmpty(volatile CBuffer *cbuf);
uint8_t cbufIsFull(volatile CBuffer *cbuf);*/

#endif /* CBUFFER_H_ */
