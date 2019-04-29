/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/dual_menu.c
  * @author  MCD Application Team
  * @brief   Dual Core Process
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
uint8_t prev_select = 0;
DEMO_StateMachine demo;

/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/**
  * @brief  Menu Init
  * @param  None
  * @retval None
  */
void DUAL_MenuInit(void)
{
  /* Start HID Interface */
  LCD_UsrLogY("USB Host Full speed initialized.\n");
  /* Start MSC Interface */
  LCD_UsrLogY("USB Host High speed initialized.\n");
}

/**
  * @brief  Manages DUAL Menu Process.
  * @param  None
  * @retval None
  */
void DUAL_MenuProcess(void)
{
  switch (demo.state)
  {
  case DEMO_IDLE:
    LCD_LOG_ClearTextZone();
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    LCD_UsrLog("Select an operation to continue:\n");

    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAtLine(19,
                                (uint8_t *)
                                " 1 - Press User button to start HID demo");
    BSP_LCD_DisplayStringAtLine(20,
                                (uint8_t *)
                                " 2 - Press and hold User button to start MSC demo");

    /* Prevent debounce effect for user key */
    HAL_Delay(400);

    demo.state = DEMO_WAIT;
    break;

  case DEMO_WAIT:
    /* Check if User button is pressed */
    if (BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_SET)
    {
      /* To start MSC demo the User button should be kept pressed at least 2s */
      HAL_Delay(500);

      if (BSP_PB_GetState(BUTTON_TAMPER) == GPIO_PIN_SET)
      {
        demo.state = DEMO_MSC_START;
        while (BSP_PB_GetState(BUTTON_TAMPER) != RESET)
        {
        }
      }
      else
      {
        demo.state = DEMO_HID_START;
      }
      LCD_LOG_ClearTextZone();
    }
    break;

  case DEMO_MSC_START:
    if (Appli_HS_state == APPLICATION_HS_READY)
    {
      /* If HS device connected */
      demo.state = DEMO_MSC;
      demo.msc_state = APPLI_MSC_START;

      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      BSP_LCD_DisplayStringAtLine(4,
                                  (uint8_t *)
                                  "Mass Storage demo:                                                            ");
    }
    else
    {
      LCD_ErrLog("No Mass storage device connected.\n");
      demo.state = DEMO_WAIT;
    }
    break;

  case DEMO_HID_START:
    if (Appli_FS_state == APPLICATION_FS_READY)
    {
      /* if FS device connected */
      demo.state = DEMO_HID;
      demo.hid_state = APPLI_HID_START;

      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      BSP_LCD_DisplayStringAtLine(4,
                                  (uint8_t *)
                                  "Mass Storage demo:                                                            ");
    }
    else
    {
      LCD_ErrLog("No HID device connected.\n");
      demo.state = DEMO_WAIT;
    }
    break;

  case DEMO_MSC:
    MSC_MenuProcess();
    break;

  case DEMO_HID:
    HID_MenuProcess();
    break;
  default:
    break;
  }

  if (Appli_FS_state == APPLICATION_FS_DISCONNECT)
  {
    Appli_FS_state = APPLICATION_IDLE;
    LCD_LOG_ClearTextZone();
    LCD_ErrLog("USB device FS disconnected!\n");
    demo.state = DEMO_IDLE;
  }

  if (Appli_HS_state == APPLICATION_HS_DISCONNECT)
  {
    Appli_HS_state = APPLICATION_IDLE;
    LCD_LOG_ClearTextZone();
    LCD_ErrLog("USB device HS disconnected!\n");
    demo.state = DEMO_IDLE;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
