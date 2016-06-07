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
#define CB_MODIFIED 0x04

typedef struct
{
	volatile uint8_t head;
	volatile uint8_t tail;
	volatile uint8_t flags;
	volatile uint8_t *buffer;
	uint8_t SZ;
} CBuffer ;

static inline void cbInit(volatile CBuffer *cbuf, volatile uint8_t *buff, uint8_t SZ)
{
	cbuf->flags = CB_EMPTY;
	cbuf->head = cbuf->tail = 0;
	cbuf->buffer = buff;
	cbuf->SZ = SZ-1;
}

static inline void cbPush(volatile CBuffer *cbuf, uint8_t data)
{
    cbuf->buffer[cbuf->head] = data;
    cbuf->head = (cbuf->head + 1) & (cbuf->SZ);

    cbuf->flags &= ~CB_EMPTY;
    if(cbuf->head==cbuf->tail)
    	cbuf->flags |= CB_FULL;
}


static inline uint8_t cbPop(volatile CBuffer *cbuf)
{
    uint8_t x = cbuf->buffer[cbuf->tail];
    cbuf->tail = (cbuf->tail + 1) & (cbuf->SZ);

    cbuf->flags &= ~CB_FULL;
    if(cbuf->tail == cbuf->head)
    	cbuf->flags |= CB_EMPTY;

    return x;
}

static inline uint8_t cbIsFull(volatile CBuffer *cbuf) { return (cbuf->flags & CB_FULL) ; }
static inline uint8_t cbIsEmpty(volatile CBuffer *cbuf) { return (cbuf->flags & CB_EMPTY) ; }
static inline uint8_t cbIsModified(volatile CBuffer *cbuf) { return (cbuf->flags & CB_MODIFIED) ; }
static inline void cbNotModified(volatile CBuffer *cbuf) { cbuf->flags &= ~CB_MODIFIED; }
static inline void cbModified(volatile CBuffer *cbuf) { cbuf->flags |= CB_MODIFIED; }

#endif /* CBUFFER_H_ */
