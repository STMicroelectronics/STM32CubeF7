/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   main application header file. 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __MAIN_H
#define __MAIN_H

#define USE_DHCP

#include "stm32f7xx_hal.h"

#ifdef USE_LCD
#include "stm32f769i_discovery_lcd.h"
#include "lcd_log.h"
#endif

#include "stm32f769i_discovery.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rng.h"



#define SERVER_PORT "4433"

#ifdef USE_DHCP
#define SERVER_NAME "10.157.11.154"
#else
#define SERVER_NAME "192.168.1.2"
#endif

#define GET_REQUEST "GET / HTTP/1.0\r\n\r\n"


#ifdef USE_DHCP

#define IP_ADDR0  0
#define IP_ADDR1  0
#define IP_ADDR2  0
#define IP_ADDR3  0

#define GW_ADDR0  0
#define GW_ADDR1  0
#define GW_ADDR2  0
#define GW_ADDR3  0

#define MASK_ADDR0  0
#define MASK_ADDR1  0
#define MASK_ADDR2  0
#define MASK_ADDR3  0

#else

#define IP_ADDR0  192
#define IP_ADDR1  168
#define IP_ADDR2  1
#define IP_ADDR3  1

#define GW_ADDR0  192
#define GW_ADDR1  168
#define GW_ADDR2  1
#define GW_ADDR3  1

#define MASK_ADDR0  255
#define MASK_ADDR1  255
#define MASK_ADDR2  254
#define MASK_ADDR3  0

#endif /* USE_DHCP */
#ifdef MBEDTLS_MEMORY_BUFFER_ALLOC_C
#define MAX_MEM_SIZE 80 * 1024
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifndef USE_LCD

#define STLINK_USARTx                           USART1
#define STLINK_USARTx_CLK_ENABLE()              __USART1_CLK_ENABLE()
#define STLINK_USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define STLINK_USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define STLINK_USARTx_FORCE_RESET()             __USART1_FORCE_RESET()
#define STLINK_USARTx_RELEASE_RESET()           __USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define STLINK_USARTx_TX_PIN                    GPIO_PIN_9
#define STLINK_USARTx_TX_GPIO_PORT              GPIOA
#define STLINK_USARTx_TX_AF                     GPIO_AF7_USART1
#define STLINK_USARTx_RX_PIN                    GPIO_PIN_10
#define STLINK_USARTx_RX_GPIO_PORT              GPIOA
#define STLINK_USARTx_RX_AF                     GPIO_AF7_USART1

#endif

void SSL_Client(void const *argument);
void Error_Handler(void);
void Success_Handler(void);


#endif /*__MAIN_H */
