/**
  ******************************************************************************
  * @file    Examples_MIX/DMA2D/DMA2D_MemToMemWithRBSwap/Inc/main.h 
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
#include "stm32f7xx_nucleo_144.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_dma2d.h"
#include "stm32_adafruit_lcd.h"


#define LAYER_SIZE_X      128
#define LAYER_SIZE_Y      160
#define LAYER_BYTE_PER_PIXEL           2 /* RGB565 format */
#define ORIGINAL_LAYER_BYTE_PER_PIXEL  4 /* ARGB8888 format */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OnError_Handler(uint32_t condition);

#endif /* __MAIN_H */

