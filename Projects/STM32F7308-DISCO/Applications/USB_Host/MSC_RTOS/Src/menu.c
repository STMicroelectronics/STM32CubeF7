/**
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/Src/menu.c
  * @author  MCD Application Team
  * @brief   This file implements Menu Functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
#define MENU_UPDATE_EVENT            0x10

/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
MSC_DEMO_StateMachine msc_demo;
osSemaphoreId MenuEvent;
uint8_t *MSC_main_menu[] = {
  (uint8_t *)
    "      1 - File Operations                                                   ",
  (uint8_t *)
    "      2 - Explorer Disk                                                     ",
  (uint8_t *)
    "      3 - Re-Enumerate                                                      ",
};
TS_StateTypeDef TS_State = { 0 };

/* Private function prototypes ----------------------------------------------- */
static void MSC_SelectItem(uint8_t ** menu, uint8_t item);
static void MSC_DEMO_ProbeKey(void);
static void MSC_MenuThread(void const *argument);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void Menu_Init(void)
{
  USBH_UsrLog("Starting MSC Demo");

  /* Create Menu Semaphore */
  osSemaphoreDef(osSem);

  MenuEvent = osSemaphoreCreate(osSemaphore(osSem), 1);

  /* Force menu to show Item 0 by default */
  osSemaphoreRelease(MenuEvent);

  /* Menu task */
  osThreadDef(Menu_Thread, MSC_MenuThread, osPriorityHigh, 0,
              8 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Menu_Thread), NULL);
}

/**
  * @brief  User task
  * @param  pvParameters not used
  * @retval None
  */
static void MSC_MenuThread(void const *argument)
{
  for (;;)
  {
    if (osSemaphoreWait(MenuEvent, osWaitForever) == osOK)
    {
      switch (msc_demo.state)
      {
      case MSC_DEMO_IDLE:
        MSC_SelectItem(MSC_main_menu, 0);
        msc_demo.state = MSC_DEMO_WAIT;
        msc_demo.select = 0;
        osSemaphoreRelease(MenuEvent);
        break;

      case MSC_DEMO_WAIT:
        MSC_SelectItem(MSC_main_menu, (msc_demo.select >> 4));

        switch (msc_demo.select)
        {
        case 0x0F:
          msc_demo.state = MSC_DEMO_FILE_OPERATIONS;
          osSemaphoreRelease(MenuEvent);
          break;

        case 0x1F:
          msc_demo.state = MSC_DEMO_EXPLORER;
          osSemaphoreRelease(MenuEvent);
          break;

        case 0x2F:
          msc_demo.state = MSC_REENUMERATE;
          osSemaphoreRelease(MenuEvent);
          break;

        default:
          break;
        }
        break;

      case MSC_DEMO_FILE_OPERATIONS:
        /* Read and Write File Here */
        if (Appli_state == APPLICATION_READY)
        {
          MSC_File_Operations();
        }
        msc_demo.state = MSC_DEMO_WAIT;
        msc_demo.select = 0;
        break;

      case MSC_DEMO_EXPLORER:
        /* Display disk content */
        if (Appli_state == APPLICATION_READY)
        {
          Explore_Disk("0:/", 1);
        }
        msc_demo.state = MSC_DEMO_WAIT;
        msc_demo.select = 0;
        break;

      case MSC_REENUMERATE:
        /* Force MSC Device to re-enumerate */
        USBH_ReEnumerate(&hUSBHost);
        msc_demo.state = MSC_DEMO_WAIT;
        msc_demo.select = 0;
        break;

      default:
        break;
      }
    }
  }
}


/**
  * @brief  Manages the menu on the screen.
  * @param  menu: Menu table
  * @param  item: Selected item to be highlighted
  * @retval None
  */
static void MSC_SelectItem(uint8_t ** menu, uint8_t item)
{
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  switch (item)
  {
  case 0:
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(14, menu[0]);
    BSP_LCD_DisplayStringAtLine(15,
                                (uint8_t *)
                                "                                   ");
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(16, menu[1]);
    BSP_LCD_DisplayStringAtLine(17,
                                (uint8_t *)
                                "                                   ");
    BSP_LCD_DisplayStringAtLine(18, menu[2]);
    BSP_LCD_DisplayStringAtLine(19,
                                (uint8_t *)
                                "                                   ");
    break;

  case 1:
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(14, menu[0]);
    BSP_LCD_DisplayStringAtLine(15,
                                (uint8_t *)
                                "                                   ");
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(16, menu[1]);
    BSP_LCD_DisplayStringAtLine(17,
                                (uint8_t *)
                                "                                   ");
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(18, menu[2]);
    BSP_LCD_DisplayStringAtLine(19,
                                (uint8_t *)
                                "                                   ");
    break;

  case 2:
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(14, menu[0]);
    BSP_LCD_DisplayStringAtLine(15,
                                (uint8_t *)
                                "                                   ");
    BSP_LCD_DisplayStringAtLine(16, menu[1]);
    BSP_LCD_DisplayStringAtLine(17,
                                (uint8_t *)
                                "                                   ");
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(18, menu[2]);
    BSP_LCD_DisplayStringAtLine(19,
                                (uint8_t *)
                                "                                   ");
    break;
  }
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
}

/**
  * @brief  Probes the MSC joystick state.
  * @param  state: Joystick state
  * @retval None
  */
static void MSC_DEMO_ProbeKey(void)
{
  uint16_t y1;
  BSP_TS_GetState(&TS_State);
  if (TS_State.touchDetected)
  {
    /* Get Y position of the first touch post calibrated */
    y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);

    if ((y1 < 192) && (y1 > 60))
    {
      msc_demo.select = 0x0F;
    }
    else if ((y1 < 216) && (y1 > 192))
    {
      msc_demo.select = 0x1F;
    }
    else if (y1 > 216)
    {
      msc_demo.select = 0x2F;
    }
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  MSC_DEMO_ProbeKey();
  osSemaphoreRelease(MenuEvent);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
