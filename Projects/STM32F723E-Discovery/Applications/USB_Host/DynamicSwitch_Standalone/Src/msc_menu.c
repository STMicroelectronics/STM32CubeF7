/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Src/menu.c
  * @author  MCD Application Team
  * @brief   This file implements MSC Menu Functions
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

/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
MSC_DEMO_StateMachine msc_demo;
uint8_t *MSC_main_menu[] = {
  (uint8_t *)
    "      1 - File Operations                                                   ",
  (uint8_t *)
    "      2 - Explorer Disk                                                     ",
  (uint8_t *)
    "      3 - Re-Enumerate                                                      ",
};

/* Private function prototypes ----------------------------------------------- */
static void MSC_SelectItem(uint8_t ** menu, uint8_t item);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Manages MSC Menu Process.
  * @param  None
  * @retval None
  */
void MSC_MenuProcess(void)
{
  switch (msc_demo.state)
  {
  case MSC_DEMO_IDLE:
    MSC_SelectItem(MSC_main_menu, 0);
    msc_demo.state = MSC_DEMO_WAIT;
    msc_demo.select = 0;
    break;

  case MSC_DEMO_WAIT:
    if (msc_demo.select != 0)
    {
      MSC_SelectItem(MSC_main_menu, (msc_demo.select >> 4));

      /* Handle select item */
      switch (msc_demo.select)
      {
      case 0x0F:
        msc_demo.state = MSC_DEMO_FILE_OPERATIONS;
        break;

      case 0x1F:
        msc_demo.state = MSC_DEMO_EXPLORER;
        break;

      case 0x2F:
        msc_demo.state = MSC_REENUMERATE;
        break;

      default:
        break;
      }
      msc_demo.select = 0;
    }
    break;

  case MSC_DEMO_FILE_OPERATIONS:
    /* Read and Write File Here */
    if (Appli_state == APPLICATION_MSC)
    {
      MSC_File_Operations();
    }
    msc_demo.state = MSC_DEMO_WAIT;
    break;

  case MSC_DEMO_EXPLORER:
    /* Display disk content */
    if (Appli_state == APPLICATION_MSC)
    {
      /* Register the file system object to the FatFs module */
      if (f_mount(&USBH_fatfs, "", 0) != FR_OK)
      {
        LCD_ErrLog("Cannot Initialize FatFs! \n");
      }
      else
      {
        Explore_Disk("0:/", 1);
      }
    }
    msc_demo.state = MSC_DEMO_WAIT;
    break;

  case MSC_REENUMERATE:
    /* Force MSC Device to re-enumerate */
    USBH_ReEnumerate(&hUSBHost);
    msc_demo.state = MSC_DEMO_WAIT;

    break;

  default:
    break;
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
void MSC_DEMO_ProbeKey(uint16_t index)
{
  if ((index < 192) && (index > 60))
  {
    msc_demo.select = 0x0F;
  }
  else if ((index < 216) && (index > 192))
  {
    msc_demo.select = 0x1F;
  }
  else if (index > 216)
  {
    msc_demo.select = 0x2F;
  }
}

