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
#include "define.h"
#include "ADXL345.h"
#include "Delay.h"

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;

extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;


extern volatile uint32_t usTick, sTick,msTick;

sensor_status sensor_event = NONE;




void Alarm_status(){

	static Alarm_state current_state = Alarm_init;
	static TextLCDType lcd;
	static uint8_t set_Temp = 26;
	static uint8_t tmpCode[4] = {7,3,9,2};
	uint8_t tmp_state = 0;



	switch(current_state){

		case Alarm_init:
			init(&lcd);
			current_state = Alarm_idle;
			update_lcd(&lcd,LCD_Unlocked);
			ADXL345_init_interrupt();
			tmp_state = 0;
			break;
		case Alarm_idle:
			current_state = A_idle(&lcd,set_Temp, tmpCode);
			tmp_state = 0;
			break;
		case ALarm_arming:
			current_state = A_arming(&lcd,tmpCode);
			tmp_state = 1;
			break;

		case Alarm_armed:
			current_state = A_armed(&lcd, set_Temp,tmpCode);
			tmp_state = 2;
			break;

		case Alarm_PRE_Trigged:
			current_state = A_Pre_Trigged(&lcd,tmpCode);
			tmp_state = 3;
			break;

		case Alarm_Trigged:
			current_state = A_Trigged(&lcd);
			break;

		case Alarm_SetTemp:
			current_state = A_setTemp(&lcd,&set_Temp);
			break;

		case Alarm_SetGyro:
			current_state = A_setGyro(&lcd);
			break;

		case Alarm_setCode:
			current_state = A_setCode(&lcd,tmpCode);
			break;
	}

	update_uart(tmp_state,set_Temp);

	delay_ms(150);


}


void init(TextLCDType *lcd){


	HAL_TIM_Base_Start_IT(&htim10);
	HAL_TIM_Base_Start_IT(&htim11);
	HAL_TIM_Base_Start_IT(&htim9);

	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	TextLCD_Init(lcd,LCD_E_GPIO_Port, LCD_RS_Pin,LCD_RW_Pin, LCD_E_Pin ,LCD_D0_GPIO_Port ,
		  (LCD_D0_Pin | LCD_D1_Pin | LCD_D2_Pin | LCD_D3_Pin | LCD_D4_Pin | LCD_D5_Pin
				  | LCD_D6_Pin | LCD_D7_Pin ));
	keypad_init();
	set_Led(L_GREEN);
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
		else if(button == 0x0C && clicks==0)
			return Key_C;
		else if(button == 0x0D && clicks !=0){
			clicks--;
			return Key_D;
		}
		clicked_button[clicks] = button;
		clicks++;
		if(clicks==4){
			clicks=0;

			 if(clicked_button[0]==code[0] && clicked_button[1]==code[1] && clicked_button[2]==code[2] && clicked_button[3]==code[3])
				 return Key_OK;
			 else
				 return Key_Wrong;
		  }
		return Key_Pressed;



	  }

	return Key_No_Pressed;

}




Alarm_state A_idle(TextLCDType *lcd,uint8_t setTemp, uint8_t *code){

	key_code current_key_state;
	int16_t currentTemp;
	static uint8_t tmp_presses = 0;


	currentTemp = Read_Analog_Temp();
	current_key_state = Alarm_code_status(code);
	if(current_key_state == Key_OK){
		set_Led(L_YELLOW);
		update_lcd(lcd,LCD_Arming);
		tmp_presses = 0;
		return ALarm_arming;
	}
	else if(current_key_state == Key_Pressed){
		TextLCD_Position(lcd,20+tmp_presses,1);
		TextLCD_Putchar(lcd,'*');
		tmp_presses ++;

	}
	else if(current_key_state == Key_Wrong){
		lcd_clearRow(lcd,2);
		TextLCD_Printf(lcd,"Wrong Pin,Try Again");
		lcd_clearRow(lcd,3);
		tmp_presses = 0;
	}

	else if(current_key_state == Key_A){
		update_lcd(lcd,LCD_SetTemp);
		tmp_presses = 0;
		return Alarm_SetTemp;
	}
	else if(current_key_state == Key_B){
		update_lcd(lcd,LCD_SetGyro);
		tmp_presses = 0;
		return Alarm_SetGyro;
	}
	else if(current_key_state == Key_C){
		update_lcd(lcd,LCD_SetCode);
		tmp_presses = 0;
		return Alarm_setCode;
	}
	else if(current_key_state == Key_D && tmp_presses !=0){
		tmp_presses--;
		TextLCD_Position(lcd,20+tmp_presses,1);
		TextLCD_Putchar(lcd,' ');
		TextLCD_Position(lcd,20+tmp_presses,1);
	}
	lcd_clearRow(lcd,1);
	TextLCD_Printf(lcd,"Temp:%d SET:%d",currentTemp,setTemp);

	return Alarm_idle;
}

Alarm_state A_arming(TextLCDType *lcd,uint8_t *code){

	static uint8_t counter = 10;
	static uint32_t last_sTick = 0;
	static uint32_t current_sTick;
	key_code current_key_state;

	static uint8_t tmp_presses = 0;


	current_sTick = sTick;
	current_key_state = Alarm_code_status(code);

	if(current_key_state == Key_OK){
		HAL_Delay(150);
		set_Led(L_GREEN);
		update_lcd(lcd,LCD_Unlocked);
		tmp_presses = 0;
		counter = 10;
		HAL_GPIO_WritePin(ACTIVE_BUZZER_GPIO_Port,ACTIVE_BUZZER_Pin, GPIO_PIN_RESET);
		return Alarm_idle;
		}
	else if(current_key_state == Key_Pressed){
		TextLCD_Position(lcd,20+tmp_presses,1);
		TextLCD_Putchar(lcd,'*');
		tmp_presses++;
	}
	else if(current_key_state == Key_D && tmp_presses != 0){
		tmp_presses--;
		TextLCD_Position(lcd,20+tmp_presses,1);
		TextLCD_Putchar(lcd,' ');
	}
	else if(current_key_state == Key_Wrong){
		TextLCD_Position(lcd,20,0);
		TextLCD_Printf(lcd,"Wrong Pin, Try again");
		lcd_clearRow(lcd,3);
		tmp_presses = 0;
	}


	if(counter == 0 ){
		set_Led(L_RED);
		TextLCD_Clear(lcd);
		update_lcd(lcd,LCD_Locked);
		counter = 10;
		sensor_event = NONE;
		ADXL345_Clear();
		HAL_GPIO_WritePin(ACTIVE_BUZZER_GPIO_Port,ACTIVE_BUZZER_Pin, GPIO_PIN_RESET);
		return Alarm_armed;
	}
	else if(last_sTick != current_sTick){
		lcd_clearRow(lcd,0);
		TextLCD_Printf(lcd,"Arming:%d ", counter);
		counter--;
		last_sTick = current_sTick;
	}

	HAL_GPIO_TogglePin(ACTIVE_BUZZER_GPIO_Port,ACTIVE_BUZZER_Pin);


	return ALarm_arming;



}

Alarm_state A_armed(TextLCDType *lcd,uint8_t setTemp,uint8_t *code){


	key_code current_key_state;
	uint8_t sensorStatus;
	static uint8_t tmp_presses = 0;

	sensorStatus = check_sensors(setTemp);

	if(sensorStatus){
		update_lcd(lcd,LCD_PRI_Trigged);
		tmp_presses = 0;
		return Alarm_PRE_Trigged;
	}

	current_key_state = Alarm_code_status(code);


	if(current_key_state == Key_OK){
			set_Led(L_GREEN);
			TextLCD_Clear(lcd);
			update_lcd(lcd,LCD_Unlocked);
			tmp_presses = 0;
			return Alarm_idle;
		}
	else if(current_key_state == Key_Pressed){
		TextLCD_Position(lcd,20+tmp_presses,1);
		TextLCD_Putchar(lcd,'*');
		tmp_presses++;

	}
	else if(current_key_state == Key_Wrong){
		lcd_clearRow(lcd,2);
		TextLCD_Puts(lcd,"Wrong Pin, Try Again");
		lcd_clearRow(lcd,3);
		tmp_presses = 0;
	}
	else if(current_key_state == Key_D && tmp_presses != 0){
		tmp_presses--;
		TextLCD_Position(lcd,20+tmp_presses,1);
		TextLCD_Putchar(lcd,' ');
	}

	return Alarm_armed;



}

Alarm_state A_Pre_Trigged(TextLCDType *lcd,uint8_t *code){


	static uint8_t counter = 30;
	static uint32_t last_sTick = 0;
	key_code current_key_state;


	static uint8_t tmp_presses = 0;


	current_key_state = Alarm_code_status(code);

	if(current_key_state == Key_OK){
		set_Led(L_GREEN);
		update_lcd(lcd,LCD_Unlocked);
		counter = 30;
		tmp_presses = 0;
		HAL_GPIO_WritePin(ACTIVE_BUZZER_GPIO_Port,ACTIVE_BUZZER_Pin, GPIO_PIN_RESET);
		return Alarm_idle;
		}
	else if(current_key_state == Key_Pressed){
		TextLCD_Position(lcd,20+tmp_presses,1);
		TextLCD_Putchar(lcd,'*');
		tmp_presses++;
	}
	else if(current_key_state == Key_Wrong){
		TextLCD_Position(lcd,20,0);
		TextLCD_Printf(lcd,"Wrong Pin, Try again");
		lcd_clearRow(lcd,3);
		tmp_presses = 0;
	}
	else if(current_key_state == Key_D && tmp_presses !=0){
		tmp_presses--;
		TextLCD_Position(lcd, 20+tmp_presses,1);
		TextLCD_Putchar(lcd,' ');
	}


	if(counter == 0 ){
		set_Led(L_RED);
		TextLCD_Clear(lcd);
		update_lcd(lcd,LCD_Trigged);
		counter = 30;
		tmp_presses = 0;
		sensor_event = NONE;
		return Alarm_Trigged;
	}
	else if(last_sTick != sTick){
		TextLCD_Position(lcd,0,1);
		TextLCD_Printf(lcd,"Enter Pin In:%d ", counter);
		counter--;
		last_sTick = sTick;
		HAL_GPIO_TogglePin(ACTIVE_BUZZER_GPIO_Port,ACTIVE_BUZZER_Pin);
	}

	return Alarm_PRE_Trigged;
}

Alarm_state A_Trigged(TextLCDType *lcd){

	static key_code current_key_state;
	static uint8_t tmp_presses = 0;

	const uint8_t Master_Pin[4] = {6,5,1,0};

	current_key_state = Alarm_code_status(Master_Pin);
	if(current_key_state == Key_OK){
		set_Led(L_GREEN);
		update_lcd(lcd,LCD_Unlocked);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
		return Alarm_idle;
	}
	else if(current_key_state == Key_Pressed){
		TextLCD_Position(lcd,20+tmp_presses,1);
		TextLCD_Putchar(lcd,'*');
		tmp_presses++;
	}
	else if(current_key_state == Key_Wrong){
		TextLCD_Position(lcd,20,0);
		TextLCD_Printf(lcd,"Wrong Pin. Try Again");
		lcd_clearRow(lcd,3);
		tmp_presses = 0;
	}
	else if(current_key_state == Key_D){
		tmp_presses--;
		TextLCD_Position(lcd,20+tmp_presses,1);
		TextLCD_Putchar(lcd, ' ');
	}
	toggle_Passive_buzzer();
	return Alarm_Trigged;

}

Alarm_state A_setTemp(TextLCDType *lcd, uint8_t *setTemp){

	uint8_t button;
	static uint8_t tmp_set_Temp = 0;
	static uint8_t tmp_presses = 0;
	button = keypad_read();
	if(button != 99){
		if(button == 0x0A){
			*setTemp = tmp_set_Temp;
			tmp_set_Temp = 0;
			//tmp_set_Temp = 0;
			tmp_presses = 0;
			lcd_clearRow(lcd,3);
			HAL_Delay(50);
		}
		else if(button == 0x0B){
			set_Led(L_GREEN);
			TextLCD_Clear(lcd);
			update_lcd(lcd,LCD_Unlocked);
			//*setTemp = tmp_set_Temp;
			tmp_set_Temp = 0;
			tmp_presses = 0;
			return Alarm_idle;

		}
		else if(button <= 0x09){

			tmp_set_Temp *= 10;
			tmp_set_Temp += button;

			TextLCD_Position(lcd,20,1);
			TextLCD_Printf(lcd,"%d ",tmp_set_Temp);
			tmp_presses++;

		}
	}
	if(tmp_presses == 3){
		tmp_set_Temp = 0;
		tmp_presses = 0;
		lcd_clearRow(lcd,3);
		HAL_Delay(50);
	}

	lcd_clearRow(lcd,1);
	TextLCD_Printf(lcd,"Set Temp:%d", *setTemp);

	return Alarm_SetTemp;

}
Alarm_state A_setGyro(TextLCDType *lcd){

	uint8_t button;
	static uint8_t tmp_set_Gyro = 0;
	static uint8_t set_Gyro = 75;
	static uint8_t tmp_presses = 0;
	button = keypad_read();
	if(button != 99){
		if(button == 0x0A){
			//tmp_set_Temp = 0;
			tmp_presses = 0;
			lcd_clearRow(lcd,3);
			set_Gyro = tmp_set_Gyro;
			ADXL345_setThreshold(set_Gyro);
			HAL_Delay(50);
		}
		else if(button == 0x0B){
			set_Led(L_GREEN);
			TextLCD_Clear(lcd);
			update_lcd(lcd,LCD_Unlocked);
			tmp_presses = 0;
			return Alarm_idle;

		}
		else if(button <= 0x09){

			tmp_set_Gyro *= 10;
			tmp_set_Gyro += button;

			TextLCD_Position(lcd,20,1);
			TextLCD_Printf(lcd,"%d ",tmp_set_Gyro);
			tmp_presses++;

		}
	}
	if(tmp_presses == 4 || 255 < tmp_set_Gyro){
		tmp_set_Gyro = 0;
		tmp_presses = 0;
		lcd_clearRow(lcd,3);
	}

	lcd_clearRow(lcd,1);
	TextLCD_Printf(lcd,"Set Gyro:%d", set_Gyro);
	return Alarm_SetGyro;



}
Alarm_state A_setCode(TextLCDType *lcd,uint8_t *setCode){

	uint8_t button;
	static uint8_t tmp_set_code[4] = {0,0,0,0};
	static uint16_t tmp_code = 0;
	static uint8_t tmp_presses = 0;


	button = keypad_read();
	if(button != 99){
		if(button == 0x0A && tmp_presses == 4){
			//tmp_set_Temp = 0;
			tmp_presses = 0;
			tmp_code = 0;
			for(uint8_t i=0; i < 4; i++){
				setCode[i] = tmp_set_code[i];
			}
			lcd_clearRow(lcd,3);

		}
		else if(button == 0x0B){
			set_Led(L_GREEN);
			TextLCD_Clear(lcd);
			update_lcd(lcd,LCD_Unlocked);
			tmp_presses = 0;
			tmp_code = 0;
			return Alarm_idle;

		}
		else if(button <= 0x09){

			tmp_code *= 10;
			tmp_code += button;
			if(tmp_presses < 4){
				tmp_set_code[tmp_presses] = button;
			}
			TextLCD_Position(lcd,20,1);
			TextLCD_Printf(lcd,"%d ",tmp_code);
			tmp_presses++;

		}
	}
	if(tmp_presses == 5){
		tmp_presses = 0;
		tmp_code = 0;
		lcd_clearRow(lcd,3);
	}

	lcd_clearRow(lcd,1);
	TextLCD_Printf(lcd,"Set Code:");
	for(uint8_t i=0; i<4; i++){
		TextLCD_Printf(lcd,"%d",setCode[i]);
	}

	return Alarm_setCode;

}
void set_Led(Led_Color ld){

	switch(ld){
			case L_OFF:
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
				break;
			case L_RED:
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
				break;
			case L_GREEN:
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
				break;
			case L_YELLOW:
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
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
			TextLCD_Printf(lcd,"A-TEMP B-ADXL C-Code");
			TextLCD_Position(lcd,20,0);
			TextLCD_Printf(lcd,"Enter Pin to lock");
			TextLCD_Position(lcd,20,1);

			break;
		case LCD_Locked:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"Locked");
			TextLCD_Position(lcd,20,0);
			TextLCD_Printf(lcd,"Enter Pin to unlock");
			TextLCD_Position(lcd,20,1);
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
		case LCD_Arming:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"Arming:");
			TextLCD_Position(lcd,20,0);
			TextLCD_Printf(lcd,"Enter Pin To Unlock");
			break;
		case LCD_SetTemp:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"A - SET  B - Return");
			TextLCD_Position(lcd,20,0);
			TextLCD_Printf(lcd,"Enter A New Temp");
			break;
		case LCD_SetGyro:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"A - SET  B - Return");
			TextLCD_Position(lcd,20,0);
			TextLCD_Printf(lcd,"Enter A Value 0-255");
			break;
		case LCD_SetCode:
			TextLCD_Clear(lcd);
			TextLCD_Printf(lcd,"A - SET  B - Return");
			TextLCD_Position(lcd,20,0);
			TextLCD_Printf(lcd,"Enter A 4-digit Pin");
			break;
	}
}

void lcd_clearRow(TextLCDType *lcd,uint8_t row){

	uint8_t x=0, y = row % 2;

	if(row == 2 || row == 3)
		x=20;
	TextLCD_Position(lcd,x,y);
	for(uint8_t i=0; i<16; i++)
		TextLCD_Putchar(lcd,' ');

	TextLCD_Position(lcd,x,y);

}

uint8_t check_sensors(uint8_t setTemp){

	int16_t temp = Read_Analog_Temp();
	if(!((setTemp - 2) <= temp && temp <= (setTemp +2) ))
		return 1;

	if(sensor_event == Motion_Trigged){
		sensor_event = NONE;
		ADXL345_Clear();
		return 1;
	}

	return 0;


}
void toggle_Passive_buzzer(){


	static uint8_t tmp = 0;

	switch(tmp){

	case 0:
		 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,50000);
		break;
	case 1:
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,200000);
		break;
	case 2:
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,2000);
		break;
	}
	tmp++;

	if (3 <= tmp)
		tmp=0;

}

int16_t Read_Analog_Temp(){

	uint32_t channel_0 = 0;
	int16_t Temp = 0;
	float Temp_1= 0, Temp_2;

	float v_out = 0;
	float T_res = 0;



	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1,HAL_MAX_DELAY) == HAL_OK)
		channel_0 = HAL_ADC_GetValue(&hadc1);


	HAL_ADC_Stop(&hadc1);

	v_out = channel_0/1241.0f;

	T_res = RS*((5-v_out)/v_out);



	Temp_1 = logf(T_res/RS)/3450;
	Temp_2 = (1/298.15);

	Temp = (1.0f/(Temp_1 + Temp_2)) - 273.15;

	return Temp;



}

void update_uart(uint8_t state, uint8_t setTemp){


	int16_t temp = Read_Analog_Temp();

	uint8_t txt[20];

	uint8_t len = 0;

	len = sprintf(txt, "[A%02d]",state);
	HAL_UART_Transmit(&huart2,txt,len,100);
	HAL_Delay(1);


	len = sprintf(txt, "[T%02d]",temp);
	HAL_UART_Transmit(&huart2,txt,len,100);
	HAL_Delay(1);


	len = sprintf(txt, "[S%02d]",setTemp);
	HAL_UART_Transmit(&huart2,txt,len,100);
	HAL_Delay(1);



}

