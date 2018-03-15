/*
 * ADXL345.h
 *
 *  Created on: 8 mars 2018
 *      Author: ibraheemal-nuaimi
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#define DEVAD 0x53

typedef struct {
	I2C_HandleTypeDef i2c1;
	int8_t Data[6];
	float x,y,z;
	float xg,yg,zg;

} ADXL345;

void ADXL345_Init(ADXL345 *adx);

void ADXL345_WriteRegister(ADXL345 *adx ,uint8_t regst, uint8_t value);

void ADXL345_ReadRegister(ADXL345 *adx ,uint8_t regst, uint8_t *singelData);

void ADXL345_ReadRegisters(ADXL345 *adx ,uint8_t regst, int8_t *tmpData, uint8_t dataLength);

void ADXL345_Calc(ADXL345 *adx);

#endif /* ADXL345_H_ */
