/*
 * Delay.c
 *
 *  Created on: 7 feb. 2017
 *      Author: scma
 */

#include "Delay.h"
#include "stm32f4xx_it.h"

extern volatile uint32_t usTick, sTick, msTick;
//extern volatile uint32_t sTick;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;




void delay_s(uint32_t s){ //delay för sekunder


	uint32_t end = sTick + s;
	while(sTick < end);




}




void delay_us(uint32_t us){
	uint32_t end = usTick + us;
	while(usTick < end);
	//HAL_TIM_Base_Stop_IT(&htim11);
}

void delay_ms(uint32_t ms){

	uint32_t end = msTick + ms;
	while(msTick < end);


}
