/*
 * header.h
 *
 *  Created on: 4 cze 2016
 *      Author: dicker
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <msp430.h>
#include <stdint.h>
#include <signal.h>

#include "virtualTerminal.h"

typedef void (*F_PTR)(void);
#define MODE_KEYS_N 3

#define goSleep() (P1OUT = 0xFF)
#define wakeUp() ()

#endif /* HEADER_H_ */
