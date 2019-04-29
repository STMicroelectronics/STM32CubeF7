/**
  ******************************************************************************
  * @file    LPTIM/LPTIM_PWMExternalClock/Src/stm32f7xx_hal_msp.c
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

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
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
* @brief  LPTIM MSP Init
* @param  hlptim : LPTIM handle
* @retval None
*/
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim)
{
GPIO_InitTypeDef     GPIO_InitStruct;
    
  /* ## - 1 - Enable LPTIM clock ############################################ */
  __HAL_RCC_LPTIM1_CLK_ENABLE();
  
  /* ## - 2 - Force & Release the LPTIM Periheral Clock Reset ############### */  
  /* Force the LPTIM Periheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();
  
  /* Release the LPTIM Periheral Clock Reset */  
  __HAL_RCC_LPTIM1_RELEASE_RESET();
  
  /* ## - 3 - Enable & Configure LPTIM Ultra Low Power Input ################# */
  /* Configure PB.02 (LPTIM1_OUT) and PB.05 (LPTIM1_IN1) in alternate function,
  Low speed push-pull mode and pull-up enabled.
  Note: In order to reduce power consumption: GPIO Speed is configured in
  LowSpeed */
  
  /* Enable GPIO PORT(s)*/
  LPTIMx_GPIO_PORT_ENABLE
  
  /* Configure PB.02 */
  GPIO_InitStruct.Pin = GPIO_PIN_OUT;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PUPD_OUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW     ;
  GPIO_InitStruct.Alternate = GPIO_AF_LPTIMx_OUT;  
  HAL_GPIO_Init(LPTIMx_OUT_GPIO_PORT, &GPIO_InitStruct);
  
  /* Configure PB.05 */
  GPIO_InitStruct.Pin = GPIO_PIN_IN1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PUPD_IN1;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW     ;
  GPIO_InitStruct.Alternate = GPIO_AF_LPTIMx_IN1;  
  HAL_GPIO_Init(LPTIMx_IN1_GPIO_PORT, &GPIO_InitStruct);  
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
