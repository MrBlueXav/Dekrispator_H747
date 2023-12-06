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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define ULPI_D7_Pin GPIO_PIN_5
#define ULPI_D7_GPIO_Port GPIOB
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
#define STLINK_TX_Pin GPIO_PIN_10
#define STLINK_TX_GPIO_Port GPIOA
#define STLINK_RX_Pin GPIO_PIN_9
#define STLINK_RX_GPIO_Port GPIOA
#define B2_Pin GPIO_PIN_13
#define B2_GPIO_Port GPIOC
#define CEC_CK_MCO1_Pin GPIO_PIN_8
#define CEC_CK_MCO1_GPIO_Port GPIOA
#define ULPI_DIR_Pin GPIO_PIN_11
#define ULPI_DIR_GPIO_Port GPIOI
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define ULPI_STP_Pin GPIO_PIN_0
#define ULPI_STP_GPIO_Port GPIOC
#define ULPI_NXT_Pin GPIO_PIN_4
#define ULPI_NXT_GPIO_Port GPIOH
#define ULPI_D3_Pin GPIO_PIN_10
#define ULPI_D3_GPIO_Port GPIOB
#define ULPI_D4_Pin GPIO_PIN_11
#define ULPI_D4_GPIO_Port GPIOB
#define ULPI_CK_Pin GPIO_PIN_5
#define ULPI_CK_GPIO_Port GPIOA
#define ULPI_D2_Pin GPIO_PIN_1
#define ULPI_D2_GPIO_Port GPIOB
#define ULPI_D5_Pin GPIO_PIN_12
#define ULPI_D5_GPIO_Port GPIOB
#define ULPI_D0_Pin GPIO_PIN_3
#define ULPI_D0_GPIO_Port GPIOA
#define ULPI_D1_Pin GPIO_PIN_0
#define ULPI_D1_GPIO_Port GPIOB
#define ULPI_D6_Pin GPIO_PIN_13
#define ULPI_D6_GPIO_Port GPIOB


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
