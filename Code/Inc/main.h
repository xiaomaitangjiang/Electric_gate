/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "stm32f1xx_hal.h"
extern DMA_HandleTypeDef hdma_usart1_rx;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <cstdlib>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void delay_us(uint16_t us);
uint8_t errorprocessing (uint16_t errortype,uint16_t * index);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM_HANDLE &htim4
#define leak_detect_Pin GPIO_PIN_5
#define leak_detect_GPIO_Port GPIOA
#define leak_detect_EXTI_IRQn EXTI9_5_IRQn
#define button_Pin GPIO_PIN_7
#define button_GPIO_Port GPIOA
#define buttonB0_Pin GPIO_PIN_0
#define buttonB0_GPIO_Port GPIOB
#define buttonB1_Pin GPIO_PIN_1
#define buttonB1_GPIO_Port GPIOB
#define buttonB12_Pin GPIO_PIN_12
#define buttonB12_GPIO_Port GPIOB
#define buzzer_Pin GPIO_PIN_11
#define buzzer_GPIO_Port GPIOA
#define power_Pin GPIO_PIN_3
#define power_GPIO_Port GPIOB
#define power_EXTI_IRQn EXTI3_IRQn
#define Gatecontrol_pin_Pin GPIO_PIN_4
#define Gatecontrol_pin_GPIO_Port GPIOB

//数组长度求值，不要使用过于复杂的表达式
#ifndef Listlength
#define Listlength(_array)  (sizeof(_array)/sizeof(_array[0]))
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
