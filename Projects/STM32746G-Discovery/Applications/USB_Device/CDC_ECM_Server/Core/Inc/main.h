/**
  ******************************************************************************
  * @file    USB_Device/CDC_ECM_Server/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
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
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc_ecm.h"
#include "usbd_cdc_ecm_if.h"
#include "lwip/netif.h"
#include "stm32746g_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LED_TIMER_LONG  ((uint32_t)(0x3FFFC))
#define LED_TIMER_SHORT ((uint32_t)(LED_TIMER_LONG/2))

/* Exported macro ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t LEDTimer;
extern USBD_HandleTypeDef USBD_Device;
extern struct netif gnetif;
/* Exported functions ------------------------------------------------------- */
void Toggle_Leds(void);
void Error_Handler(void);
#endif /* __MAIN_H */

