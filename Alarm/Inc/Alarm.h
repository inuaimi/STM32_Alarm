/*
 * Alarm.h
 *
 *  Created on: 7 mars 2018
 *      Author: ibraheemal-nuaimi
 */

#ifndef ALARM_H_
#define ALARM_H_
#include "stm32f4xx_hal.h"
#include "LCD.h"



typedef enum{Alarm_init, Alarm_idle, ALarm_arming,
	Alarm_armed,Alarm_PRE_Trigged,Alarm_Trigged, Alarm_SetTemp,Alarm_SetGyro} Alarm_state;

typedef enum{Key_No_Pressed, Key_Pressed,Key_OK,Key_Wrong, Key_A, Key_B} key_code;

typedef enum{NONE, Motion_Trigged,Sound_Trigged,Diamond_Trigged} sensor_status;

typedef enum{L_OFF, L_RED,L_YELLOW,L_GREEN} Led_Color;

typedef enum{LCD_NONE, LCD_Unlocked, LCD_Locked,LCD_Arming,LCD_PRI_Trigged,LCD_Trigged, LCD_SetTemp} LCD_Status;


void Alarm_status();
Alarm_state A_idle(TextLCDType *lcd);
Alarm_state A_arming(TextLCDType *lcd);
Alarm_state A_armed(TextLCDType *lcd);

Alarm_state A_Pre_Trigged(TextLCDType *lcd);
Alarm_state A_Trigged(TextLCDType *lcd);

Alarm_state A_setTemp(TextLCDType *lcd, uint8_t *setTemp);

key_code Alarm_code_status(const uint8_t *code);

void lcd_clearRow(TextLCDType *lcd,uint8_t row);




#endif /* ALARM_H_ */