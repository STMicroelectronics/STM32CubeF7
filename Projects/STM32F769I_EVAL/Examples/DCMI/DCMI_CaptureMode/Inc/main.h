/**
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f769i_eval.h"
#include "stm32f769i_eval_lcd.h"
#include "stm32f769i_eval_sdram.h"
#include "stm32f769i_eval_camera.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* LCD Frame Buffer address */

#define LCD_FRAME_BUFFER                  0xC0000000 /* LCD Frame buffer of size 800x480 in ARGB8888 */

#define CAMERA_FRAME_BUFFER               0xC0177000
#define LCD_FRAME_BUFFER                  0xC0000000

/* #define TEST_RESOLUTION_R160x120         */
/*          */#define TEST_RESOLUTION_R320x240
/* #define TEST_RESOLUTION_R480x272         */
/* #define TEST_RESOLUTION_R640x480         */

#if defined(TEST_RESOLUTION_R160x120)
#define RESOLUTION      RESOLUTION_R160x120
#define XSIZE           160
#define YSIZE           120
#endif

#if defined(TEST_RESOLUTION_R320x240)
#define RESOLUTION      RESOLUTION_R320x240
#define XSIZE           320
#define YSIZE           240
#endif

#if defined(TEST_RESOLUTION_R480x272)
#define RESOLUTION      RESOLUTION_R480x272
#define XSIZE           480
#define YSIZE           272
#endif

#if defined(TEST_RESOLUTION_R640x480)
#define RESOLUTION      RESOLUTION_R640x480
#define XSIZE           640
#define YSIZE           480
#endif
#define XOFFSET         ((BSP_LCD_GetXSize() - XSIZE)/2)
#define YOFFSET         ((BSP_LCD_GetYSize() - YSIZE)/2)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
