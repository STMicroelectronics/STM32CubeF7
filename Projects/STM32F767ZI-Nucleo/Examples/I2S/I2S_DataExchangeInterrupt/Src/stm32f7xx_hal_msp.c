/**
  ******************************************************************************
  * @file    I2S/I2S_DataExchangeInterrupt/Src/stm32f7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @defgroup I2S_DataExchangeInterrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief I2S MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hi2s: I2S handle pointer
  * @retval None
  */
void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
  RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;
  GPIO_InitTypeDef  GPIO_InitStruct;  
   
  /* I2S clock config 
  PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) × (PLLI2SN/PLLM)
  I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
  RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  RCC_ExCLKInitStruct.I2sClockSelection = RCC_I2SCLKSOURCE_PLLI2S;
  RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = 192;
  RCC_ExCLKInitStruct.PLLI2S.PLLI2SR = 2;
  HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct);
  
    
  /* Enable I2S clock */
  __HAL_RCC_SPI1_CLK_ENABLE();
  
  /* Enable SCK, SD and WS GPIO clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /* CODEC_I2S pins configuration: WS, SCK and SD pins */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* I2S IRQ Configuration */
  HAL_NVIC_SetPriority(SPI1_IRQn, 0xF, 0);
  HAL_NVIC_EnableIRQ(SPI1_IRQn); 
}

/**
  * @brief I2S MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s)
{
  /* Disable I2S clock */
  __HAL_RCC_SPI1_CLK_ENABLE();
  
  /* Disable SCK, SD and WS GPIO clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
