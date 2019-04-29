/**
  ******************************************************************************
  * @file    DCMI/DCMI_SnapshotMode/Inc/main.h
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
#include "stm32756g_eval.h"
#include "stm32756g_eval_lcd.h"
#include "stm32756g_eval_sram.h"
#include "stm32756g_eval_camera.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* LCD Frame Buffer address */
#define CAMERA_FRAME_BUFFER               0xC0260000
#define LCD_FRAME_BUFFER                  0xC0130000

/* #define TEST_RESOLUTION_R160x120         */
/*          */#define TEST_RESOLUTION_R320x240
/* #define TEST_RESOLUTION_R480x272         */
/* The following resolusion is valid only on MB1063 */
/* #define TEST_RESOLUTION_R640x480         */       

#if defined(TEST_RESOLUTION_R160x120)
#define resolution      RESOLUTION_R160x120
#define xsize           160
#define ysize           120
#define xoffset         240
#define yoffset         180
#endif

#if defined(TEST_RESOLUTION_R320x240)
#define resolution      RESOLUTION_R320x240
#define xsize           320
#define ysize           240
#define xoffset         160
#define yoffset         120
#endif

#if defined(TEST_RESOLUTION_R480x272)
#define resolution      RESOLUTION_R480x272
#define xsize           480
#define ysize           272
#define xoffset         80
#define yoffset         104
#endif

#if defined(TEST_RESOLUTION_R640x480)
#define resolution      RESOLUTION_R640x480
#define xsize           640
#define ysize           480
#define xoffset         0
#define yoffset         0
#endif
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
