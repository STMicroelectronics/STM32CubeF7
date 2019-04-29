/**
  ******************************************************************************
  * @file    Display/LCD_PicturesFromUSB/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7308_discovery.h"
#include "stm32f7308_discovery_lcd.h"
#include "fatfs_storage.h"
#include "lcd_log.h"
#include "usbh_msc.h"
#include "ff.h"
#include "usbh_core.h"
/* FatFs includes component */
#include "ff_gen_drv.h"
#include "usbh_diskio.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
  IMG_DEMO_START = 0,
  IMG_DEMO_FILE_OPERATIONS,
  IMG_DEMO_EXPLORER,
  IMG_DEMO_DISPLAY,
}IMG_Demo_State;

typedef struct _DemoStateMachine {
  __IO IMG_Demo_State state;
  __IO uint8_t        select; 
}IMG_DEMO_StateMachine;

typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_READY,    
  APPLICATION_DISCONNECT,
  APPLICATION_CONNECTING,
}IMG_ApplicationTypeDef;
/* Exported constants --------------------------------------------------------*/
#define INTERNAL_BUFFER_START_ADDRESS       0x60000000

extern USBH_HandleTypeDef hUSBHost;
extern FATFS USBH_fatfs;
extern IMG_ApplicationTypeDef Appli_state;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void IMG_File_Operations(void);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
