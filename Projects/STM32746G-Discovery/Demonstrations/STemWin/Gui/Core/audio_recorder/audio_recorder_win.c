/**
  ******************************************************************************
  * @file    audio_recorder_win.c
  * @author  MCD Application Team
  * @brief   audio_recorde functions
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "audio_recorder_res.c"
#include "audio_recorder_app.h"

/** @addtogroup AUDIO_RECOREDR_MODULE
  * @{
  */

/** @defgroup AUDIO_RECORDER
  * @brief audio recorder routines
  * @{
  */
  
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);


/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  audio_recorder_board =
{
  3,
  "audio recorder   ",
  open_recorder,
  0,
  Startup,
  NULL,
}
;

/* Private defines -----------------------------------------------------------*/
#define ID_MAIN_FRAME                   (GUI_ID_USER + 0x00)
#define ID_BUTTON_EXIT                  (GUI_ID_USER + 0x02)
                                        
#define ID_IMAGE_0                      (GUI_ID_USER + 0x03)
#define ID_RECORD_STOP                  (GUI_ID_USER + 0x04)
#define ID_RECORD_REC_CANCEL_PLAY       (GUI_ID_USER + 0x05)
#define ID_RECORD_PAUSE                 (GUI_ID_USER + 0x06)
#define ID_ELAPSED_TIME                 (GUI_ID_USER + 0x07)

#define ID_FILENAME                     (GUI_ID_USER + 0x09)
#define ID_FILENAMECAPTION              (GUI_ID_USER + 0x0A)

#define ID_PLAY                         (GUI_ID_USER + 0x0C)
#define ID_EQUAL                        (GUI_ID_USER + 0x0D)
#define ID_PROCESS_TIMER                (GUI_ID_USER + 0x0E)

#define RECORDER_MODE_REC_DISABLED            0x00  
#define RECORDER_MODE_REC_IDLE                0x01  
#define RECORDER_MODE_RECORDING               0x02 
#define RECORDER_MODE_PLAY_IDLE               0x03
#define RECORDER_MODE_PLAYING                 0x04
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "", ID_MAIN_FRAME, 0, 0, 480, 272, 0, 0x64, 0 }, 
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 33, 180, 574, 51, 0, 0, 0 },
  { TEXT_CreateIndirect, "00:00", ID_ELAPSED_TIME, 270, 235, 80, 20, 0, 0x0, 0 },  
  { TEXT_CreateIndirect, "Press", ID_FILENAMECAPTION, 200, 30, 200, 19, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "[Record] to start", ID_FILENAME, 245, 30, 200, 19, 0, 0x0, 0 },  
  { IMAGE_CreateIndirect, "Image", ID_EQUAL, 200, 75, 175, 60, 0, 0, 0 }, 
};

static WM_HWIN               hMainWin;
extern K_ModuleItem_Typedef  audio_player_board;
static uint8_t               RecorderMode; 
static char                  FileName[256]; 
RNG_HandleTypeDef            hRNG;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Paints exit button
  * @param  hObj: button handle
  * @retval None
  */
static void _BuildFileName(void) {

  hRNG.Instance = RNG;
  __HAL_RCC_RNG_CLK_ENABLE();
  HAL_RNG_Init(&hRNG);
  sprintf(FileName, "0:/Record/Record_%04lu.wav", HAL_RNG_GetRandomNumber(&hRNG) % 10000);
}
/**
  * @brief  Paints exit button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_exit(BUTTON_Handle hObj) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_AA_FillCircle(60, 0, 60);

  GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_FontLubalGraph16B);
  GUI_DispStringAt("Menu", 15, 13);
	
}


/**
  * @brief  Paints Stop button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_stop(BUTTON_Handle hObj) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  if((RecorderMode == RECORDER_MODE_REC_IDLE)||
     (RecorderMode == RECORDER_MODE_REC_DISABLED))
  {
    GUI_DrawBitmap(&record_bmstop_des, 0, 0);
  }
  else 
  {
    if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
      GUI_DrawBitmap(&record_bmstop_pressed, 0, 0);
    } 
    else
    {
      GUI_DrawBitmap(&record_bmstop, 0, 0); 
    }
  }  
}

/**
  * @brief  Paints pause button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_pause(BUTTON_Handle hObj) {

 
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  if((RecorderMode == RECORDER_MODE_REC_IDLE)||
     (RecorderMode == RECORDER_MODE_PLAY_IDLE)||
     (RecorderMode == RECORDER_MODE_REC_DISABLED))    
  {
    GUI_DrawBitmap(&record_bmpause_des, 0, 0);
  }
  else 
  {
    if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
      GUI_DrawBitmap(&record_bmpause_pressed, 0, 0);
    }
    else
    {
      
      if((AUDIO_RECORDER_GetState() == AUDIO_RECORDER_PLAY_PAUSE)||
         (AUDIO_RECORDER_GetState() == AUDIO_RECORDER_RECORD_PAUSE))
      {
        GUI_DrawBitmap(&record_bmpause_pressed, 0, 0);
      }
      else
      {
        GUI_DrawBitmap(&record_bmpause, 0, 0);
      }
    }   
  }
}

/**
  * @brief  Paints record button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_record(BUTTON_Handle hObj) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  if(RecorderMode == RECORDER_MODE_REC_DISABLED)   
  {
    GUI_DrawBitmap(&bmrecord_disabled, 0, 0); 
  }
  else if(RecorderMode == RECORDER_MODE_PLAYING)   
  {

    GUI_DrawBitmap(&record_bmplay_pressed, 0, 0); 
  }
  else if(RecorderMode == RECORDER_MODE_REC_IDLE)
  {
    if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
      GUI_DrawBitmap(&bmrecord_pressed, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmrecord, 0, 0);
    }
  }
  else if(RecorderMode == RECORDER_MODE_PLAY_IDLE)
  {
    if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
      GUI_DrawBitmap(&record_bmplay_pressed, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&record_bmplay, 0, 0);
    }
  }
  else if(RecorderMode == RECORDER_MODE_RECORDING)
  {
    if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
      GUI_DrawBitmap(&bmcancel, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmcancel_unpressed, 0, 0);
    }
  }
}  
 
/**
  * @brief  callback for Exit button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_exit(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_exit(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for stop button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_stop(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_stop(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for speaker button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_pause(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_pause(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for record button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_record(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_record(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback function of the media connection status
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMediaConnection(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  static WM_HTIMER      hStatusTimer;  
  static uint8_t        prev_usb_status = 0;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    prev_usb_status = k_StorageGetStatus(USB_DISK_UNIT);    
    hStatusTimer = WM_CreateTimer(pMsg->hWin, 0, 500, 0); 
    
    break;
    
  case WM_TIMER:
    if(prev_usb_status != k_StorageGetStatus(USB_DISK_UNIT))
    {
      prev_usb_status = k_StorageGetStatus(USB_DISK_UNIT);
      if(prev_usb_status == 0)
      {
        if( RecorderMode == RECORDER_MODE_RECORDING)
        {
          AUDIO_RECORDER_StopRec();
        }
        
        else if( RecorderMode == RECORDER_MODE_PLAYING)
        {
          AUDIO_RECORDER_StopPlayer();
        }
        
        hItem = WM_GetDialogItem(hMainWin, ID_FILENAMECAPTION);
        TEXT_SetText(hItem, ">>>");   
        hItem = WM_GetDialogItem(hMainWin, ID_FILENAME);
        TEXT_SetText(hItem, "USB NOT ready");
        
        RecorderMode = RECORDER_MODE_REC_DISABLED;
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_STOP);
        WM_DisableWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_REC_CANCEL_PLAY);
        WM_DisableWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_PAUSE);
        WM_DisableWindow(hItem); 
        WM_Update(hItem);
      }
      else
      {
        
        hItem = WM_GetDialogItem(hMainWin, ID_FILENAMECAPTION);
        TEXT_SetText(hItem, "Press");   
        hItem = WM_GetDialogItem(hMainWin, ID_FILENAME);
        TEXT_SetText(hItem, "[Record] to start");
        
        RecorderMode = RECORDER_MODE_REC_IDLE;
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_STOP);
        WM_EnableWindow(hItem);
        WM_Update(hItem);        
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_REC_CANCEL_PLAY);
        WM_EnableWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_PAUSE);
        WM_EnableWindow(hItem); 
        WM_Update(hItem);        
      }
    }
    WM_RestartTimer(pMsg->Data.v, 500);
    break;
    
  case WM_DELETE:
    if(hStatusTimer != 0)
    {
      WM_DeleteTimer(hStatusTimer);
      hStatusTimer = 0;
    }
    break;   
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  callback for audio process
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAudioProcess(WM_MESSAGE * pMsg) {

  uint32_t Id;
  static WM_HTIMER hProcessTimer;
  WM_HWIN hItem;
  char tmp[] = "00:00/00:00";  
  uint32_t duration;
  
  switch (pMsg->MsgId) 
  {

  case WM_CREATE:
    hProcessTimer = WM_CreateTimer(pMsg->hWin, ID_PROCESS_TIMER, 1000, 0);

    break;
    
  case WM_TIMER:
    
    Id = WM_GetTimerId(pMsg->Data.v);
   
    if(Id == ID_PROCESS_TIMER)
    {
      if(AUDIO_RECORDER_GetState() == AUDIO_RECORDER_RECORDING)
      {
        /*Set elapsed time */
        duration = AUDIO_RECORDER_GetElapsedTime();     
        sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
        
        hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, tmp);  
      } 
      
      if(AUDIO_RECORDER_GetState() == AUDIO_RECORDER_PLAYING)
      {
        /*Set elapsed time */
        duration = AUDIO_RECORDER_GetTotalTime() - AUDIO_RECORDER_GetPlayedTime();
        sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60);
        hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, tmp);  
      }         
      WM_RestartTimer(pMsg->Data.v, 1000);
    }
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hProcessTimer);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}



/**
  * @brief  Notify State Change
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
void _cbNotifyStateChange (void) 
{
  WM_HWIN hItem;  
  if(AUDIO_RECORDER_GetState() == AUDIO_RECORDER_SUSPENDED)
  {
    if(RecorderMode == RECORDER_MODE_PLAYING)
    {
      RecorderMode = RECORDER_MODE_PLAY_IDLE;
      hItem = WM_GetDialogItem(hMainWin, ID_RECORD_REC_CANCEL_PLAY);
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);        
    }
    
    if(RecorderMode == RECORDER_MODE_RECORDING)
    {
      RecorderMode = RECORDER_MODE_REC_IDLE;
    }

    hItem = WM_GetDialogItem(hMainWin, ID_EQUAL);
    IMAGE_SetBitmap(hItem, &bmframe0);
    
    WM_InvalidateWindow(hItem);
    WM_Update(hItem);        
    
    if(hMainWin != 0)
    {
      hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);        
      TEXT_SetText(hItem, "00:00"); 
      WM_Update(hItem);
    }
  } 
}
      
/**
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
    
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
         
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUAL);
    IMAGE_SetBitmap(hItem, &bmframe0);
    
    hItem = BUTTON_CreateEx(420, 0, 60, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(335, 185, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_RECORD_PAUSE);
    WM_SetCallback(hItem, _cbButton_pause);     
    
    hItem = BUTTON_CreateEx(200, 185, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_RECORD_STOP);
    WM_SetCallback(hItem, _cbButton_stop);    
    
    hItem = BUTTON_CreateEx(270, 185, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_RECORD_REC_CANCEL_PLAY);
    WM_SetCallback(hItem, _cbButton_record); 
    
    if((module_prop[k_ModuleGetIndex(&audio_player_board)].in_use) ||
      (k_StorageGetStatus(USB_DISK_UNIT) == 0))
    { 
      
      if(module_prop[k_ModuleGetIndex(&audio_player_board)].in_use)
      {
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_FILENAMECAPTION);
        TEXT_SetText(hItem, ">>>");   
        hItem = WM_GetDialogItem(pMsg->hWin, ID_FILENAME);
        TEXT_SetText(hItem, "Audio IF busy");
      }
      
      if (k_StorageGetStatus(USB_DISK_UNIT) == 0)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_FILENAMECAPTION);
        TEXT_SetText(hItem, ">>>");   
        hItem = WM_GetDialogItem(pMsg->hWin, ID_FILENAME);
        TEXT_SetText(hItem, "USB NOT ready");
      }

      RecorderMode = RECORDER_MODE_REC_DISABLED;
        
      hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_STOP);
      WM_DisableWindow(hItem);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_REC_CANCEL_PLAY);
      WM_DisableWindow(hItem);    
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_PAUSE);
      WM_DisableWindow(hItem);         
        
    }
    else
    {
      RecorderMode = RECORDER_MODE_REC_IDLE;
      WM_CreateWindowAsChild(0,
                             100,
                             10,
                             10,
                             pMsg->hWin, 
                             WM_CF_SHOW | WM_CF_HASTRANS,
                             _cbAudioProcess, 
                             0);
    }
    
    
    WM_CreateWindowAsChild(479, 252, 1, 1, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMediaConnection , 0); 

    break;     
    
  case WM_PAINT:
    GUI_DrawBitmap(&bmrecorder, 7, 40);
    
    GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);    
    GUI_SetPenSize( 5 );
    GUI_DrawRect(220, 233, 304, 257);

    GUI_AA_FillEllipse(285, 222, 114, 70);
    GUI_SetColor(GUI_WHITE);
    GUI_AA_FillEllipse(285, 224, 114, 70);    

    break;     

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    
    switch(Id) {
    case ID_BUTTON_EXIT: 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        if( RecorderMode == RECORDER_MODE_RECORDING)
        {
          AUDIO_RECORDER_StopRec();
          RecorderMode = RECORDER_MODE_PLAY_IDLE;
        }
        else if( RecorderMode == RECORDER_MODE_PLAYING)
        {
          AUDIO_RECORDER_StopPlayer();
          RecorderMode = RECORDER_MODE_PLAY_IDLE;
        }
        break;
      case WM_NOTIFICATION_RELEASED:
        
        if(module_prop[k_ModuleGetIndex(&audio_player_board)].in_use == 0)
        {          
          AUDIO_RECORDER_DeInit();
        }        
        GUI_EndDialog(pMsg->hWin, 0);
        
        break;
      }
      break;
      
    case ID_RECORD_PAUSE: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        
          if((AUDIO_RECORDER_GetState() == AUDIO_RECORDER_PLAY_PAUSE)||
             (AUDIO_RECORDER_GetState() == AUDIO_RECORDER_RECORD_PAUSE))
          {
            AUDIO_RECORDER_PauseResume();
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUAL);
            IMAGE_SetGIF(hItem, equal, sizeof(equal));
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);    
          }
          else if((AUDIO_RECORDER_GetState() == AUDIO_RECORDER_PLAYING)||
             (AUDIO_RECORDER_GetState() == AUDIO_RECORDER_RECORDING))
          {
            AUDIO_RECORDER_PauseResume();
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUAL);
            IMAGE_SetBitmap(hItem, &bmframe0);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);    
          }

        break;
      }
      break;

    case ID_RECORD_STOP: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        if( RecorderMode == RECORDER_MODE_RECORDING)
        {
          AUDIO_RECORDER_StopRec();
          RecorderMode = RECORDER_MODE_PLAY_IDLE;
        }
        
        else if( RecorderMode == RECORDER_MODE_PLAYING)
        {
          AUDIO_RECORDER_StopPlayer();
          RecorderMode = RECORDER_MODE_PLAY_IDLE;
        }
        
        else if( RecorderMode == RECORDER_MODE_PLAY_IDLE)
        {
          RecorderMode = RECORDER_MODE_REC_IDLE;
        }
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_REC_CANCEL_PLAY);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);     

        hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_STOP);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);     
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_PAUSE);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);    
        break;
      }
      break;

    case ID_RECORD_REC_CANCEL_PLAY: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        if( RecorderMode == RECORDER_MODE_REC_IDLE) /* Start Recored */
        {
          _BuildFileName();
          
          hItem = WM_GetDialogItem(hMainWin, ID_FILENAMECAPTION);
          TEXT_SetText(hItem, "File : ");   
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);             
          hItem = WM_GetDialogItem(hMainWin, ID_FILENAME);
          TEXT_SetText(hItem, FileName);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);   
          
          AUDIO_RECORDER_SelectFile(FileName, FA_CREATE_ALWAYS | FA_WRITE);
          AUDIO_RECORDER_StartRec(DEFAULT_AUDIO_IN_FREQ);
          RecorderMode = RECORDER_MODE_RECORDING;
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUAL);
          IMAGE_SetGIF(hItem, equal, sizeof(equal));
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);              
        }
        
        else if( RecorderMode == RECORDER_MODE_RECORDING) /* Cancel */
        {
          AUDIO_RECORDER_StopRec();
          RecorderMode = RECORDER_MODE_REC_IDLE; 
          AUDIO_RECORDER_RemoveAudioFile(FileName);
         }
        
        else if( RecorderMode == RECORDER_MODE_PLAY_IDLE) /* Start Play */
        {
          RecorderMode = RECORDER_MODE_PLAYING;
          AUDIO_RECORDER_SelectFile(FileName, FA_OPEN_EXISTING | FA_READ);          
          AUDIO_RECORDER_Play(DEFAULT_AUDIO_IN_FREQ);
          
                   
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUAL);
          IMAGE_SetGIF(hItem, equal, sizeof(equal));
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);            
        }
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_REC_CANCEL_PLAY);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);     

        hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_STOP);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);     
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_PAUSE);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);          
        
        break;
      }
      break;
      
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }   
}


/**
  * @brief  audio recorder window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  /*Initialize audio Interface */
  if(module_prop[k_ModuleGetIndex(&audio_player_board)].in_use == 0)
  {        
    AUDIO_RECORDER_Init(50);
  }
  hMainWin = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
}

/**
  * @}
  */

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
