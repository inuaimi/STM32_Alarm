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

#define RS 10000.0f

typedef enum{Alarm_init, Alarm_idle, ALarm_arming,
	Alarm_armed,Alarm_PRE_Trigged,Alarm_Trigged, Alarm_SetTemp,Alarm_SetGyro, Alarm_setCode} Alarm_state;

typedef enum{Key_No_Pressed, Key_Pressed,Key_OK,Key_Wrong, Key_A, Key_B,Key_C, Key_D} key_code;

typedef enum{NONE, Motion_Trigged,Sound_Trigged,Diamond_Trigged} sensor_status;

typedef enum{L_OFF, L_RED,L_YELLOW,L_GREEN} Led_Color;

typedef enum{LCD_NONE, LCD_Unlocked, LCD_Locked,LCD_Arming,LCD_PRI_Trigged,LCD_Trigged, LCD_SetTemp, LCD_SetGyro,LCD_SetCode} LCD_Status;


void Alarm_status();
Alarm_state A_idle(TextLCDType *lcd,uint8_t setTemp, uint8_t *code);
Alarm_state A_arming(TextLCDType *lcd,uint8_t *code);
Alarm_state A_armed(TextLCDType *lcd,uint8_t setTemp,uint8_t *code);

Alarm_state A_Pre_Trigged(TextLCDType *lcd,uint8_t *code);
Alarm_state A_Trigged(TextLCDType *lcd);

Alarm_state A_setTemp(TextLCDType *lcd, uint8_t *setTemp);
Alarm_state A_setGyro(TextLCDType *lcd);
Alarm_state A_setCode(TextLCDType *lcd,uint8_t *setCode);


key_code Alarm_code_status(const uint8_t *code);

void lcd_clearRow(TextLCDType *lcd,uint8_t row);
uint8_t check_sensors(uint8_t setTemp);

int16_t Read_Analog_Temp();

key_code Alarm_code_status_2(uint16_t code);






#endif /* ALARM_H_ */
