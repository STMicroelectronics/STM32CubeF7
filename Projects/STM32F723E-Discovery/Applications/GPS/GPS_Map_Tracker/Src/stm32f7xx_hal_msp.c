/**
  ******************************************************************************
  * @file    GPS/GPS_Map_Tracker/Src/stm32f7xx_hal_msp.c
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
#include "nano_gps_click.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
extern UART_HandleTypeDef  STMOD_UART_Handle;
  DMA_HandleTypeDef   hdma_STModUart;
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

 if (huart == &STMOD_UART_Handle )
 {
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Alternate = 0;


 __HAL_RCC_GPIOH_CLK_ENABLE();
 __HAL_RCC_GPIOI_CLK_ENABLE();
 
 /* on MB1260 REVB, there are Pull-up and pull-down on PMOD_SEL_0 and PMOD_SEL_1 to set UART as default */
 HAL_GPIO_DeInit( GPIOH,GPIO_PIN_15 );
 HAL_GPIO_DeInit( GPIOI, GPIO_PIN_10 );

  GPIO_InitStructure.Pin       = GPIO_PIN_15;
  GPIO_InitStructure.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init( GPIOH, &GPIO_InitStructure );
  GPIO_InitStructure.Pin       = GPIO_PIN_10;
  HAL_GPIO_Init( GPIOI,&GPIO_InitStructure );

  STMOD_UART_CLK_ENABLE();
  STMOD_UART_FORCE_RESET();
  STMOD_UART_RELEASE_RESET();


  STMOD_WKP_GPIO_CLKEN();
  GPIO_InitStructure.Pin       = STMOD_WKP_PIN;
  GPIO_InitStructure.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init( STMOD_WKP_PORT, &GPIO_InitStructure );


  STMOD_RST_GPIO_CLKEN();
  GPIO_InitStructure.Pin       = STMOD_RST_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init( STMOD_RST_PORT, &GPIO_InitStructure );
  
  /* Reset modules */
  HAL_GPIO_WritePin( STMOD_RST_PORT, STMOD_RST_PIN, GPIO_PIN_SET );  

  /* Power Mikrobus or set GPIO0 - GPIO 2 */
  GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;

  STMOD_PWR_GPIO_CLKEN();
  GPIO_InitStructure.Pin       = STMOD_PWR_PIN;
  HAL_GPIO_Init( STMOD_PWR_PORT, &GPIO_InitStructure );
  HAL_GPIO_WritePin( STMOD_PWR_PORT, STMOD_PWR_PIN, GPIO_PIN_RESET );


  /* STMOD_RX connected to UART_Tx */
  STMOD_RX_GPIO_CLKEN();
  GPIO_InitStructure.Pin       = STMOD_RX_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Alternate = STMOD_RX_AF;
  HAL_GPIO_Init( STMOD_RX_PORT, &GPIO_InitStructure );

  /* STMOD_TX connected to  UART_Rx */
  STMOD_TX_GPIO_CLKEN();
  GPIO_InitStructure.Pin       = STMOD_TX_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Alternate = STMOD_TX_AF;
  HAL_GPIO_Init( STMOD_TX_PORT, &GPIO_InitStructure );

  /* DMA configuration */
  STMOD_UART_DMAx_CLK_ENABLE(); 

  hdma_STModUart.Instance                 = STMOD_UART_DMAx_STREAM;
  hdma_STModUart.Init.Channel             = STMOD_UART_DMAx_CHANNEL;
  hdma_STModUart.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_STModUart.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_STModUart.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_STModUart.Init.PeriphDataAlignment = STMOD_UART_DMAx_PERIPH_DATA_SIZE;
  hdma_STModUart.Init.MemDataAlignment    = STMOD_UART_DMAx_MEM_DATA_SIZE;
  hdma_STModUart.Init.Mode                = DMA_CIRCULAR;
  hdma_STModUart.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_STModUart.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  hdma_STModUart.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_STModUart.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_STModUart.Init.PeriphBurst         = DMA_PBURST_SINGLE;

  __HAL_LINKDMA( &STMOD_UART_Handle, hdmarx, hdma_STModUart );

  /* Configure the DMA Stream */
  HAL_DMA_Init( &hdma_STModUart );
  
  
  /* Release Mikrobus GPS RESET */
  HAL_Delay( 1000 );  
  HAL_GPIO_WritePin( STMOD_RST_PORT, STMOD_RST_PIN, GPIO_PIN_RESET );
  
  HAL_Delay( 500 );
  
  
  HAL_GPIO_WritePin( STMOD_PWR_PORT, STMOD_PWR_PIN, GPIO_PIN_SET );
  HAL_Delay(1500);  /* Power up pulse at least 1s for GPS chip */
  HAL_GPIO_WritePin( STMOD_PWR_PORT, STMOD_PWR_PIN, GPIO_PIN_RESET );

  HAL_NVIC_SetPriority(STMOD_UART_DMAx_IRQ, 4, 0 );
  HAL_NVIC_EnableIRQ(STMOD_UART_DMAx_IRQ );

  HAL_NVIC_SetPriority( STMOD_UART_IRQ, 5, 0 );
  HAL_NVIC_EnableIRQ( STMOD_UART_IRQ );
 }
 else
 {
   /* Enable clock ST-link used as VCP: USART6 */
  VCP_CLK_ENABLE();

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  VCP_TX_GPIO_CLK_ENABLE();
  VCP_RX_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStructure.Pin       = VCP_TX_PIN;
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull      = GPIO_NOPULL;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStructure.Alternate = VCP_TX_AF;

  HAL_GPIO_Init(VCP_TX_GPIO_PORT, &GPIO_InitStructure);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStructure.Pin = VCP_RX_PIN;
  GPIO_InitStructure.Alternate = VCP_RX_AF;

  HAL_GPIO_Init(VCP_RX_GPIO_PORT, &GPIO_InitStructure);
 }
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  STMOD_UART_CLK_DISABLE();

  /*##-2- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(STMOD_UART_DMAx_IRQ);
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
