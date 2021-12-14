/**
  ******************************************************************************
  * @file    k_modules_res.h
  * @author  MCD Application Team
  * @brief   Header for k_modules_res.c file
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
#ifndef __K_MODULE_RES_H
#define __K_MODULE_RES_H

#ifdef __cplusplus
 extern "C" {
#endif
 
/* Includes ------------------------------------------------------------------*/   
#include <stdlib.h>
#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

#ifndef GUI_FLASH
  #define GUI_FLASH
#endif    
extern GUI_FLASH const GUI_FONT GUI_FontITCAvantGardeStdBk20;    
extern GUI_FLASH const GUI_FONT GUI_FontITCAvantGardeStdBk32;  
extern GUI_FLASH const GUI_FONT GUI_FontLubalGraphXLtBT20;
extern GUI_FLASH const GUI_FONT GUI_FontLubalin32;

/* Exported types ------------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmstart;
extern GUI_CONST_STORAGE GUI_BITMAP bmstart_pressed;

extern GUI_CONST_STORAGE GUI_BITMAP bmexit;
extern GUI_CONST_STORAGE GUI_BITMAP bmexit_pressed;

extern GUI_CONST_STORAGE GUI_BITMAP bmselect;
extern GUI_CONST_STORAGE GUI_BITMAP bmopened;

extern GUI_CONST_STORAGE GUI_BITMAP bmplay_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmplay_not_pressed;


extern GUI_CONST_STORAGE GUI_BITMAP bmstop_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmstop_not_pressed;

extern GUI_CONST_STORAGE GUI_BITMAP bmpause_pressed;
extern GUI_CONST_STORAGE GUI_BITMAP bmpause_not_pressed;

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DrawRect3D(int x0, int y0, int width, int height);

#ifdef __cplusplus
}
#endif

#endif /*__K_MODULE_RES_H */

