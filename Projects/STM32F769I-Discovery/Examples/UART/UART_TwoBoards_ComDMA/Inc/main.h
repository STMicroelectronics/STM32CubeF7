/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComDMA/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f769i_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART6
#define USARTx_CLK_ENABLE()              __USART6_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART6_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART6_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_6
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF8_USART6
#define USARTx_RX_PIN                    GPIO_PIN_7
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_AF                     GPIO_AF8_USART6

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_STREAM              DMA2_Stream6
#define USARTx_RX_DMA_STREAM              DMA2_Stream1
#define USARTx_TX_DMA_CHANNEL             DMA_CHANNEL_5
#define USARTx_RX_DMA_CHANNEL             DMA_CHANNEL_5


/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA2_Stream6_IRQn
#define USARTx_DMA_RX_IRQn                DMA2_Stream1_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA2_Stream6_IRQHandler
#define USARTx_DMA_RX_IRQHandler          DMA2_Stream1_IRQHandler

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART6_IRQn
#define USARTx_IRQHandler                USART6_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE
  
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

