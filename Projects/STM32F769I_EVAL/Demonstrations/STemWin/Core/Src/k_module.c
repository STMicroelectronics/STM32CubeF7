/**
  ******************************************************************************
  * @file    k_module.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel module functions 
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
#include "k_module.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MODULE
  * @brief Kernel module routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmjumper1_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmjumper2_3;
extern K_ModuleItem_Typedef  vnc_server_board;

/* Private macros ------------------------------------------------------------*/
#define MAX_MODULES_NUM                                                       15
/* Private variables ---------------------------------------------------------*/
K_ModulePropertyTypedef    module_prop [MAX_MODULES_NUM];
K_ModuleOpenTypedef        open_link   [MAX_MODULES_NUM];
uint16_t module_num = 0;
uint16_t openlink_num = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Module Initialization.
  * @param  None.
  * @retval None.
  */
void k_ModuleInit(void) 
{
  module_num = 0;
  openlink_num = 0;
  memset(module_prop, 0, sizeof(module_prop));
  memset(open_link, 0, sizeof(module_prop));  
}

/**
  * @brief  Get the module number.
  * @param  None.
  * @retval None.
  */
uint8_t k_ModuleGetNumber(void) 
{
  return module_num;
}
/**
  * @brief  Add module.
  * @param  module: pointer to data structure of type K_ModuleItem_Typedef
  * @retval module add status
  */
uint8_t k_ModuleAdd(K_ModuleItem_Typedef *module) 
{
  module_prop[module_num].in_use = 0;
  module_prop[module_num].win_state = 0;
  module_prop[module_num].module = module;
  module_num++;
  return 0;
}

/**
  * @brief  return module index.
  * @param  module: pointer to data structure of type K_ModuleItem_Typedef
  * @retval module add status
  */
uint8_t k_ModuleGetIndex(K_ModuleItem_Typedef *module)
{
  uint8_t ModuleIdx;
  
  for (ModuleIdx = 0; ModuleIdx < module_num; ModuleIdx++)
  {
    if (module_prop[ModuleIdx].module->id == module->id)
    {
      return ModuleIdx;
    }
  }
  return 0;
}
/**
  * @brief  Link module to file browser open menu
  * @param  module: pointer to data structure of type K_ModuleItem_Typedef
  * @param  ext:    pointer to file extension.  
  * @retval None
  */

uint8_t k_ModuleOpenLink(K_ModuleItem_Typedef *module, char *ext) 
{
  if(openlink_num < (MAX_MODULES_NUM - 1))
  {
    open_link[openlink_num].module = module;
    strcpy(open_link[openlink_num].ext, ext);
    openlink_num++;
    return 0;
   }
  else
  {
    return 1;
  }
}

/**
  * @brief  Link module to file browser open menu
  * @param  ext: file extension
  * @retval pointer to module
  */

K_GET_DIRECT_OPEN_FUNC *k_ModuleCheckLink(char *ext) 
{
  uint8_t i   = 0;
  
  for (i = 0; i < MAX_MODULES_NUM; i++)
  {
    if(strcmp(open_link[i].ext, ext) == 0)
    {
      return (K_GET_DIRECT_OPEN_FUNC *)(open_link[i].module->DirectOpen);
    }
  }
  return NULL;
}

/**
  * @brief  Remove module
  * @param  module: pointer to data structure of type K_ModuleItem_Typedef
  * @retval None
  */

void k_ModuleRemove(K_ModuleItem_Typedef *module) 
{
  uint8_t idx = 0;
  
  for (idx  = 0; idx < MAX_MODULES_NUM; idx ++)
  {
    if(module_prop[module_num].module->id == module->id)
    {
      module_prop[module_num].module = NULL;
      module_num--;
    }
  }
}

/**
  * @brief callback Message Box 
  * @param  pMsg : pointer to data structure
  * @retval None
  */
static void k_cbMessageBox(WM_MESSAGE* pMsg) {
  WM_HWIN hWin;
  int Id, NCode;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_PAINT:
    
    /* Check on VNC server module */
    if(module_prop[k_ModuleGetIndex(&vnc_server_board)].in_use == 0)
    {
      GUI_DrawBitmap(&bmjumper1_2, 280, 80);
      GUI_DrawBitmap(&bmjumper1_2, 280, 130);
    }
    else
    {
      GUI_DrawBitmap(&bmjumper2_3, 280, 80);
      GUI_DrawBitmap(&bmjumper2_3, 280, 130);
    }
    break;    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    if ((Id == GUI_ID_OK) || (Id == GUI_ID_CANCEL)) {
      
      if (NCode == WM_NOTIFICATION_RELEASED) {
        Id = WM_GetId(pMsg->hWinSrc);
        GUI_EndDialog(hWin, (Id == GUI_ID_OK) ? 1 : 0);
      }
    }
    
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Paints cancel button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_cancel(BUTTON_Handle hObj) {

  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();

  
  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_SetColor(GUI_DARKGRAY);
    GUI_AA_FillCircle(87, 85, 85);
    GUI_SetBkColor(GUI_DARKGRAY);
    GUI_SetColor(GUI_WHITE);   
  }
  else
  {
    GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_AA_FillCircle(85, 85, 85);
    GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_SetColor(GUI_WHITE);    
  }  

  GUI_SetFont(&GUI_FontLubalGraph24B);
  GUI_DispStringAt("Cancel", 95, 47);
}

/**
  * @brief  Paints ok button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_ok(BUTTON_Handle hObj) {

  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();

  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_SetColor(GUI_DARKGRAY);
    GUI_AA_FillCircle(93, 85, 85);
    GUI_SetBkColor(GUI_DARKGRAY);
    GUI_SetColor(GUI_WHITE);   
  }
  else
  {
    GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_AA_FillCircle(93, 85, 85);
    GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_SetColor(GUI_WHITE);    
  } 

  GUI_SetFont(&GUI_FontLubalGraph24B);
  GUI_DispStringAt("OK", 37, 37);
}

/**
  * @brief  callback for cancel button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_cancel(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_cancel(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
  * @brief  callback for ok button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_ok(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_ok(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}
  
/**
  * @brief  Show Message Box
  * @param  pTitle: pointer to the title
  * @param  pText: pointer to the text
  * @retval int 
  */
int k_ExecCreatedDialog (WM_HWIN hDialog) 
{
  WM_DIALOG_STATUS DialogStatus = {0};
  /* Let window know how to send feedback (close info & return value) */
  GUI_SetDialogStatusPtr(hDialog, &DialogStatus);
  while (!DialogStatus.Done) {
    if (!GUI_Exec()) {
      GUI_Delay(10); /* Wait for event (keyboard, mouse or whatever) */
    }
  }
  return DialogStatus.ReturnValue;
}


/**
  * @brief  Show Message Box
  * @param  pTitle: pointer to the title
  * @param  pText: pointer to the text
  * @retval int 
  */ 
int k_ShowMessageBox(WM_HWIN hWin, const char* pTitle, const char* pText) 
{
  WM_HWIN hFrame, hClient, hBut, hItem;
  int r = 0;
  /* Create frame win */
  hFrame = FRAMEWIN_CreateEx(100, 65, 600, 350, hWin, WM_CF_SHOW, 0, 0, pTitle, &k_cbMessageBox);
  FRAMEWIN_SetClientColor   (hFrame, GUI_WHITE);
  FRAMEWIN_SetFont          (hFrame, &GUI_FontAvantGarde32);
  FRAMEWIN_SetTextAlign     (hFrame, GUI_TA_HCENTER);
  /* Create dialog items */
  hClient = WM_GetClientWindow(hFrame);
  
  hItem = TEXT_CreateEx(50, 10, 500, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER,  (GUI_ID_USER + 0x00), pText);
  TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
  
  hItem = TEXT_CreateEx(50, 40, 500, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER,  (GUI_ID_USER + 0x01), "pins. Please, verify the following jumpers positions");
  TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);    
  
  hItem = TEXT_CreateEx(230, 90, 50, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER,  (GUI_ID_USER + 0x02), "JP3");
  TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);  
  
  hItem = TEXT_CreateEx(230, 140, 50, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER,  (GUI_ID_USER + 0x03), "JP6");
  TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);    
  
  hItem = TEXT_CreateEx(50, 195, 500, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER,  (GUI_ID_USER + 0x04), "If the jumpers settings are wrong, press 'cancel',");
  TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);  
  
  hItem = TEXT_CreateEx(10, 225, 570, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER,  (GUI_ID_USER + 0x05), "set the jumpers correctly and reset");
  TEXT_SetFont(hItem, &GUI_FontLubalGraph24B); 
  
  hItem = TEXT_CreateEx(10, 255, 570, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER,  (GUI_ID_USER + 0x05), "the board. Otherwise, press 'OK' to continue");
  TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);   
  
  hBut = BUTTON_CreateEx(505, 235, 170, 170, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
  WM_SetCallback(hBut, _cbButton_ok);
  
  hBut = BUTTON_CreateEx((-95), 225, 170, 170, hClient, WM_CF_SHOW, 0, GUI_ID_CANCEL);
  WM_SetCallback(hBut, _cbButton_cancel);
 
  //WM_SetFocus(_hFrame);  
  WM_MakeModal(hFrame);
  r = k_ExecCreatedDialog(hFrame);  
  return r;
}

/**
  * @}
  */

/**
  * @}
  */

