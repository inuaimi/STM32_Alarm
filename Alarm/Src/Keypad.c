/*
 * Keypad.c
 *
 *  Created on: 10 mars 2018
 *      Author: ibraheemal-nuaimi
 */
#include "Keypad.h"
#include "stm32f4xx_hal.h"

//Private functions
void keypad_SetColumn(uint8_t column);
uint8_t keypad_CheckRow(uint8_t column);


uint8_t KEYPAD_Buttons[4][4] = {
		{0x01, 0x02, 0x03, 0x0A},
		{0x04, 0x05, 0x06, 0x0B},
		{0x07, 0x08, 0x09, 0x0C},
		{0x0E, 0x00, 0x0F, 0x0D},
};



void keypad_init(){

	  HAL_GPIO_WritePin(PAD_COLUM_1_GPIO_Port,PAD_COLUM_1_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(PAD_COLUM_2_GPIO_Port,PAD_COLUM_2_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(PAD_COLUM_3_GPIO_Port,PAD_COLUM_3_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(PAD_COLUM_4_GPIO_Port,PAD_COLUM_4_Pin,GPIO_PIN_SET);

}

int8_t keypad_read(){

	uint8_t button;
	static uint8_t keypressed = 0;



	keypad_SetColumn(1);
	button = keypad_CheckRow(1);
	if(button != 99 && keypressed == 0){
		keypressed = 1;
		return button;
	}

	keypad_SetColumn(2);
	button = keypad_CheckRow(2);
	if(button != 99 && keypressed == 0){
		keypressed = 1;
		return button;
	}


	keypad_SetColumn(3);
	button = keypad_CheckRow(3);
	if(button != 99 && keypressed == 0){
		keypressed = 1;
		return button;
	}


	keypad_SetColumn(4);
	button = keypad_CheckRow(4);
	if(button != 99 && keypressed == 0){
		keypressed = 1;
		return button;
	}

	keypressed = 0;
	return 99;


}


void keypad_SetColumn(uint8_t column){

	keypad_init();


	switch(column){
		 case 1 :
			 HAL_GPIO_WritePin(PAD_COLUM_1_GPIO_Port,PAD_COLUM_1_Pin,GPIO_PIN_RESET);
			 break;
		  case 2 :
			  HAL_GPIO_WritePin(PAD_COLUM_2_GPIO_Port,PAD_COLUM_2_Pin,GPIO_PIN_RESET);
			  break;
		  case 3 :
			  HAL_GPIO_WritePin(PAD_COLUM_3_GPIO_Port,PAD_COLUM_3_Pin,GPIO_PIN_RESET);
			 break;
		  case 4 :
			  HAL_GPIO_WritePin(PAD_COLUM_4_GPIO_Port,PAD_COLUM_4_Pin,GPIO_PIN_RESET);
			 break;
	  }


}




uint8_t keypad_CheckRow(uint8_t column){

	if(!HAL_GPIO_ReadPin(PAD_ROW_1_GPIO_Port,PAD_ROW_1_Pin))
		return KEYPAD_Buttons[0][column-1];
	if(!HAL_GPIO_ReadPin(PAD_ROW_2_GPIO_Port,PAD_ROW_2_Pin))
		return KEYPAD_Buttons[1][column-1];
	if(!HAL_GPIO_ReadPin(PAD_ROW_3_GPIO_Port,PAD_ROW_3_Pin))
		return KEYPAD_Buttons[2][column-1];
	if(!HAL_GPIO_ReadPin(PAD_ROW_4_GPIO_Port,PAD_ROW_4_Pin))
		return KEYPAD_Buttons[3][column-1];

	return 99;

}
