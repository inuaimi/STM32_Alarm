#include "LCD.h"
#include "stm32f4xx_hal.h"
#include "Delay.h"
#include <stdarg.h>



void TextLCD_Strobe(TextLCDType *lcd);



void TextLCD_Cmd(TextLCDType *lcd, uint8_t cmd);

void TextLCD_Data(TextLCDType *lcd, uint8_t data);
char *convert(unsigned int num, int base);

void TextLCD_Init(TextLCDType *lcd, GPIO_TypeDef *controlPort, uint16_t rsPin, uint16_t rwPin,uint16_t enPin, GPIO_TypeDef *dataPort, uint16_t dataPins,...){


	lcd->rsPin = rsPin;
	lcd->rwPin = rwPin;
	lcd->strbPin= enPin;
	lcd->controlPort = controlPort;
	lcd->dataPort = dataPort;
	lcd->dataPins = dataPins;
	HAL_GPIO_WritePin(controlPort, lcd->rwPin, GPIO_PIN_RESET);

	HAL_Delay(20);
	HAL_GPIO_WritePin(controlPort, lcd->rsPin, GPIO_PIN_RESET);
	TextLCD_Cmd(lcd, 0x38);
	HAL_Delay(6);
	TextLCD_Cmd(lcd, 0x38);
	delay_us(120);
	TextLCD_Cmd(lcd, 0x38);
	TextLCD_Cmd(lcd, 0x38);
	TextLCD_Cmd(lcd, 0x06);
	TextLCD_Cmd(lcd, 0x0C); //0x0e
	TextLCD_Clear(lcd);
	TextLCD_Home(lcd);


}


void TextLCD_Home(TextLCDType *lcd){

	TextLCD_Cmd(lcd, 0x80);


	delay_us(50);



}

void TextLCD_Clear(TextLCDType *lcd){

	TextLCD_Cmd(lcd, 0x01);
	HAL_Delay(2);
}

void TextLCD_Position(TextLCDType *lcd, int x, int y){


	TextLCD_Cmd(lcd, (x+(y<<6)) | 0x80);


}

void TextLCD_Putchar(TextLCDType *lcd, uint8_t data){

    TextLCD_Data(lcd,data);


}


void TextLCD_Puts(TextLCDType *lcd, char *string){


	int i=0;

    while (i < strlen(string))
    {
        TextLCD_Data(lcd,string[i]);
        i++;
    }
}


void TextLCD_PutsLen(TextLCDType *lcd, uint8_t *string, uint8_t len){

	int i=0;

    while (i < len)
    {
        TextLCD_Data(lcd,string[i]);
        i++;
    }
}


void TextLCD_Printf(TextLCDType *lcd, char *message, ...){

    char *p;
    int i;
    unsigned u;
    char *s;

    va_list argp;
    va_start(argp, message);

    p = message;

    for(p=message; *p!='\0';p++)
    {
        if(*p != '%')
        {
        	TextLCD_Putchar(lcd, *p);
        	continue;
        }

        p++;

        switch(*p)
        {
             case 'c' :
                       i = va_arg(argp,int);
                       TextLCD_Putchar(lcd, i);
                       break;
             case 'd' :
                       i = va_arg(argp,int);
                       if(i<0)
                       {
                            i = -i;
                            TextLCD_Putchar(lcd,'-');
                       }
                       TextLCD_Puts(lcd,convert(i,10));
                       break;
             case 'o':
                       i = va_arg(argp,unsigned int);
                       TextLCD_Puts(lcd,convert(i,8));
                       break;
             case 's':
                       s = va_arg(argp,char *);
                       TextLCD_Puts(lcd, s);
                       break;
             case 'u':
                       u=va_arg(argp, unsigned int);
                       TextLCD_Puts(lcd, convert(u,10));
                       break;
             case 'x':
                       u=va_arg(argp, unsigned int);
                       TextLCD_Puts(lcd, convert(u,16));
                       break;
             case '%':
            	 	 TextLCD_Putchar(lcd,'%');
                       break;
          }
      }

      va_end(argp);



}


//Private:
void TextLCD_Strobe(TextLCDType *lcd){


	HAL_GPIO_WritePin(lcd->controlPort,  lcd->strbPin,  GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(lcd->controlPort, lcd->strbPin, GPIO_PIN_RESET);
	delay_us(5);
}



void TextLCD_Cmd(TextLCDType *lcd, uint8_t cmd){

	HAL_GPIO_WritePin(lcd->controlPort, lcd->rsPin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(lcd->controlPort, lcd->rwPin, GPIO_PIN_RESET);

	lcd->dataPort->ODR = lcd->dataPins & cmd;

	TextLCD_Strobe(lcd);
	delay_us(50);



}

void TextLCD_Data(TextLCDType *lcd, uint8_t data){
	HAL_GPIO_WritePin(lcd->controlPort, lcd->rsPin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(lcd->controlPort, lcd->rwPin, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(lcd->dataPort, data, GPIO_PIN_SET );
	lcd->dataPort->ODR = lcd->dataPins & data;
	TextLCD_Strobe(lcd);
	delay_us(50);



}


char *convert(unsigned int num, int base)
{
   static char buf[33];
   char *ptr;

   ptr=&buf[sizeof(buf)-1];
   *ptr='\0';
   do
   {
       *--ptr = "0123456789abcdef"[num%base];
       num/=base;
   }while(num!=0);
   return(ptr);
}

