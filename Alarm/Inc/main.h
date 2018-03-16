/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LCD_D0_Pin GPIO_PIN_0
#define LCD_D0_GPIO_Port GPIOC
#define LCD_D1_Pin GPIO_PIN_1
#define LCD_D1_GPIO_Port GPIOC
#define LCD_D2_Pin GPIO_PIN_2
#define LCD_D2_GPIO_Port GPIOC
#define LCD_D3_Pin GPIO_PIN_3
#define LCD_D3_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_1
#define LED_GREEN_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_4
#define LED_RED_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define PAD_COLUM_1_Pin GPIO_PIN_6
#define PAD_COLUM_1_GPIO_Port GPIOA
#define PAD_COLUM_2_Pin GPIO_PIN_7
#define PAD_COLUM_2_GPIO_Port GPIOA
#define LCD_D4_Pin GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOC
#define LCD_D5_Pin GPIO_PIN_5
#define LCD_D5_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_0
#define LCD_RS_GPIO_Port GPIOB
#define LCD_RW_Pin GPIO_PIN_1
#define LCD_RW_GPIO_Port GPIOB
#define LCD_E_Pin GPIO_PIN_2
#define LCD_E_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOC
#define LCD_D7_Pin GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOC
#define PAD_ROW_1_Pin GPIO_PIN_9
#define PAD_ROW_1_GPIO_Port GPIOC
#define PAD_COLUM_3_Pin GPIO_PIN_8
#define PAD_COLUM_3_GPIO_Port GPIOA
#define PAD_COLUM_4_Pin GPIO_PIN_9
#define PAD_COLUM_4_GPIO_Port GPIOA
#define LED_YELLOW_Pin GPIO_PIN_10
#define LED_YELLOW_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define PAD_ROW_2_Pin GPIO_PIN_10
#define PAD_ROW_2_GPIO_Port GPIOC
#define PAD_ROW_3_Pin GPIO_PIN_11
#define PAD_ROW_3_GPIO_Port GPIOC
#define PAD_ROW_4_Pin GPIO_PIN_12
#define PAD_ROW_4_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define MOTION_SENSOR_Pin GPIO_PIN_4
#define MOTION_SENSOR_GPIO_Port GPIOB
#define MOTION_SENSOR_EXTI_IRQn EXTI4_IRQn

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/