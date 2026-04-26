/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUILTIN_LED_Pin GPIO_PIN_13
#define BUILTIN_LED_GPIO_Port GPIOC
#define Stopper_pin_Pin GPIO_PIN_2
#define Stopper_pin_GPIO_Port GPIOA
#define IN2_Pin GPIO_PIN_5
#define IN2_GPIO_Port GPIOA
#define IN1_Pin GPIO_PIN_7
#define IN1_GPIO_Port GPIOA
#define Keyboard_pin6_Pin GPIO_PIN_0
#define Keyboard_pin6_GPIO_Port GPIOB
#define Keyboard_pin7_Pin GPIO_PIN_1
#define Keyboard_pin7_GPIO_Port GPIOB
#define Keyboard_pin8_Pin GPIO_PIN_10
#define Keyboard_pin8_GPIO_Port GPIOB
#define Keyboard_pin9_Pin GPIO_PIN_11
#define Keyboard_pin9_GPIO_Port GPIOB
#define IR_sensor_pin_Pin GPIO_PIN_13
#define IR_sensor_pin_GPIO_Port GPIOB
#define Keyboard_pin5_Pin GPIO_PIN_12
#define Keyboard_pin5_GPIO_Port GPIOA
#define Keyboard_pin4_Pin GPIO_PIN_15
#define Keyboard_pin4_GPIO_Port GPIOA
#define Keyboard_pin3_Pin GPIO_PIN_3
#define Keyboard_pin3_GPIO_Port GPIOB
#define Keyboard_pin2_Pin GPIO_PIN_4
#define Keyboard_pin2_GPIO_Port GPIOB
#define Keyboard_pin1_Pin GPIO_PIN_5
#define Keyboard_pin1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
