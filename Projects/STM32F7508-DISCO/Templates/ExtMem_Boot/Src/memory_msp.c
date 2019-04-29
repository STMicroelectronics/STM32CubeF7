/**
  ******************************************************************************
  * @file    memory_msp.c
  * @author  MCD Application Team
  * @brief   External memory code execution MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
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
#include "stm32f7xx_hal.h"
#include "memory.h"
#include "memory_msp.h"
 
/** @addtogroup MEMORY
  * @{
  */

/** @defgroup MEMORY
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup MEMORY_MSP_Private_Functions
  * @{
  */

#if (CODE_AREA == USE_QSPI) || (BINARY_AREA == USE_SPI_NOR)
/**
  * @brief QSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for QSPI interrupt
  * @retval None
  */
void QSPI_MspInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the QuadSPI memory interface clock */
  __HAL_RCC_QSPI_CLK_ENABLE();
  /* Reset the QuadSPI memory interface */
  __HAL_RCC_QSPI_FORCE_RESET();
  __HAL_RCC_QSPI_RELEASE_RESET();
  /* Enable GPIO clocks */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();


  /*##-2- Configure peripheral GPIO ##########################################*/
  /* QSPI CS GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_6;
  gpio_init_structure.Alternate = GPIO_AF10_QUADSPI;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /* QSPI CLK GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_2;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /* QSPI D0 GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_11;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
  /* QSPI D1 GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_12;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
#if (CODE_AREA == USE_QSPI)
  /* QSPI D2 GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_2;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  /* QSPI D3 GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_13;
  gpio_init_structure.Alternate = GPIO_AF9_QUADSPI;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
#endif
}
#endif

#if (BINARY_AREA == USE_SPI_NOR)
/**
  * @brief QSPI MSP De-Initialization
  *        This function free the hardware resources not needed anymore:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for QSPI interrupt
  * @retval None
  */
void QSPI_MspDeInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  /*##-1- Reset peripheral GPIO ##########################################*/
  /* QSPI CS GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_6;
  gpio_init_structure.Mode      = GPIO_MODE_INPUT;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /* QSPI CLK GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /* QSPI D0 GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_11;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* QSPI D1 GPIO pin configuration  */
  gpio_init_structure.Pin       = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Reset the QuadSPI memory interface */
  __HAL_RCC_QSPI_FORCE_RESET();
  __HAL_RCC_QSPI_RELEASE_RESET();

  /* Disable the QuadSPI memory interface clock */
  __HAL_RCC_QSPI_CLK_DISABLE();

  /* Disable GPIO clocks */
  __HAL_RCC_GPIOB_CLK_DISABLE();
  /* External mamory configuration requires GPIOF */
  /* __HAL_RCC_GPIOF_CLK_DISABLE(); */
}
#endif

#if (DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)
/**
  * @brief  Initializes SDRAM MSP.
  * @retval None
  */
void SDRAM_MspInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;
  
  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  
  /* GPIOC configuration */
  gpio_init_structure.Pin   = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                              GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                              GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_8 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOH configuration */  
  gpio_init_structure.Pin   = GPIO_PIN_3 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure); 
  
}
#endif /*(DATA_AREA == USE_EXTERNAL_SDRAM) || (CODE_AREA == USE_EXTERNAL_SDRAM)*/

#if (BINARY_AREA == USE_SDCARD)
/**
  * @brief  Initializes the SD (&i2c mxp) MSP.
  * @param  hsd: SD handle
  * @param  Params  
  * @retval None
  */
void SDCARD_MspInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;
  
/* GPIO setting for SDCARD */
  /* Enable SDIO clock */
  __HAL_RCC_SDMMC1_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_SDMMC1;
  
  /* GPIOC configuration */
  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
   
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  /* GPIOD configuration */
  gpio_init_structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /*Dectect pin */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* GPIO configuration in input for uSD_Detect signal */
  gpio_init_structure.Pin       = GPIO_PIN_13;
  gpio_init_structure.Mode      = GPIO_MODE_INPUT;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);


///* GPIO setting for I2C */
//  /*** Configure the GPIOs ***/  
//  /* Enable GPIO clock */
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  
//  /* Configure I2C Tx as alternate function */
//  gpio_init_structure.Pin = GPIO_PIN_8;
//  gpio_init_structure.Mode = GPIO_MODE_AF_OD;
//  gpio_init_structure.Pull = GPIO_NOPULL;
//  gpio_init_structure.Speed = GPIO_SPEED_FAST;
//  gpio_init_structure.Alternate = GPIO_AF4_I2C1;
//  HAL_GPIO_Init(GPIOB, &gpio_init_structure);
//  
//  /* Configure I2C Rx as alternate function */
//  gpio_init_structure.Pin = GPIO_PIN_9;
//  HAL_GPIO_Init(GPIOB, &gpio_init_structure);
//  
//  /*** Configure the I2C peripheral ***/ 
//  /* Enable I2C clock */
//  __HAL_RCC_I2C1_CLK_ENABLE();
//  
//  /* Force the I2C peripheral clock reset */  
//  __HAL_RCC_I2C1_FORCE_RESET(); 
//  
//  /* Release the I2C peripheral clock reset */  
//  __HAL_RCC_I2C1_RELEASE_RESET();
}

/**
  * @brief  De-Initializes the SD (&i2c mxp) MSP.
  * @param  hsd: SD handle
  * @param  Params  
  * @retval None
  */
void SDCARD_MspDeInit(void)
{
  GPIO_InitTypeDef gpio_init_structure;
  
/* Restore default GPIO setting */
  /* Common GPIO configuration */
  gpio_init_structure.Mode      = GPIO_MODE_INPUT;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_LOW;
  
  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

  /*** Disable all uneeded clocks ***/ 
  __HAL_RCC_SDMMC1_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_I2C1_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();

  /* External mamory configuration requires GPIOD */
  /* __HAL_RCC_GPIOD_CLK_DISABLE(); */
}
#endif /* (BINARY_AREA == USE_SDCARD) */

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
