/*
 * Keypad.h
 *
 *  Created on: 10 mars 2018
 *      Author: ibraheemal-nuaimi
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_
#include "stm32f4xx_hal.h"




void keypad_init();
int8_t keypad_read();


#endif /* KEYPAD_H_ */
