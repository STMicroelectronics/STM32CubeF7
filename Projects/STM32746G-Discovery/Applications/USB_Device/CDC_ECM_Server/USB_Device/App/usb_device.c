/**
  ******************************************************************************
  * @file    USB_Device/CDC_ECM_Standalone/USB_Device/App/usb_device.c
  * @author  MCD Application Team
  * @brief   This file implements the USB Device
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc_ecm.h"
#include "usbd_cdc_ecm_if.h"

/* Private variables ---------------------------------------------------------*/

extern USBD_HandleTypeDef USBD_Device;
/* Private function prototypes -----------------------------------------------*/

extern void Error_Handler(void);
/* USB Device Core handle declaration. */

/**
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_Device_Init(void)
{

  /* Init Device Library */
  if (USBD_Init(&USBD_Device, &CDC_ECM_Desc, 0) != USBD_OK)
  {
    Error_Handler();
  }

  /* Add Supported Class */
  if (USBD_RegisterClass(&USBD_Device, USBD_CDC_ECM_CLASS) != USBD_OK)
  {
    Error_Handler();
  }

  /* Add CDC Interface Class */
  if (USBD_CDC_ECM_RegisterInterface(&USBD_Device, &USBD_CDC_ECM_fops) != (uint8_t) USBD_OK)
  {
    Error_Handler();
  }

  /* Start Device Process */
  if (USBD_Start(&USBD_Device) != USBD_OK)
  {
    Error_Handler();
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
