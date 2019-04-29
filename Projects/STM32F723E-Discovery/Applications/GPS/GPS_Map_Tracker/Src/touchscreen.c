/**
  ******************************************************************************
  * @file    GPS/GPS_Map_Tracker/src/touchscreen.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the touchscreen driver.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup GPS_Map_Tracker
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/ 
/* Private Structures and Enumerations ---------------------------------------*/
/* Global variables ---------------------------------------------------------*/
TS_StateTypeDef  TS_State = {0};

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  TouchScreen get touch position
  * @param  None
  * @retval None
  */
uint8_t TouchScreen_GetTouchButtonPosition(void)
{
  uint16_t x1, y1;
  uint8_t buttonnum = 0;

        /* Check in polling mode in touch screen the touch status and coordinates */
        /* of touches if touch occurred                                           */
        BSP_TS_GetState(&TS_State);
        if(TS_State.touchDetected)
        {
          /* One or dual touch have been detected          */
          /* Only take into account the first touch so far */

          /* Get X and Y position of the first */          
          x1 = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
          y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);

      
         
          if (y1 > 200)
          {
            if ((x1 > 170) && (x1 < 240))
            {
    
              buttonnum = 1; /* freq+ */
            }
          }
          else
          {
            buttonnum = 0xFF;
          }
        } /* of if(TS_State.TouchDetected) */
   return buttonnum;
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
