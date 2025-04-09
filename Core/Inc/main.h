/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define KEY2_Pin GPIO_PIN_2
#define KEY2_GPIO_Port GPIOE
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define TOUCH_SPI_CS_Pin GPIO_PIN_13
#define TOUCH_SPI_CS_GPIO_Port GPIOC
#define DS0_Pin GPIO_PIN_9
#define DS0_GPIO_Port GPIOF
#define DS1_Pin GPIO_PIN_10
#define DS1_GPIO_Port GPIOF
#define TOUCH_SPI_SCK_Pin GPIO_PIN_0
#define TOUCH_SPI_SCK_GPIO_Port GPIOB
#define TOUCH_PEN_Pin GPIO_PIN_1
#define TOUCH_PEN_GPIO_Port GPIOB
#define TOUCH_SPI_MISO_Pin GPIO_PIN_2
#define TOUCH_SPI_MISO_GPIO_Port GPIOB
#define TOUCH_SPI_MOSI_Pin GPIO_PIN_11
#define TOUCH_SPI_MOSI_GPIO_Port GPIOF
#define SPI_Flash_CS_Pin GPIO_PIN_14
#define SPI_Flash_CS_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_15
#define LCD_BL_GPIO_Port GPIOB
#define USB_PWR_Pin GPIO_PIN_15
#define USB_PWR_GPIO_Port GPIOA
#define ETH_RESET_Pin GPIO_PIN_3
#define ETH_RESET_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
