/**
  ******************************************************************************
  * @file    BSP/Src/log.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the LCD Log firmware functions
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
#include "lcd_log.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD Log demo
  * @param  None
  * @retval None
  */
void Log_demo(void)
{
  uint8_t   i = 0;
  uint8_t   scroll_direction = 0;
  uint8_t   scroll_index = 0;

  /* Initialize LCD Log module */
  LCD_LOG_Init();

  /* Show Header and Footer texts */
  LCD_LOG_SetHeader((uint8_t *)"Log Example");

  /* Output User logs */
  for (i = 0; i < 10; i++)
  {
    LCD_UsrLog ("This is Line %d \n", i);
  }

  HAL_Delay(500);

  /* Clear Old logs */
  LCD_LOG_ClearTextZone();

  /* Output new user logs */
  for (i = 0; i < 30; i++)
  {
    LCD_UsrLog ("This is Line %d \n", i);
  }

  /* Check for joystick user input for scroll (back and forward) */
  while (1)
  {
    if (scroll_direction == 0)
    {
      LCD_LOG_ScrollBack();
      scroll_index++;
      if (scroll_index > 30)
      {
        scroll_direction = 1;
      }
    }
    else
    {
      LCD_LOG_ScrollForward();
      scroll_index--;
      if (scroll_index == 0)
      {
        scroll_direction = 0;
      }
    }

    if (CheckForUserInput() > 0)
    {
      return;
    }
    HAL_Delay (10);
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
