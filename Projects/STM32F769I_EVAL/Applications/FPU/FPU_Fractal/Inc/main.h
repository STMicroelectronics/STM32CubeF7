/**
  ******************************************************************************
  * @file    FPU/FPU_Fractal/Inc/main.h 
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
#include <stdio.h>
#include "stm32f769i_eval.h"
#include "stm32f769i_eval_lcd.h"
#include "stm32f769i_eval_io.h"
#include "stm32f769i_eval_sdram.h"
#include "stm32f769i_eval_ts.h"

#define ITERATION       ((uint32_t)256)
#define FRACTAL_ZOOM    ((uint16_t)200)
#define  REAL_CONSTANT  (0.285f)
#define  IMG_CONSTANT  (0.01f)

#define ARGB8888_BYTES_PER_PIXEL 4

#define LCD_FRAME_BUFFER      ((uint32_t)0xC0000000) /* LCD frame buffer */
#define FRACTAL_FRAME_BUFFER  ((uint32_t)0xC0200000) /* Julia Fractal frame buffer */


#define LCD_X_SIZE ((uint32_t) 800)
#define LCD_Y_SIZE ((uint32_t) 480)

#if (__FPU_USED == 1)
#define SCORE_FPU_MODE                "FPU On"
#else
#define SCORE_FPU_MODE                "FPU Off"
#endif  /* __FPU_USED */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
