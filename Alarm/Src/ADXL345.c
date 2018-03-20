/*
 * ADXL345.c
 *
 *  Created on: 19 mars 2018
 *      Author: ibraheemal-nuaimi
 */

#include "ADXL345.h"
#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

void ADXL345_WriteRegister(uint8_t regst, uint8_t value){

	uint8_t data[2];
	data[0] = regst;
	data[1] = value;
	HAL_I2C_Master_Transmit (&hi2c1, DEVAD<<1, data, 2, 100); //size 2 and timeout 100ms
}
void ADXL345_ReadRegister(uint8_t regst, uint8_t *tmpsingelData){


	HAL_I2C_Mem_Read(&hi2c1, DEVAD<<1, regst, 1, tmpsingelData, 1, 100);

}

void ADXL345_ReadRegisters(uint8_t regst,int8_t *tmpData, uint8_t dataLength){


	HAL_I2C_Mem_Read(&hi2c1, DEVAD <<1, regst, 1, tmpData, dataLength, 100);

}

void ADXL345_Init(){



	ADXL345_WriteRegister(0x31, 0x01);  // 0x31 Dataformat register ! 0x02 sets the data range to +4g

	ADXL345_WriteRegister(0x2D, 0x00); // 0x2D Power_ctl register ! 0x00 sets all the bits to 0

	ADXL345_WriteRegister(0x2D, 0x08); // 0x2D Power_ctl register ! 0x80 sets the measure bit to 1 and wake up to 8Hz

}

void ADXL345_init_interrupt(){


	int txt[100];
	int len;
	uint8_t dummydata;
	ADXL345_ReadRegister(0x00,&dummydata); // read the DEVID

	len = sprintf(txt,"ALARM Data %x\n\r",dummydata);
	HAL_UART_Transmit(&huart2,txt,len,100);


	ADXL345_WriteRegister(ADXL345_POWER_CTL, 0x00); //  0x00 sets all the bits to 0
	ADXL345_WriteRegister(ADXL345_POWER_CTL, 0x10); //  0x10 Auto sleep
	//ADXL345_WriteRegister(ADXL345_POWER_CTL, 0x08); //  0x08 Measure mode

	ADXL345_WriteRegister(ADXL345_DATA_FORMAT,0x21); //  0x21 sets the data range to +4g and invert interrupt

	//activity/ inactivity thresholds (0-255)
	ADXL345_WriteRegister(ADXL345_THRESH_ACT,0x4B); //0x4B = 75 (75*62.5mg)
	//ADXL345_WriteRegister(ADXL345_THRESH_INACT, 0x4B);
	//ADXL345_WriteRegister(ADXL345_TIME_INACT,0x05); // 5 seconds of no activity is inactive

	//look for activity/inactivity on all axes
	ADXL345_WriteRegister(ADXL345_ACT_INACT_CTL,0x77); // AC/DC is set to 0

	//look for tap movement only on z axis and suppress button to zero *
	ADXL345_WriteRegister(ADXL345_TAP_AXES,0x01);

	//Tap Threshold *
	ADXL345_WriteRegister(ADXL345_THRESH_TAP,0x0F); //0x4b = 75 (75*62.5mg)

	//Tap Duration *
	ADXL345_WriteRegister(ADXL345_DUR,0x0F); // 15 *625us

	//Double Tap Latency *
	ADXL345_WriteRegister(ADXL345_LATENT,0x4B); // 0x4B = 75 scale factor 1.25ms/LSB

	//Double Tap Window *
	ADXL345_WriteRegister(ADXL345_WINDOW,0xC8); //1.25ms per increment (C8=200)

	// free fall Threshold (0-255)
	//ADXL345_WriteRegister(ADXL345_THRESH_FF,0x06); //62.5mg (300mg - 600mg)

	// Free fall Duration
	//ADXL345_WriteRegister(ADXL345_TIME_FF,0x30); // scale factor 5ms/LSB (0x14 - ox46 recommended)

	//Setting all interrupts to take place on PIN INT1 (DATA_READY,Watermark and overrun are set to INT2)
	ADXL345_WriteRegister(ADXL345_INT_MAP,0x83);

	//Enabling Single_tap, double_tap, activity interrupt
	ADXL345_WriteRegister(ADXL345_INT_ENABLE,0x70);

	ADXL345_WriteRegister(ADXL345_POWER_CTL, 0x08); //  0x08 Measure mode

	ADXL345_Clear();


	len = sprintf(txt,"Done!!");
	HAL_UART_Transmit(&huart2,txt,len,100);

}

void ADXL345_Ready(){

	  //while(HAL_I2C_IsDeviceReady(&hi2c1,devAd,1,10) != HAL_OK)
		//		  ;
	  //len = sprintf(bufftx,"--Sensor ready---\n");
	  //HAL_UART_Transmit(&huart2,bufftx,len,100 );

}


void ADXL345_Clear(){

	uint8_t Data = 0;


	ADXL345_ReadRegister(ADXL345_INT_SOURCE,&Data);


}
void ADXL345_setThreshold(uint8_t tmp_threshold){

	if(!(0 <= tmp_threshold && tmp_threshold <= 255))
		return;

	//Disable All interrupts
	ADXL345_WriteRegister(ADXL345_INT_ENABLE,0x00);


	ADXL345_WriteRegister(ADXL345_THRESH_ACT,tmp_threshold);

	//Enabling Single_tap, double_tap, activity interrupt
	ADXL345_WriteRegister(ADXL345_INT_ENABLE,0x70);


}
