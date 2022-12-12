/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define CS_Pin GPIO_PIN_0
#define CS_GPIO_Port GPIOC
#define OE_Pin GPIO_PIN_1
#define OE_GPIO_Port GPIOC
#define A6_Pin GPIO_PIN_2
#define A6_GPIO_Port GPIOC
#define A7_Pin GPIO_PIN_3
#define A7_GPIO_Port GPIOC
#define CLK_OUT_Pin GPIO_PIN_0
#define CLK_OUT_GPIO_Port GPIOA
#define D5_Pin GPIO_PIN_5
#define D5_GPIO_Port GPIOA
#define D6_Pin GPIO_PIN_6
#define D6_GPIO_Port GPIOA
#define D7_Pin GPIO_PIN_7
#define D7_GPIO_Port GPIOA
#define RESET_Pin GPIO_PIN_0
#define RESET_GPIO_Port GPIOB
#define D4_Pin GPIO_PIN_8
#define D4_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_9
#define D3_GPIO_Port GPIOA
#define D2_Pin GPIO_PIN_10
#define D2_GPIO_Port GPIOA
#define D1_Pin GPIO_PIN_11
#define D1_GPIO_Port GPIOA
#define D0_Pin GPIO_PIN_12
#define D0_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define A4_Pin GPIO_PIN_15
#define A4_GPIO_Port GPIOA
#define A0_Pin GPIO_PIN_10
#define A0_GPIO_Port GPIOC
#define A1_Pin GPIO_PIN_11
#define A1_GPIO_Port GPIOC
#define A2_Pin GPIO_PIN_12
#define A2_GPIO_Port GPIOC
#define A3_Pin GPIO_PIN_2
#define A3_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define A5_Pin GPIO_PIN_7
#define A5_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
