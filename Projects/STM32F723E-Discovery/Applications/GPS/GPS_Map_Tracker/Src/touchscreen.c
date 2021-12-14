/**
  ******************************************************************************
  * @file    GPS/GPS_Map_Tracker/src/touchscreen.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the touchscreen driver.
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

