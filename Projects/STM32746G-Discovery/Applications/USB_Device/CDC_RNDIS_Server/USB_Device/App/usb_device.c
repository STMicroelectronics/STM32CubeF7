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
#include "usbd_cdc_rndis.h"
#include "usbd_cdc_rndis_if.h"

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
  (void) USBD_Init(&USBD_Device, &CDC_RNDIS_Desc, 0);

  /* Add Supported Class */
  (void) USBD_RegisterClass(&USBD_Device, USBD_CDC_RNDIS_CLASS);

  /* Add CDC Interface Class */
  (void) USBD_CDC_RNDIS_RegisterInterface(&USBD_Device, &USBD_CDC_RNDIS_fops);

  /* Start Device Process */
  (void) USBD_Start(&USBD_Device);
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
