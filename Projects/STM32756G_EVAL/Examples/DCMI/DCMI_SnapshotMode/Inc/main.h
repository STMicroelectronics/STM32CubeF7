/**
  ******************************************************************************
  * @file    DCMI/DCMI_SnapshotMode/Inc/main.h
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

