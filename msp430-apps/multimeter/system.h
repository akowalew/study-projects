/*
 * system.h
 *
 *  Created on: 15 cze 2016
 *      Author: dicker
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

void initAll();

extern volatile uint8_t programFlags;
extern volatile uint16_t adcmem0, adcmem1;
extern volatile uint8_t delayRemain;
extern volatile uint8_t debounceDelayRemain;

void buttonsChanged();

#endif /* SYSTEM_H_ */
