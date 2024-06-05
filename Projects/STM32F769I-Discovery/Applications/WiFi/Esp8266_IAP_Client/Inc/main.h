/**
  ******************************************************************************
  * @file    Applications\WiFi\ESP8266_IAP_Client\Inc\config.h
  * @author  MCD Application Team
  * @brief   Header with config parameters
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

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#ifdef USE_LCD
 #include "stm32f769i_discovery_lcd.h"
#endif /* USE_LCD */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Edit the defines below to suit the WiFi connection
   and the webserver to use. -------------------------------------------------*/

/* Access point connection parameters */
#define WIFI_SSID       "AndroidAP"
#define WIFI_PASSWORD   "welcome0"
#define HOST_ADDRESS    "192.168.43.1"
#define HOST_PORT       8080

/* Binary file path name relative to server root directory */
#define FW_FILE         "hello.bin"
/* Maximum size of the downloaded binary file 
   Please Make sure not to exceed 330KB */
#define DATA_MAX_SIZE   (70 * 1024)

/* Maximum number of trials for WiFi connection */
#define MAX_NUM_TRIAL   10

/* Flash Memory Location parameters 
   Please make sure not to overwrite the WiFi example code area */
#define USER_FLASH_FIRST_PAGE_ADDRESS    0x08020000
#define USER_FLASH_SECOND_PAGE_ADDRESS   0x08040000

#define USER_FLASH_LAST_PAGE_ADDRESS     0x081C0000
#define USER_FLASH_END_ADDRESS           0x081FFFFF

/* This section can be used to tailor USARTx instance used and associated
   resources */
#define USARTx                           UART5
#define USARTx_CLK_ENABLE()              __UART5_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __UART5_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __UART5_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_12
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF8_UART5
#define USARTx_RX_PIN                    GPIO_PIN_2
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF8_UART5

/* Definition for USARTx's NVIC IRQ and IRQ Handlers */
#define USARTx_IRQn                      UART5_IRQn
#define USARTx_IRQHandler                UART5_IRQHandler

/* WiFi module Reset pin definitions */
#define ESP8266_RST_GPIO_PORT            GPIOJ
#define ESP8266_RST_PIN                  GPIO_PIN_14
#define ESP8266_RST_GPIO_CLK_ENABLE()    __GPIOJ_CLK_ENABLE()

#ifdef USE_LCD
#define LCD_BG_LAYER_ADDRESS             LCD_FB_START_ADDRESS
#endif /* USE_LCD */

/* Exported macro ------------------------------------------------------------*/
#define ARRAY_SIZE(array)     ((int)(sizeof(array) / sizeof((array)[0])))

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */

