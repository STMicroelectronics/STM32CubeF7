/**
  ******************************************************************************
  * @file    BSP/Src/touchscreen.c
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

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  CIRCLE_RADIUS        30
/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       ((i * BSP_LCD_GetXSize()) / 5)
#define  CIRCLE_YPOS(i)       (BSP_LCD_GetYSize() - CIRCLE_RADIUS - 60)
/* Private variables ---------------------------------------------------------*/
static TS_StateTypeDef  TS_State;
/* Private function prototypes -----------------------------------------------*/
static void Touchscreen_SetHint(void);
static void Touchscreen_DrawBackground (uint8_t state);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Touchscreen Demo
  * @param  None
  * @retval None
  */
void Touchscreen_demo (void)
{
  uint8_t  status = 0;
  uint16_t x, y;
  uint8_t  state = 0;
  uint8_t  text[30];
  uint8_t  radius;
  uint8_t  radius_previous = 0;

  Touchscreen_SetHint();

  status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

  if (status != TS_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"Touchscreen cannot be initialized", CENTER_MODE);
  }
  else
  {
    Touchscreen_DrawBackground(state);
  }

  while (1)
  {
    if (status == TS_OK)
    {
      /* Check in polling mode in touch screen the touch status and coordinates */
      /* if touch occurred                                                      */
      BSP_TS_GetState(&TS_State);
      if(TS_State.touchDetected)
      {
        /* Get X and Y position of the touch post calibrated */
        x = TS_State.touchX[0];
        y = TS_State.touchY[0];

        BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        sprintf((char*)text, "Nb touch detected = %d", TS_State.touchDetected);
        BSP_LCD_DisplayStringAt(15, BSP_LCD_GetYSize() - 40, (uint8_t *)&text, LEFT_MODE);

        /* Display 1st touch detected coordinates */
        sprintf((char*)text, "1[%d,%d]    ", x, y);
        BSP_LCD_DisplayStringAt(15,
                                BSP_LCD_GetYSize() - 25,
                                (uint8_t *)&text,
                                LEFT_MODE);

        if (TS_State.touchDetected >= 2)  /* Display 2nd touch detected coordinates if applicable */
        {
          sprintf((char*)text, "2[%d,%d]    ", TS_State.touchX[1], TS_State.touchY[1]);
        }
        else
        {
          sprintf((char*)text, "              ");
        }
        BSP_LCD_DisplayStringAt((((BSP_LCD_GetXSize() - 30) * 1) / 5) + 15,
                                BSP_LCD_GetYSize() - 25,
                                (uint8_t *)&text,
                                LEFT_MODE);

        if (TS_State.touchDetected >= 3)  /* Display 3rd touch detected coordinates if applicable */
        {
          sprintf((char*)text, "3[%d,%d]    ", TS_State.touchX[2], TS_State.touchY[2]);
        }
        else
        {
          sprintf((char*)text, "              ");
        }
        BSP_LCD_DisplayStringAt((((BSP_LCD_GetXSize() - 30) * 2) / 5) + 15,
                                BSP_LCD_GetYSize() - 25,
                                (uint8_t *)&text,
                                LEFT_MODE);

        if (TS_State.touchDetected >= 4)  /* Display 4th touch detected coordinates if applicable */
        {
          sprintf((char*)text, "4[%d,%d]    ", TS_State.touchX[3], TS_State.touchY[3]);
        }
        else
        {
          sprintf((char*)text, "              ");
        }
        BSP_LCD_DisplayStringAt((((BSP_LCD_GetXSize() - 30) * 3) / 5) + 15,
                                BSP_LCD_GetYSize() - 25,
                                (uint8_t *)&text,
                                LEFT_MODE);

        if (TS_State.touchDetected >= 5)  /* Display 5th touch detected coordinates if applicable */
        {
          sprintf((char*)text, "5[%d,%d]    ", TS_State.touchX[4], TS_State.touchY[4]);
        }
        else
        {
          sprintf((char*)text, "              ");
        }
        BSP_LCD_DisplayStringAt((((BSP_LCD_GetXSize() - 30) * 4) / 5) + 15,
                                BSP_LCD_GetYSize() - 25,
                                (uint8_t *)&text,
                                LEFT_MODE);

        /* Calculate circle radius to fill according to finger pressure applied on screen (weight) */
        radius = TS_State.touchWeight[0]/3;
        if (radius > CIRCLE_RADIUS)
        {
          radius = CIRCLE_RADIUS;
        }
        else if (radius < 1)
        {
          radius = 1;
        }


        if ((y > (CIRCLE_YPOS(1) - CIRCLE_RADIUS)) &&
            (y < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
        {

          if ((x > (CIRCLE_XPOS(1) - CIRCLE_RADIUS)) &&
              (x < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
          {
            if ((radius != radius_previous) || (state != 1))
            {
              if (state != 1) /* Erase previous filled circle */
              {
                Touchscreen_DrawBackground(state);
              }
              BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
              BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), radius);
              radius_previous = radius;
              state = 1;
            }
          }
          if ((x > (CIRCLE_XPOS(2) - CIRCLE_RADIUS)) &&
              (x < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
          {
            if ((radius != radius_previous) || (state != 2))
            {
              if (state != 2) /* Erase previous filled circle */
              {
                Touchscreen_DrawBackground(state);
              }
              BSP_LCD_SetTextColor(LCD_COLOR_RED);
              BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), radius);
              radius_previous = radius;
              state = 2;
            }
          }

          if ((x > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) &&
              (x < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
          {
            if ((radius != radius_previous) || (state != 4))
            {
              if (state != 4) /* Erase previous filled circle */
              {
                Touchscreen_DrawBackground(state);
              }
              BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
              BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), radius);
              radius_previous = radius;
              state = 4;
            }
          }

          if ((x > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) &&
              (x < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
          {
            if ((radius != radius_previous) || (state != 8))
            {
              if (state != 8) /* Erase previous filled circle */
              {
                Touchscreen_DrawBackground(state);
              }
              BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
              BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), radius);
              radius_previous = radius;
              state = 8;
            }
          }
        }

      } /* of if(TS_State.touchDetected) */
    }

    if (CheckForUserInput() > 0)
    {
      return;
    }

    HAL_Delay(10);
  }
}

/**
  * @brief  Display TS Demo Hint
  * @param  None
  * @retval None
  */
static void Touchscreen_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Touchscreen Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the Touchscreen to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"fill the colored circles according to pressure applied", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Up to 5 finger touch coordinates are displayed", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);

}

/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
static void Touchscreen_DrawBackground (uint8_t state)
{

  switch (state)
  {

    case 0:
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);


      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);


      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);


      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS);

      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);
      BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2);
      break;

    case 1:
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);
      break;

    case 2:
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2);
      break;

    case 4:
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2);
      break;

    case 8:
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS - 2);
      break;

  }
}

/**
  * @}
  */

/**
  * @}
  */

