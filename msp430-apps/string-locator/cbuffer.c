/*
 * cbuffer.c
 *
 *  Created on: 6 cze 2016
 *      Author: dicker
 */

#ifndef CBUFFER_C_
#define CBUFFER_C_

#include "cbuffer.h"

void cbufInit(volatile CBuffer *cbuf, volatile uint8_t *buff, uint8_t SZ)
{
	cbuf->flags = CB_EMPTY;
	cbuf->head = cbuf->tail = 0;
	cbuf->buffer = buff;
	cbuf->SZ = SZ;
}

inline void cbufPush(volatile CBuffer *cbuf, uint8_t data)
{
    cbuf->buffer[cbuf->head] = data;
    cbuf->head = (cbuf->head + 1) & (cbuf->SZ-1);

    if(cbuf->head==cbuf->tail)
    	cbuf->flags |= CB_FULL;
    else
    	cbuf->flags &= ~CB_FULL;
}


inline uint8_t cbufPop(volatile CBuffer *cbuf)
{
    uint8_t x = cbuf->buffer[cbuf->tail];
    cbuf->tail = (cbuf->tail + 1) & (cbuf->SZ-1);

    if(cbuf->tail == cbuf->head)
    	cbuf->flags |= CB_EMPTY;
    else
    	cbuf->flags &= ~CB_EMPTY;

    return x;
}


inline uint8_t cbufIsFull(volatile CBuffer *cbuf)
{
	return cbuf->flags & CB_FULL ;
}


inline uint8_t cbufIsEmpty(volatile CBuffer *cbuf)
{
	return cbuf->flags & CB_EMPTY ;
}

#endif /* CBUFFER_C_ */
