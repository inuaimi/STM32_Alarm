/*
 * Alarm.c
 *
 *  Created on: 7 mars 2018
 *      Author: ibraheemal-nuaimi
 */
#include "Alarm.h"
#include "stm32f4xx_hal.h"
#include "Keypad.h"
#include "LCD.h"
#include "math.h"

/* External variables --------------------------------------------------------*/
/*
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern volatile uint32_t usTick, sTick;

sensor_status sensor_event = NONE;

void init(TextLCDType *lcd);
uint8_t compareArray(uint8_t *a, uint8_t *b,uint8_t len);
void set_Led(Led_Color ld);
void update_lcd(TextLCDType *lcd,LCD_Status tmpS);

void Alarm_status(){

	static Alarm_state current_state = Alarm_init;
	static TextLCDType lcd;
	uint8_t button = 0;
	static uint8_t set_Temp;
	static uint8_t tmp_set_Temp;



	static uint8_t counter;
	static uint32_t last_sTick, current_sTick;
	static key_code current_key_state;
	static uint8_t tmp_presses = 0;

	const uint8_t Pin[4] = {7,3,9,2};
	const uint8_t Master_Pin[4] = {6,5,1,0};



	switch(current_state){

		case Alarm_init:
			init(&lcd);
			current_state = Alarm_idle;
			update_lcd(&lcd,LCD_Unlocked);
			break;

		case Alarm_idle:
			current_key_state = Alarm_code_status(Pin);
			tmp_presses = 0;
			if(current_key_state == Key_OK){
				last_sTick = sTick;
				current_state = ALarm_arming;
				set_Led(L_YELLOW);
				update_lcd(&lcd,LCD_Arming);
				counter = 30;

			}
			else if(current_key_state == Key_Pressed)
				TextLCD_Putchar(&lcd,'*');
			else if(current_key_state == Key_Wrong)
				update_lcd(&lcd,LCD_Wrong);
			/*else if(current_key_state == Key_A){
				update_lcd(&lcd,LCD_SetTemp);
				current_state = Alarm_SetTemp;
			}*/
/*
			HAL_Delay(150);
			break;

		case ALarm_arming:
			current_sTick = sTick;
			current_key_state = Alarm_code_status(Pin);

			if(current_key_state == Key_OK){
				HAL_Delay(150);
				set_Led(L_GREEN);
				current_state = Alarm_idle;
				update_lcd(&lcd,LCD_Unlocked);
				}
			else if(current_key_state == Key_Pressed){
				TextLCD_Position(&lcd,20+tmp_presses,0);
				TextLCD_Putchar(&lcd,'*');
				tmp_presses++;
			}
			else if(current_key_state == Key_Wrong){
				TextLCD_Position(&lcd,20,0);
				TextLCD_Printf(&lcd,"    ");
				tmp_presses = 0;
			}


			if(counter == 0 ){
				set_Led(L_RED);
				current_state = Alarm_armed;
				TextLCD_Clear(&lcd);
				update_lcd(&lcd,LCD_Locked);
				counter = 20;
				sensor_event = NONE;
			}
			else if(last_sTick != current_sTick){
				TextLCD_Position(&lcd,0,0);
				TextLCD_Printf(&lcd,"Arming:%d ", counter);
				counter--;
				last_sTick = current_sTick;
			}

		  	HAL_Delay(150);
			break;

		case Alarm_armed:
			current_key_state = Alarm_code_status(Master_Pin);
			if(current_key_state == Key_OK){
					set_Led(L_GREEN);
					current_state = Alarm_idle;
					TextLCD_Clear(&lcd);
					update_lcd(&lcd,LCD_Unlocked);
				}
			else if(current_key_state == Key_Pressed)
				TextLCD_Putchar(&lcd,'*');
			else if(current_key_state == Key_Wrong)
				update_lcd(&lcd,LCD_Wrong);

			if(sensor_event == Motion_Trigged){
				update_lcd(&lcd,LCD_PRI_Trigged);
				current_state = Alarm_PRE_Trigged;
			}



		  	HAL_Delay(150);

			break;
		case Alarm_PRE_Trigged:

			current_sTick = sTick;
			current_key_state = Alarm_code_status(Pin);

			if(current_key_state == Key_OK){
				set_Led(L_GREEN);
				current_state = Alarm_idle;
				update_lcd(&lcd,LCD_Unlocked);
				}
			else if(current_key_state == Key_Pressed){
				TextLCD_Position(&lcd,20+tmp_presses,0);
				TextLCD_Putchar(&lcd,'*');
				tmp_presses++;
			}
			else if(current_key_state == Key_Wrong){
				TextLCD_Position(&lcd,20,0);
				TextLCD_Printf(&lcd,"    ");
				tmp_presses = 0;
			}


			if(counter == 0 ){
				set_Led(L_RED);
				current_state = Alarm_Trigged;
				TextLCD_Clear(&lcd);
				update_lcd(&lcd,LCD_Trigged);
				counter = 30;
				sensor_event = NONE;
			}
			else if(last_sTick != current_sTick){
				TextLCD_Position(&lcd,0,1);
				TextLCD_Printf(&lcd,"Enter Pin In:%d ", counter);
				counter--;
				last_sTick = current_sTick;
			}

			HAL_Delay(150);
			break;

		case Alarm_Trigged:
			current_key_state = Alarm_code_status(Master_Pin);
			if(current_key_state == Key_OK){
				set_Led(L_GREEN);
				current_state = Alarm_idle;
				update_lcd(&lcd,LCD_Unlocked);
			}
			else if(current_key_state == Key_Pressed){
				TextLCD_Position(&lcd,20+tmp_presses,0);
				TextLCD_Putchar(&lcd,'*');
				tmp_presses++;
			}
			else if(current_key_state == Key_Wrong){
				TextLCD_Position(&lcd,20,0);
				TextLCD_Printf(&lcd,"    ");
				tmp_presses = 0;
			}
			HAL_Delay(150);
			break;

		case Alarm_SetTemp:
			button = keypad_read();
			if(button != 99){
				if(button == 0x0A){
					set_Led(L_GREEN);
					current_state = Alarm_idle;
					TextLCD_Clear(&lcd);
					update_lcd(&lcd,LCD_Unlocked);
				}
				else{ //if(button <= 0x00 && 0x09 <= button){

					tmp_set_Temp *= 10;
					tmp_set_Temp += button;

					TextLCD_Position(&lcd,20,1);
					TextLCD_Printf(&lcd,"%d ",tmp_set_Temp);
					tmp_presses++;
					HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				}

			}
			HAL_Delay(150);
			break;




	}




}



key_code Alarm_code_status(const uint8_t *code){

	static uint8_t clicks = 0;
	static uint8_t clicked_button[4];
	uint8_t button;

	button = keypad_read();
	if(button !=99){
		if(button == 0x0A && clicks==0)
			return Key_A;
		else if(button == 0x0B && clicks==0)
			return Key_B;
		clicked_button[clicks] = button;
		clicks++;
		if(clicks==4){
			clicks=0;

			 if(clicked_button[0]==code[0] && clicked_button[1]==code[1] && clicked_button[2]==code[2] && clicked_button[3]==code[3])
				 return Key_OK;
			 else
				 return Key_Wrong;
		  }
		//HAL_Delay(100);

		return Key_Pressed;



	  }

	return Key_No_Pressed;

}


void init(TextLCDType *lcd){


	HAL_TIM_Base_Start_IT(&htim10);
	HAL_TIM_Base_Start_IT(&htim11);

	TextLCD_Init(lcd,LCD_E_GPIO_Port, LCD_RS_Pin,LCD_RW_Pin, LCD_E_Pin ,LCD_D0_GPIO_Port ,
		  (LCD_D0_Pin | LCD_D1_Pin | LCD_D2_Pin | LCD_D3_Pin | LCD_D4_Pin | LCD_D5_Pin
				  | LCD_D6_Pin | LCD_D7_Pin ));
	keypad_init();
	set_Led(L_GREEN);

}



void set_Led(Led_Color ld){

	switch(ld){
			case L_OFF:
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
				break;
			case L_RED:
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
				break;
			case L_GREEN:
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
				break;
			case L_YELLOW:
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
				break;

	}
}


void update_lcd(TextLCDType *lcd,LCD_Status tmpS){


	switch(tmpS){
		case LCD_NONE:
			TextLCD_Clear(lcd);
			break;
		case LCD_Unlocked:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"A - Temp ");
			//TextLCD_Position(lcd,0,1);
			TextLCD_Printf(lcd," B - Gyro");
			TextLCD_Position(lcd,20,0);
			TextLCD_Printf(lcd,"Enter Pin to lock");
			TextLCD_Position(lcd,20,1);


			//TextLCD_Position(&lcd,60,0);
			//TextLCD_Printf(lcd,"Enter Pin to lock");

			break;
		case LCD_Locked:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"Locked");
			TextLCD_Position(lcd,0,1);
			TextLCD_Printf(lcd,"Enter Pin to unlock");
			TextLCD_Position(lcd,20,0);
			break;


		case LCD_PRI_Trigged:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"Larm Trigged");
			TextLCD_Position(lcd,0,1);
			TextLCD_Printf(lcd,"Enter Pin In:");
			TextLCD_Position(lcd,20,0);
			break;
		case LCD_Trigged:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"Larm Trigged");
			TextLCD_Position(lcd,0,1);
			TextLCD_Printf(lcd,"Enter Master Pin");
			TextLCD_Position(lcd,20,0);
			break;
		case LCD_Wrong:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"Unlocked");
			TextLCD_Position(lcd,0,1);
			TextLCD_Printf(lcd,"Wrong Pin");
			TextLCD_Position(lcd,20,0);
			TextLCD_Printf(lcd,"Try again");
			TextLCD_Position(lcd,20,1);
			break;
		case LCD_Arming:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"Arming:");
			TextLCD_Position(lcd,0,1);
			TextLCD_Printf(lcd,"Enter Pin To Unlock");
			break;
		case LCD_SetTemp:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"A - SET  B - Return");
			TextLCD_Position(lcd,0,1);
			TextLCD_Printf(lcd,"Enter A New Temp");
			break;
	}



}
*/
