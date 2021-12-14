/**
  ******************************************************************************
  * @file    menu_selector.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the menu_selector 
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
#ifndef __MENU_SELECTOR_H
#define __MENU_SELECTOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7508_discovery_lcd.h"
/* Include touch screen FT5336 component Driver */
#include "../Components/ft5336/ft5336.h"


/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define STEMWIN_DEMO_SELECTED         (uint8_t)1
#define TOUCHGFX_DEMO_SELECTED        (uint8_t)2

/* Exported functions --------------------------------------------------------*/
uint8_t Run_MenuSelector(void);
void Reset_MenuSelector_Ressource(void);
/**
  * @}
  */

/**
  * @}
  */




#endif /* __MENU_SELECTOR_H */
/**
  * @}
  */

