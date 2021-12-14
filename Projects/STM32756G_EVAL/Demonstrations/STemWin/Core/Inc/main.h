/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c file
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

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_it.h"

/* EVAL includes components */
#include "stm32756g_eval.h"
#include "stm32756g_eval_sdram.h"
#include "stm32756g_eval_ts.h"
#include "stm32756g_eval_io.h"
#include "stm32756g_eval_sd.h"
#include "stm32756g_eval_audio.h"
#include "stm32756g_eval_nor.h"
#include "stm32756g_eval_lcd.h"
   
/* FatFs includes components */
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "sd_diskio.h"
   
/* GUI includes components */
#include "GUI.h"
#include "DIALOG.h"         
#include "ST_GUI_Addons.h"   
#include "k_modules_res.h"
   
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define NUM_DISK_UNITS       2
#define USB_DISK_UNIT        0
#define MSD_DISK_UNIT        1

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
extern GUI_CONST_STORAGE GUI_BITMAP bmF7Logo;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

