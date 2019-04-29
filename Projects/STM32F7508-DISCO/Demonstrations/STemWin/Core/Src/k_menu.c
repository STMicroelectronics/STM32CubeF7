/**
  ******************************************************************************
  * @file    k_menu.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel menu functions 
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
  
/* Includes ------------------------------------------------------------------*/

#include "main.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MENU
  * @brief Kernel menu routines
  * @{
  */

/* Private defines -----------------------------------------------------------*/
#define ID_ICONVIEW_MENU               (GUI_ID_USER + 0x00)
#define ID_FEATURES_FPU                (GUI_ID_USER + 0x44)
#define ID_FEATURES_IC                 (GUI_ID_USER + 0x45)
#define ID_FEATURES_DC                 (GUI_ID_USER + 0x46)
#define ID_FEATURES_CPU                (GUI_ID_USER + 0x47)
#define ID_FEATURES_FREQ               (GUI_ID_USER + 0x48)

ICONVIEW_Handle hIcon ;
WM_HWIN         hCPULoad;
extern GUI_CONST_STORAGE GUI_BITMAP bmF7Logo;

/**
  * @brief  Callback routine of desktop window.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) {
  
  uint32_t NCode, Id;
  static uint8_t sel;
  
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();

    /*H/W Information */
    GUI_SetColor(0x007C18CC);
    GUI_AA_FillRoundedRect(400, 31, 780, 238, 15);
    GUI_DrawBitmap(&bmF7Logo, 400, 47); 

    break;
  
  
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);     
    NCode = pMsg->Data.v;  
    
    switch (NCode) 
    {  
      
    case WM_NOTIFICATION_RELEASED:
      if (Id == ID_ICONVIEW_MENU)
      {
        sel = ST_AnimatedIconView_GetSel(pMsg->hWinSrc);
        
        if(sel < k_ModuleGetNumber())
        {
          ST_AnimatedIconView_SetSel(pMsg->hWinSrc, -1);
          if(module_prop[sel].in_use == 0)
          {
            module_prop[sel].in_use = 1;
            module_prop[sel].module->startup(pMsg->hWin, 0, 0);
          }
          else if(module_prop[sel].win_state == 1)
          {
            module_prop[sel].module->startup(pMsg->hWin, 0, 0);
          }          
        }
        else
        {
          WM_InvalidateWindow (pMsg->hWinSrc);
        }
      }
      break;

    case WM_NOTIFICATION_CHILD_DELETED:
        module_prop[sel].in_use = 0;
        module_prop[sel].win_state = 0;
        break;
      
    default:
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  set the GUI profile
  * @param  None.
  * @retval None.
  */
void k_PeriodicProcesses(void)
{
  char tmp[16];
  WM_HWIN   hItem; 
  static uint32_t InitTick = 0;
  
  hItem = WM_GetDialogItem(WM_GetDesktopWindowEx(0), ID_FEATURES_CPU);
  
  if(hItem)
  {
    if((WM_IsVisible(hItem)) && ((osKernelSysTick() - InitTick ) > 500))
    {
      InitTick = osKernelSysTick();
      
      sprintf((char *)tmp , "MCU Load : %d%%",  osGetCPUUsage());
      TEXT_SetText(hItem, tmp);
      
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);
    }
  }
}

/**
  * @brief  set the GUI profile
  * @param  None.
  * @retval None.
  */
void k_SetGuiProfile(void) 
{
  BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
  DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
  
  FRAMEWIN_SetDefaultTextColor(0, GUI_WHITE);
  FRAMEWIN_SetDefaultTextColor(1, GUI_WHITE);      
  
  FRAMEWIN_SetDefaultBarColor(0, GUI_STCOLOR_LIGHTBLUE);
  FRAMEWIN_SetDefaultBarColor(1, GUI_STCOLOR_LIGHTBLUE);
  
  FRAMEWIN_SetDefaultFont(&GUI_FontAvantGarde20B);
  
  FRAMEWIN_SetDefaultClientColor(GUI_WHITE);  
  FRAMEWIN_SetDefaultTitleHeight(25);
  
  
  WINDOW_SetDefaultBkColor(GUI_WHITE);
  
  LISTVIEW_SetDefaultGridColor(GUI_WHITE);
  LISTVIEW_SetDefaultBkColor(LISTVIEW_CI_SEL, GUI_STCOLOR_LIGHTBLUE);
  DROPDOWN_SetDefaultColor(DROPDOWN_CI_SEL, GUI_STCOLOR_LIGHTBLUE);
  LISTVIEW_SetDefaultBkColor(LISTVIEW_CI_SELFOCUS, GUI_STCOLOR_LIGHTBLUE);
  DROPDOWN_SetDefaultColor(DROPDOWN_CI_SELFOCUS, GUI_STCOLOR_LIGHTBLUE);
  SCROLLBAR_SetDefaultWidth(8);
  SCROLLBAR_SetDefaultSTSkin();
  
  HEADER_SetDefaultBkColor(GUI_STCOLOR_LIGHTBLUE);
  HEADER_SetDefaultTextColor(GUI_WHITE);
  HEADER_SetDefaultFont(GUI_FONT_16_1);
  HEADER_SetDefaultSTSkin();
  SCROLLBAR_SetDefaultColor(GUI_STCOLOR_LIGHTBLUE, SCROLLBAR_CI_THUMB);  
  SCROLLBAR_SetDefaultColor(GUI_STCOLOR_LIGHTBLUE, SCROLLBAR_CI_SHAFT);  
  SCROLLBAR_SetDefaultColor(GUI_STCOLOR_LIGHTBLUE, SCROLLBAR_CI_ARROW);    
  ST_CHOOSEFILE_SetDelim('/');
  GUI_SetDefaultFont(GUI_FONT_20_ASCII);
  
  ST_CHOOSEFILE_SetButtonSize(40, 20);
    
  TEXT_SetDefaultTextColor(GUI_STCOLOR_LIGHTBLUE);
  TEXT_SetDefaultFont(&GUI_FontLubalGraph20);
}

/**
  * @brief  Initializes the menu.
  * @param  None.
  * @retval None.
  */
void k_InitMenu(void) 
{
  uint8_t i = 0;
  WM_HWIN   hItem;

  
  WM_SetCallback(WM_GetDesktopWindowEx(0), _cbBk);
  
  
  hIcon = ST_AnimatedIconView_CreateEx(0, 
                                       0, 
                                       LCD_GetXSize() - 80, 
                                       LCD_GetYSize() - 0, 
                                       WM_GetDesktopWindowEx(0), 
                                       WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_LATE_CLIP,
                                       0,
                                       ID_ICONVIEW_MENU, 
                                       100, 
                                       120, 40, 5);
  
  ST_AnimatedIconView_SetDualFont(hIcon, &GUI_FontLubalGraph16, &GUI_FontLubalGraph16);
  
  ST_AnimatedIconView_SetSpace(hIcon, GUI_COORD_Y, 0);
  ST_AnimatedIconView_SetSpace(hIcon, GUI_COORD_X, 0);
  ST_AnimatedIconView_SetFrame(hIcon, GUI_COORD_Y, 0);
  ST_AnimatedIconView_SetFrame(hIcon, GUI_COORD_X, 0);
  
  ST_AnimatedIconView_SetTextColor(hIcon, ICONVIEW_CI_UNSEL, GUI_STCOLOR_LIGHTBLUE);
  ST_AnimatedIconView_SetBkColor(hIcon, ICONVIEW_CI_UNSEL, GUI_WHITE);
  ST_AnimatedIconView_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_WHITE);
  ST_AnimatedIconView_SetSel(hIcon, -1);
  ST_AnimatedIconView_SetDualTextColor(hIcon, ICONVIEW_CI_SEL, GUI_STCOLOR_LIGHTBLUE, GUI_STCOLOR_DARKBLUE    );  
  
  for (i = 0; i < k_ModuleGetNumber(); i++)
  {   
    ST_AnimatedIconView_AddIcon(hIcon, module_prop[i].module->open_icon, module_prop[i].module->close_icon, (char *)module_prop[i].module->name);
  } 
  
  hItem = TEXT_CreateEx(407, 152, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_FPU, "FPU : ON");
  TEXT_SetFont(hItem, GUI_FONT_8_1);
  TEXT_SetTextColor(hItem, GUI_WHITE); 
  
  hItem = TEXT_CreateEx(407, 167, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_IC, "I-Cache : ON");
  TEXT_SetFont(hItem, GUI_FONT_8_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);     
  
  hItem = TEXT_CreateEx(407, 182, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_DC, "D-Cache : ON");
  TEXT_SetFont(hItem, GUI_FONT_8_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);  
  
  hItem = TEXT_CreateEx(407, 198, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_CPU, "MCU Load < 1%");
  TEXT_SetFont(hItem, GUI_FONT_8_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);  
  
  hItem = TEXT_CreateEx(407, 212, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_FREQ, "Clk : 200 MHz");
  TEXT_SetFont(hItem, GUI_FONT_8_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);    
  
    
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
