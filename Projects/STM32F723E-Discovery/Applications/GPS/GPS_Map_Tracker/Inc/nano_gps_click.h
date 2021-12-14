/**
  ******************************************************************************
  * @file    GPS/GPS_Map_Tracker/Inc/nano_gps_click.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the nano gps click operations.
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
#ifndef __NANO_GPS_CLICK_H
#define __NANO_GPS_CLICK_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f723e_discovery.h"
#include <stdarg.h>
#include <string.h>

/* Exported types ------------------------------------------------------------*/
/* UART interface definitions */
#define STMOD_UART                            UART7
#define STMOD_UART_CLK_ENABLE()               __HAL_RCC_UART7_CLK_ENABLE()
#define STMOD_UART_CLK_DISABLE()              __HAL_RCC_UART7_CLK_DISABLE()
#define STMOD_UART_FORCE_RESET()              __HAL_RCC_UART7_FORCE_RESET()
#define STMOD_UART_RELEASE_RESET()            __HAL_RCC_UART7_RELEASE_RESET()

/* STMOD_PWR */
#define STMOD_PWR_GPIO_CLKEN()                 __HAL_RCC_GPIOB_CLK_ENABLE()
#define STMOD_PWR_PORT                         GPIOB
#define STMOD_PWR_PIN                          GPIO_PIN_0

#define STMOD_WKP_GPIO_CLKEN()                 __HAL_RCC_GPIOA_CLK_ENABLE()
#define STMOD_WKP_PORT                         GPIOA
#define STMOD_WKP_PIN                          GPIO_PIN_4

/* STMOD_RESET - RESET is same for Mikrobus connector and ESP connector on MB1280 */
#define STMOD_RST_GPIO_CLKEN()                 __HAL_RCC_GPIOF_CLK_ENABLE()
#define STMOD_RST_PORT                         GPIOF
#define STMOD_RST_PIN                          GPIO_PIN_11

/* STMOD_RX */
#define STMOD_RX_GPIO_CLKEN()                 __HAL_RCC_GPIOF_CLK_ENABLE()
#define STMOD_RX_PORT                         GPIOF
#define STMOD_RX_PIN                          GPIO_PIN_6
#define STMOD_RX_AF                           GPIO_AF8_UART7

/* STMOD_TX */
#define STMOD_TX_GPIO_CLKEN()                 __HAL_RCC_GPIOF_CLK_ENABLE()
#define STMOD_TX_PORT                         GPIOF
#define STMOD_TX_PIN                          GPIO_PIN_7
#define STMOD_TX_AF                           GPIO_AF8_UART7

/* UART DMA Stream definitions */
#define STMOD_UART_DMAx_CHANNEL              DMA_CHANNEL_5
#define STMOD_UART_DMAx_STREAM               DMA1_Stream3
#define STMOD_UART_DMAx_IRQ                  DMA1_Stream3_IRQn
#define STMOD_UART_DMAx_IRQHandler           DMA1_Stream3_IRQHandler

#define STMOD_UART_IRQ                       UART7_IRQn

#define STMOD_UART_DMAx_CLK_ENABLE()         __HAL_RCC_DMA1_CLK_ENABLE()

#define STMOD_UART_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_BYTE
#define STMOD_UART_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_BYTE
#define DMA_MAX_SZE                          0xFFFF
#define DMA_MAX(x)                           ( ((x)<=DMA_MAX_SZE) ? (x):DMA_MAX_SZE )
   
#define VCP_PORT                         USART6
#define VCP_CLK_ENABLE()                 __HAL_RCC_USART6_CLK_ENABLE()
#define VCP_RX_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define VCP_TX_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()

#define VCP_TX_PIN                       GPIO_PIN_7
#define VCP_TX_GPIO_PORT                 GPIOC
#define VCP_TX_AF                        GPIO_AF8_USART6

#define VCP_RX_PIN                        GPIO_PIN_6
#define VCP_RX_GPIO_PORT                  GPIOC
#define VCP_RX_AF                         GPIO_AF8_USART6
   
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void GPS_Init(void);
void GPS_DeInit(void);
void GPS_Start(uint8_t* Buffer, uint32_t Length);
void GPS_ReceiveMsg(uint16_t* rd_idx, uint32_t timeout );
void GPS_DecodeFrameNMEA (char * Nmea);
void VCP_Init(void);
void VCP_SendMessage(char *fmt, ...);
#ifdef __cplusplus
}
#endif

#endif /* __ESP8266_IO_H */

