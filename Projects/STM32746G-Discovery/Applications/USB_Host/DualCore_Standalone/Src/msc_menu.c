/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/msc_menu.c
  * @author  MCD Application Team
  * @brief   Mass Storage Process
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

/* Includes ----------------------------------------------------------------- */
#include "main.h"

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */
/* Private function prototypes ---------------------------------------------- */
extern DEMO_StateMachine demo;

/* Private functions -------------------------------------------------------- */

/**
  * @brief  Manages MSC Menu Process.
  * @param  None
  * @retval None
  */
void MSC_MenuProcess(void)
{
  switch (demo.msc_state)
  {
  case APPLI_MSC_START:
    if (Appli_HS_state == APPLICATION_HS_READY)
    {
      BSP_LCD_ClearStringLine(19);
      BSP_LCD_ClearStringLine(20);

      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAtLine(19,
                                  (uint8_t *)
                                  "Press User button to start read and write operations");

      /* Wait for User Input */
      while ((BSP_PB_GetState(BUTTON_TAMPER) != SET) &&
             (Appli_HS_state != APPLICATION_HS_DISCONNECT))
      {
      }
      demo.msc_state = APPLI_MSC_FILE_OPERATIONS;

      /* Prevent debounce effect for user key */
      HAL_Delay(400);

      BSP_LCD_ClearStringLine(19);
    }
    break;

  case APPLI_MSC_FILE_OPERATIONS:

    /* Read and Write File Here */
    if (Appli_HS_state == APPLICATION_HS_READY)
    {
      MSC_File_Operations();

      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAtLine(19,
                                  (uint8_t *)
                                  "Press User button to display disk content");

      /* Wait for User Input */
      while ((BSP_PB_GetState(BUTTON_TAMPER) != SET) &&
             (Appli_HS_state != APPLICATION_HS_DISCONNECT))
      {
      }
      demo.msc_state = APPLI_MSC_EXPLORER;

      /* Prevent debounce effect for user key */
      HAL_Delay(400);

      BSP_LCD_ClearStringLine(19);
    }
    break;

  case APPLI_MSC_EXPLORER:
    /* Display disk content */
    if (Appli_HS_state == APPLICATION_HS_READY)
    {
      Explore_Disk("0:/", 1);
      demo.msc_state = APPLI_MSC_START;
      demo.state = DEMO_IDLE;

      BSP_LCD_ClearStringLine(19);
      BSP_LCD_ClearStringLine(20);
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAtLine(20, (uint8_t *) "Press User button to exit");

      /* Prevent debounce effect for user key */
      HAL_Delay(400);

      /* Wait for User Input */
      while ((BSP_PB_GetState(BUTTON_TAMPER) != SET) &&
             (Appli_HS_state != APPLICATION_HS_DISCONNECT))
      {
      }

      /* Prevent debounce effect for user key */
      HAL_Delay(400);
    }
    break;

  default:
    break;
  }
}

