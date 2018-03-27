/*
 * Delay.h
 *
 *  Created on: 7 feb. 2017
 *      Author: scma
 */
#include <stm32f4xx_hal.h>

#ifndef DELAY_H_
#define DELAY_H_



void delay_s(uint32_t s);

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif /* DELAY_H_ */
