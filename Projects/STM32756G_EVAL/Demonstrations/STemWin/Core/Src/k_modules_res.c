/**
  ******************************************************************************
  * @file    k_modules_res.c
  * @author  MCD Application Team
  * @brief   This file contains the Hex dumps of the images available 
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

/* Includes ------------------------------------------------------------------*/  
#include "k_modules_res.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MODULES_RES
  * @brief Kernel modules resources 
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  draw a 3D rectangle
  * @param  x0: Upper left X-position
  * @param  y0: Upper left Y-position
  * @param  width: width  of the rectangle
  * @param  height: height  of the rectangle
  * @retval None.
  */
void DrawRect3D(int x0, int y0, int width, int height)
{
  GUI_SetColor(GUI_BLACK);
  GUI_DrawRect(x0, y0, (x0 + width), (y0 + height));

  GUI_DrawHLine((y0 + height - 1), (x0 + 1), (x0 + width - 1));
  GUI_DrawVLine((x0 + width - 1),  (y0 + 1), (y0 + height - 1));

  GUI_SetColor(GUI_WHITE);
  GUI_DrawVLine((x0 + 1), (y0 + 1), (y0 + height - 2));
  GUI_DrawHLine((y0 + 1), (x0 + 1), (x0 + width - 2));
}

/**
  * @}
  */

/**
  * @}
  */


