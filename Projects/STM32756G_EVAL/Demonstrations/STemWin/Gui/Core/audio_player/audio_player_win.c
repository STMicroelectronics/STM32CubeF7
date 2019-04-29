/**
  ******************************************************************************
  * @file    audioplayer_win.c
  * @author  MCD Application Team
  * @brief   Audio player functions
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
#include "DIALOG.h"
#include "k_module.h"
#include "audio_player_app.h"
#include "audio_player_res.c"
#include "k_modules_res.h"
#include "k_storage.h"
#include "k_rtc.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_PLAYER
  * @brief audio player routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void FILEMGR_GetFileOnly (char *file, char *path);
/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  audio_player_board =
{
  1,
  "audio player",
  open_audio,
  0,
  Startup,
  NULL,
};
typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t repeat         : 2;
    uint32_t pause          : 1;
    uint32_t play           : 1;
    uint32_t stop           : 1;    
    uint32_t mute           : 1;
    uint32_t volume         : 8;   
    uint32_t reserved       : 18;
  }b;
}
AudioSettingsTypeDef;
/* Private defines -----------------------------------------------------------*/

#define REPEAT_NONE            0
#define REPEAT_ONCE            1
#define REPEAT_ALL             2


#define PLAY_INACTIVE                 0x00
#define PLAY_ACTIVE                   0x01

#define PAUSE_INACTIVE                0x00
#define PAUSE_ACTIVE                  0x01

#define STOP_INACTIVE                 0x00  
#define STOP_ACTIVE                   0x01  


#define ID_PROCESS_TIMER       1
#define ID_PLAYLIST_TIMER      2

#define ID_FRAMEWIN_INFO      (GUI_ID_USER + 0x01)

/* Play List */
#define ID_WAVFILE_LIST       (GUI_ID_USER + 0x10)

/* Button */
#define ID_MUTE_BUTTON            (GUI_ID_USER + 0x20)
#define ID_PREVIOUS_BUTTON        (GUI_ID_USER + 0x21)
#define ID_PLAY_BUTTON            (GUI_ID_USER + 0x22)
#define ID_NEXT_BUTTON            (GUI_ID_USER + 0x23)
#define ID_STOP_BUTTON            (GUI_ID_USER + 0x24)
#define ID_PAUSE_BUTTON           (GUI_ID_USER + 0x25)
#define ID_WAV_BUTTON             (GUI_ID_USER + 0x26)
#define ID_PLAY_LIST_BUTTON       (GUI_ID_USER + 0x27)
#define ID_ADD_BUTTON             (GUI_ID_USER + 0x28)
#define ID_EQUAL_BUTTON           (GUI_ID_USER + 0x29)
#define ID_EXIT_BUTTON            (GUI_ID_USER + 0x2A)
#define ID_REPEAT_ALL_BUTTON      (GUI_ID_USER + 0x2B)
#define ID_REPEAT_ONE_BUTTON      (GUI_ID_USER + 0x2C)
#define ID_REPEAT_OFF_BUTTON      (GUI_ID_USER + 0x2D)
#define ID_EXIT_EQUAL_BUTTON      (GUI_ID_USER + 0x2E)


/* Audio information */
#define ID_TITLE              (GUI_ID_USER + 0x40)
#define ID_ELAPSED_TIME       (GUI_ID_USER + 0x41)
#define ID_TOTAL_TIME         (GUI_ID_USER + 0x42)

/* Text */
#define ID_TEXT_ADD          (GUI_ID_USER + 0x50)
#define ID_TEXT_FILE         (GUI_ID_USER + 0x51)
#define ID_TEXT_OPEN         (GUI_ID_USER + 0x52)
#define ID_TEXT_FOLDER       (GUI_ID_USER + 0x53) 

#define ID_TEXT_WARNING      (GUI_ID_USER + 0x54)

#define ID_SLIDER_VOLUME      (GUI_ID_USER + 0x55)
#define ID_SLIDER_DURATION    (GUI_ID_USER + 0x56)

#define ID_EQUABAR_1           (GUI_ID_USER + 0x57)
#define ID_EQUABAR_2           (GUI_ID_USER + 0x58)
#define ID_EQUABAR_3           (GUI_ID_USER + 0x59)
#define ID_EQUABAR_4           (GUI_ID_USER + 0x5A)
#define ID_EQUABAR_5           (GUI_ID_USER + 0x5B)
#define ID_EQUABAR_6           (GUI_ID_USER + 0x5C)
#define ID_LOUDBAR_1           (GUI_ID_USER + 0x5D)

#define ID_EQUABAR_TEXT_1      (GUI_ID_USER + 0x60)
#define ID_EQUABAR_TEXT_2      (GUI_ID_USER + 0x61)
#define ID_EQUABAR_TEXT_3      (GUI_ID_USER + 0x62)
#define ID_EQUABAR_TEXT_4      (GUI_ID_USER + 0x63)
#define ID_EQUABAR_TEXT_5      (GUI_ID_USER + 0x64)
#define ID_EQUABAR_TEXT_6      (GUI_ID_USER + 0x65)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect,    "Audio Player", ID_FRAMEWIN_INFO,     1, 0, 640, 480, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect,  "Listview",     ID_WAVFILE_LIST,      230, 103, 305, 260, 0, 0x0,  0 },  
  { TEXT_CreateIndirect,      "Title",        ID_TITLE,     220,  60,  330,  40,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "00:00",         ID_ELAPSED_TIME,        515,  398,  35,  16,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "Add",           ID_TEXT_ADD,            170, 310, 33, 20,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "file",          ID_TEXT_FILE,           170, 333, 29, 20,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "Add",          ID_TEXT_OPEN,           60, 310, 45, 30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "folder",        ID_TEXT_FOLDER,         60, 333, 50, 30,  0, 0x0,  0 },  
};

static WM_HWIN hMainWin; 
static WM_HTIMER hProcessTimer;
static WM_HTIMER hPlaylistTimer;
static WM_HWIN  hTimerWin;
static WM_HWIN  hEquWin = 0;

static WAV_InfoTypedef         WavInfo;
static  FILELIST_FileTypeDef   WavList;
static  CHOOSEFILE_INFO       FileInfo;
 
static char const            *apDrives[1] = {"0:"};
static const char            acMask_audio[] = ".audio";
static const char            acMask_dir[] = ".dir";
static uint16_t              file_pos = 0; 
static AudioSettingsTypeDef  PlayerSettings;
static uint8_t               playlist_select = 0;
              
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Paints Play button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_play(BUTTON_Handle hObj, uint32_t play_status) {
  int Index = 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    if(PlayerSettings.b.play == PLAY_INACTIVE)
    {
      GUI_DrawBitmap(&audio_bmplay_pressed, 0, 0);
    }
    else   
    {
      GUI_DrawBitmap(&audio_bmplay, 0, 0);
    }  
  }
  else
  {
    if(PlayerSettings.b.play == PLAY_ACTIVE)
    {
      GUI_DrawBitmap(&audio_bmplay_pressed, 0, 0);
    }
    else   
    {
      GUI_DrawBitmap(&audio_bmplay, 0, 0);
    } 
  } 
}

/**
  * @brief  Paints Stop button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_stop(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&audio_bmstop_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&audio_bmstop, 0, 0);
  }
}

/**
  * @brief  Paints Previous button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_previous(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&audio_bmbackward_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&audio_bmbackward, 0, 0);
  }
}

/**
  * @brief  Paints Next button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_next(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&audio_bmforeward_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&audio_bmforeward, 0, 0);
  }
}

/**
  * @brief  Paints Add to play list button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_add(BUTTON_Handle hObj) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  GUI_DrawBitmap(&bmopen, 0, 0);
}

/**
  * @brief  Paints Open button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_open(BUTTON_Handle hObj) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
    
  GUI_DrawBitmap(&bmadd, 0, 5);
}

/**
  * @brief  Paints pause button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_pause(BUTTON_Handle hObj, uint32_t play_status) {

  int Index = 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    if(PlayerSettings.b.pause == PAUSE_INACTIVE)
    {
      GUI_DrawBitmap(&audio_bmpause_pressed, 0, 0);
    }
    else   
    {
      GUI_DrawBitmap(&audio_bmpause, 0, 0);
    }  
  }
  else
  {
    if(PlayerSettings.b.pause == PAUSE_ACTIVE)
    {
      GUI_DrawBitmap(&audio_bmpause_pressed, 0, 0);
    }
    else   
    {
      GUI_DrawBitmap(&audio_bmpause, 0, 0);
    } 
  }   
}

/**
  * @brief  Paints equal button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_equal(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
    
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&audio_bmequal_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&audio_bmequal, 0, 0);
  }
}

/**
  * @brief  Paints repeat button
  * @param  repeat_status: repeat button status
  * @retval None
  */
static void _OnPaint_repeat_one(BUTTON_Handle hObj, uint32_t repeat_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  GUI_DrawBitmap(&audio_bmrepeat_one, 0, 0);
  
  if(PlayerSettings.b.repeat == REPEAT_ONCE)
  {
    GUI_DrawBitmap(&audio_bmrepeat_one_pressed, 0, 0);
  }
  else   
  {
    GUI_DrawBitmap(&audio_bmrepeat_one, 0, 0);
  }
}

/**
  * @brief  Paints repeat button
  * @param  repeat_status: repeat button status
  * @retval None
  */
static void _OnPaint_repeat_all(BUTTON_Handle hObj, uint32_t repeat_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  GUI_DrawBitmap(&audio_bmrepeat_all, 0, 0);
  
  if(PlayerSettings.b.repeat == REPEAT_ALL)
  {
    GUI_DrawBitmap(&audio_bmrepeat_all_pressed, 0, 0);
  }
  else   
  {
    GUI_DrawBitmap(&audio_bmrepeat_all, 0, 0);
  }  
}

/**
  * @brief  Paints repeat button
  * @param  repeat_status: repeat button status
  * @retval None
  */
static void _OnPaint_repeat_off(BUTTON_Handle hObj, uint32_t repeat_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  GUI_DrawBitmap(&audio_bmrepeat_off_pressed, 0, 0);
  
  if(PlayerSettings.b.repeat == REPEAT_NONE)
  {
    GUI_DrawBitmap(&audio_bmrepeat_off_pressed, 0, 0);
  }
  else   
  {
    GUI_DrawBitmap(&audio_bmrepeat_off, 0, 0);
  }
}
/**
  * @brief  Paints speaker button
  * @param  speaker_status: speaker button status
  * @retval None
  */
static void _OnPaint_speaker(BUTTON_Handle hObj, uint32_t speaker_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  if(PlayerSettings.b.mute == MUTE_OFF)
  {
    GUI_DrawBitmap(&audio_bmspeaker, 0, 0);    
  }
  else
  {
    GUI_DrawBitmap(&audio_bmspeaker1, 0, 0);
  }  
}

/**
  * @brief  Paints exit button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_close(BUTTON_Handle hObj) {

  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();

  
  GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_AA_FillCircle(55, 0, 55);

  GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font20_1);
  GUI_DispStringAt("Menu", 12, 10);
}

#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
/**
  * @brief  Paints exit button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_close_equal(BUTTON_Handle hObj) {

  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();

  
  GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_AA_FillCircle(55, 0, 55);

  GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font20_1);
  GUI_DispStringAt("Back", 15, 10);
}
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */

/**
  * @brief  callback for play button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbButton_play(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_play(pMsg->hWin, PlayerSettings.b.pause);
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
  * @brief  callback for next button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_next(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_next(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for previous button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_previous(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_previous(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for add to play list button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_add(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_add(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for open button 
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_open(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_open(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg); 
      break;
  }
}

/**
  * @brief  callback for close button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_close(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_close(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
/**
  * @brief  callback for close equal frame button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_close_equal(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_close_equal(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */

/**
  * @brief  callback for repeat one button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_repeat_one(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_repeat_one(pMsg->hWin, PlayerSettings.b.repeat);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for repeat all button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_repeat_all(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_repeat_all(pMsg->hWin, PlayerSettings.b.repeat);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for repeat off button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_repeat_off(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_repeat_off(pMsg->hWin, PlayerSettings.b.repeat);
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
static void _cbButton_speaker(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_speaker(pMsg->hWin, PlayerSettings.b.mute);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}


/**
  * @brief  callback for pause button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_pause(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_pause(pMsg->hWin, PlayerSettings.b.pause);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for equal button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_equal(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_equal(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}
/**
  * @brief  callback for audio process
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAudioProcess(WM_MESSAGE * pMsg) {

  uint32_t Id;
  
  WM_HWIN hItem;
  char tmp[12] = "00:00";
  long progress;
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
      if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
      {
        progress = AUDIOPLAYER_GetProgress();
        
        /*Set elapsed time */
        duration = progress / WavInfo.ByteRate;     
        sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
        
        hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, tmp);  
        
        /* Set progress slider position */
        hItem = WM_GetDialogItem(hMainWin, ID_SLIDER_DURATION);
        progress = (long)(progress/(WavInfo.FileSize/100));
        
        SLIDER_SetValue(hItem, progress);
        WM_Update(hItem);
        
        if(hEquWin)
        {
          hItem = WM_GetDialogItem(hEquWin, ID_ELAPSED_TIME);
          TEXT_SetText(hItem, tmp);  
          
          hItem = WM_GetDialogItem(hEquWin, ID_SLIDER_DURATION);
          SLIDER_SetValue(hItem, progress);
          WM_Update(hItem);        
        }
      }
      else if(AUDIOPLAYER_GetState() == AUDIOPLAYER_EOF)
      {
        /* Set progress slider position */
        hItem = WM_GetDialogItem(hMainWin, ID_SLIDER_DURATION);        
        SLIDER_SetValue(hItem, 100);
        WM_Update(hItem);
      }

      
      AUDIOPLAYER_Process();
      WM_RestartTimer(pMsg->Data.v, 1000);
    }
    else if (Id == ID_PLAYLIST_TIMER)
    {
      playlist_select = 0;
    }
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hProcessTimer);
    if(hPlaylistTimer)
    {
     WM_DeleteTimer(hPlaylistTimer); 
     hPlaylistTimer = 0;
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
static void _AddEntireFolder(char *Foldername, int update)
{
  FRESULT res;
  uint32_t duration;
  FILINFO fno;
  DIR dir;
  char *fn;
  static char tmp[FILEMGR_FILE_NAME_SIZE]; 
  WM_HWIN hItem;

  res = f_opendir(&dir, Foldername);
  
  if (res == FR_OK)
  {
    
    while (1)
    {
      res = f_readdir(&dir, &fno);
      
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }
      fn = fno.fname;

      if (WavList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if(((strstr(fn, ".wav")) || (strstr(fn, ".WAV"))))
          {
            
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            strncpy((char *)WavList.file[WavList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            
            if((update == 1) && (hMainWin != 0))
            {
              if(AUDIOPLAYER_GetFileInfo(tmp, &WavInfo) == 0)
              {
                
                hItem = WM_GetDialogItem(hMainWin, ID_WAVFILE_LIST);
                LISTVIEW_AddRow(hItem, NULL);  
                FILEMGR_GetFileOnly (tmp, fn);
                
                LISTVIEW_SetItemText(hItem, 0, WavList.ptr, fn);
                
                
                duration = WavInfo.FileSize / WavInfo.ByteRate; 
                sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
                LISTVIEW_SetItemText(hItem, 1, WavList.ptr, tmp);
                LISTVIEW_SetItemTextColor(hItem, 1, WavList.ptr, LISTVIEW_CI_UNSEL, GUI_STCOLOR_DARKBLUE);              
              }
            }
            WavList.ptr++;
          }
        }
      }   
    }
  }
  f_closedir(&dir);
}


/**
  * @brief  Play wav file.
  * @param  filename: pointer to file name.
  * @retval None
  */
static void _PlayFile(char *filename)
{
	uint32_t duration;
  static char tmp[FILEMGR_FILE_NAME_SIZE];  
  WM_HWIN hItem;
  
  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_STOP)
  {
    if(AUDIOPLAYER_GetFileInfo(filename, &WavInfo) == 0)
    {
      /* Title */
      FILEMGR_GetFileOnly (tmp, filename);
      TEXT_SetText(WM_GetDialogItem(hMainWin, ID_TITLE), tmp);  
      
      /* Total Time */
      duration = WavInfo.FileSize / WavInfo.ByteRate;     
      sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
      hItem = WM_GetDialogItem(hMainWin, ID_TOTAL_TIME);
      TEXT_SetText(hItem, tmp); 
      
      /* Open audio file */
      if(AUDIOPLAYER_SelectFile(filename) == 0)
      {
        /* start playing */
        AUDIOPLAYER_Play(WavInfo.SampleRate);
        if(PlayerSettings.b.mute == MUTE_ON)
        {
          AUDIOPLAYER_Mute(MUTE_ON);
        }
      }
    }
    else
    {
      TEXT_SetText(WM_GetDialogItem(hMainWin, ID_TITLE), "Unknown");  
      hItem = WM_GetDialogItem(hMainWin, ID_TOTAL_TIME);
      TEXT_SetText(hItem, "--:--");      
    }  
  }
}

/**
  * @brief  Notify the end of wav file.
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void)
{
  WM_HWIN hItem;
  
  AUDIOPLAYER_Stop();
  
  if(PlayerSettings.b.repeat != REPEAT_NONE)
  {
    if (PlayerSettings.b.repeat == REPEAT_ALL)
    {
      if(file_pos < (WavList.ptr - 1))
      {
        file_pos++;
        LISTVIEW_IncSel(WM_GetDialogItem(hMainWin, ID_WAVFILE_LIST)); 
      }
      else 
      {        
        file_pos = 0; 
        LISTVIEW_SetSel(WM_GetDialogItem(hMainWin, ID_WAVFILE_LIST), file_pos);
      }
    }
    
    _PlayFile((char *)WavList.file[file_pos].name);
  }
  else
  {
    PlayerSettings.b.play = PLAY_INACTIVE; 
    hItem = WM_GetDialogItem(hMainWin, ID_PLAY_BUTTON);
    WM_InvalidateWindow(hItem);
    WM_Update(hItem);
  }

  return AUDIOPLAYER_ERROR_NONE;
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
      if(WavList.file[file_pos].name[0] == '0')
      {
        
        PlayerSettings.b.play = PLAY_INACTIVE;
        PlayerSettings.b.pause = PAUSE_INACTIVE;
        
        WavList.ptr = 0;
        
        AUDIOPLAYER_Stop();
        hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, "00:00");            
        
        /* Title Initialization in play list */
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
        TEXT_SetText(hItem, "       No wav file selected");
        TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
        TEXT_SetTextColor(hItem, 0x00DCA939);        
        
        hItem = WM_GetDialogItem(hMainWin, ID_SLIDER_DURATION);
        SLIDER_SetValue(hItem, 0);  
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(hMainWin, ID_PLAY_BUTTON);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(hMainWin, ID_PAUSE_BUTTON);
        WM_InvalidateWindow(hItem);
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

#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
/**
  * @brief  Callback routine of equal dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbEqualDialog(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
  WM_HWIN hItem;
  char tmp[] = "00:00";    
  int volume, progress, duration;
  
  switch (pMsg->MsgId) {
    
  case WM_CREATE:
   
    progress = (long)(AUDIOPLAYER_GetProgress()/(WavInfo.FileSize/100));
        
    hItem = SLIDER_CreateEx(10, 400, 493, 12, pMsg->hWin, WM_CF_SHOW, 0, ID_SLIDER_DURATION);
    SLIDER_SetFocusColor(hItem, 0x00969696);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetValue(hItem, progress);
    SLIDER_SetWidth(hItem, 0);
    SLIDER_SetSTSkin(hItem);
    WM_DisableWindow(hItem);
    
    duration = AUDIOPLAYER_GetProgress() / WavInfo.ByteRate;     
    sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );    
    
    hItem = TEXT_CreateEx(515,  398,  35,  16, pMsg->hWin, WM_CF_SHOW, 0, ID_ELAPSED_TIME, tmp);
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextColor(hItem, 0x00969696);
    
    /* 1 Khz equ */
    hItem = TEXT_CreateEx(47, 49,  35,  16, pMsg->hWin, WM_CF_SHOW, 0, ID_EQUABAR_TEXT_1, "1 KHz");
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    
    hItem = SLIDER_CreateEx(50, 65, 30, 330, pMsg->hWin, WM_CF_SHOW, SLIDER_CF_VERTICAL, ID_EQUABAR_1);
    SLIDER_SetFocusColor(hItem, 0x00DCA939);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetValue(hItem, 100 - EQUI_DB_TO_PERC(k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU1_BKP)));
    SLIDER_SetWidth(hItem, 30);
    SLIDER_SetSTSkin(hItem); 
    
    /* 2 Khz equ */
    hItem = TEXT_CreateEx(117, 49,  35,  16, pMsg->hWin, WM_CF_SHOW, 0, ID_EQUABAR_TEXT_2, "2 KHz");
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    
    hItem = SLIDER_CreateEx(120, 65, 30, 330, pMsg->hWin, WM_CF_SHOW, SLIDER_CF_VERTICAL, ID_EQUABAR_2);
    SLIDER_SetFocusColor(hItem, 0x00DCA939);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetValue(hItem, 100 - EQUI_DB_TO_PERC(k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU2_BKP)));
    SLIDER_SetWidth(hItem, 30);
    SLIDER_SetSTSkin(hItem); 
    
    /* 5 Khz equ */
    hItem = TEXT_CreateEx(187, 49,  35,  16, pMsg->hWin, WM_CF_SHOW, 0, ID_EQUABAR_TEXT_3, "5 KHz");
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    
    hItem = SLIDER_CreateEx(190, 65, 30, 330, pMsg->hWin, WM_CF_SHOW, SLIDER_CF_VERTICAL, ID_EQUABAR_3);
    SLIDER_SetFocusColor(hItem, 0x00DCA939);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetValue(hItem, 100 - EQUI_DB_TO_PERC(k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU3_BKP)));
    SLIDER_SetWidth(hItem, 30);
    SLIDER_SetSTSkin(hItem); 
    
    
    /* 10 Khz equ */
    hItem = TEXT_CreateEx(255, 49,  50,  16, pMsg->hWin, WM_CF_SHOW, 0, ID_EQUABAR_TEXT_4, "10 KHz");
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    
    hItem = SLIDER_CreateEx(260, 65, 30, 330, pMsg->hWin, WM_CF_SHOW, SLIDER_CF_VERTICAL, ID_EQUABAR_4);
    SLIDER_SetFocusColor(hItem, 0x00DCA939);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetValue(hItem, 100 - EQUI_DB_TO_PERC(k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU4_BKP)));
    SLIDER_SetWidth(hItem, 30);
    SLIDER_SetSTSkin(hItem); 
    
    
    /* 15 Khz equ */
    hItem = TEXT_CreateEx(325, 49,  50,  16, pMsg->hWin, WM_CF_SHOW, 0, ID_EQUABAR_TEXT_5, "15 KHz");
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    
    hItem = SLIDER_CreateEx(330, 65, 30, 330, pMsg->hWin, WM_CF_SHOW, SLIDER_CF_VERTICAL, ID_EQUABAR_5);
    SLIDER_SetFocusColor(hItem, 0x00DCA939);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetValue(hItem, 100 - EQUI_DB_TO_PERC(k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU5_BKP)));
    SLIDER_SetWidth(hItem, 30);
    SLIDER_SetSTSkin(hItem); 
    
    /* Loudness */
    hItem = TEXT_CreateEx(455, 49,  100,  16, pMsg->hWin, WM_CF_SHOW, 0, ID_EQUABAR_TEXT_6, "Loudness");
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    
    hItem = SLIDER_CreateEx(470, 65, 30, 330, pMsg->hWin, WM_CF_SHOW, SLIDER_CF_VERTICAL, ID_LOUDBAR_1);
    SLIDER_SetFocusColor(hItem, 0x00DCA939);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetValue(hItem, 100 - k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_LOUD_BKP));
    SLIDER_SetWidth(hItem, 30);
    SLIDER_SetSTSkin(hItem);     
    
    
    hItem = BUTTON_CreateEx(580, 0, 60, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_EXIT_EQUAL_BUTTON);
    WM_SetCallback(hItem, _cbButton_close_equal);      
    break;
    
  case WM_PAINT:
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();    
    
    GUI_DrawBitmap(&audio_bmbackward_des, 7, 420);
    GUI_DrawBitmap(&audio_bmpause_des, 58, 420);
    GUI_DrawBitmap(&audio_bmplay_des, 112, 420);
    GUI_DrawBitmap(&audio_bmstop_des, 165, 420);
    GUI_DrawBitmap(&audio_bmforeward_des, 220, 420);
    
    GUI_DrawBitmap(&audio_bmrepeat_all_des, 310, 420);
    GUI_DrawBitmap(&audio_bmrepeat_one_des, 365, 420);
    GUI_DrawBitmap(&audio_bmrepeat_off_des, 420, 420);
    GUI_DrawBitmap(&audio_bmequal_pressed, 507, 420);
    
    GUI_SetColor(0x00969696);
    volume = AUDIOPLAYER_GetVolume();
    GUI_DrawRect(591, 65, 613, 395);    
    GUI_FillRect(592, 66 + 330 * (100 - volume)/100, 612, 394);
    
    if(PlayerSettings.b.mute == MUTE_OFF)
    {
      GUI_DrawBitmap(&audio_bmspeaker_des, 580, 420);       
    }
    else
    {
      GUI_DrawBitmap(&audio_bmspeaker_des1, 580, 420);    
    }
    
    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {      
      
    case ID_EQUABAR_1: 
      if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUABAR_1);
        AUDIO_SetEq(0, 100 - SLIDER_GetValue(hItem));
      }
      break;      
      
    case ID_EQUABAR_2: 
      if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUABAR_2);
        AUDIO_SetEq(1, 100 - SLIDER_GetValue(hItem));
      }
      break;    


    case ID_EQUABAR_3: 
      if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUABAR_3);
        AUDIO_SetEq(2, 100- SLIDER_GetValue(hItem));
      }
      break;    

    case ID_EQUABAR_4: 
      if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUABAR_4);
        AUDIO_SetEq(3, 100- SLIDER_GetValue(hItem));
      }
      break;    

    case ID_EQUABAR_5: 
      if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EQUABAR_5);
        AUDIO_SetEq(4, 100- SLIDER_GetValue(hItem));
      }
      break;          
      
    case ID_LOUDBAR_1: 
      if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LOUDBAR_1);
        AUDIO_SetLoudnessGain(100- SLIDER_GetValue(hItem));
      }
      break;     
      
      /* Notifications sent by 'Back' Button */
    case ID_EXIT_EQUAL_BUTTON:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        osDelay(100);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LOUDBAR_1);
        AUDIO_SetEqParams(100- SLIDER_GetValue(hItem));
        GUI_EndDialog(pMsg->hWin, 0);

        hEquWin = 0;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}
#else /* !AUDIO_USE_SPIRIT_EQUALIZER */
#if 0 /* Don't show any pop-up window */
/**
  * @brief  Callback routine of informing user about exploring the disk
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbHintEqNotAvailable(WM_MESSAGE * pMsg) 
{ 
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    GUI_SetBkColor(0x00DCA939);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font16_1HK);
    GUI_DispStringHCenterAt("Audio equalizer feature not supported", 120 , 10);
    GUI_DispStringHCenterAt("with GNU compiler ...", 120 , 30);
    GUI_SetFont(GUI_DEFAULT_FONT);
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;  
  }
}
#endif /* no pop-up window */
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */

/**
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMainDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id, ItemNbr, r;
  int index;
  uint32_t duration;
  static char tmp[40];  
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    /* Initialization of 'Listview' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
    LISTVIEW_AddColumn(hItem, 220, "Track", GUI_TA_VCENTER | GUI_TA_LEFT);
    LISTVIEW_AddColumn(hItem, 60, "Duration   ", GUI_TA_VCENTER | GUI_TA_RIGHT);
    LISTVIEW_SetGridVis(hItem, 0);
    LISTVIEW_SetAutoScrollV(hItem, 0);
    LISTVIEW_SetHeaderHeight(hItem, 0);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, GUI_WHITE);
    LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_UNSEL, 0x00DCA939);
    LISTVIEW_SetFont(hItem, GUI_FONT_20_ASCII);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SEL, 0x00DCA939);
    LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_SEL, GUI_WHITE);
    LISTVIEW_SetRowHeight(hItem, 30);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_SELFOCUS, 0x00DCA939);
    LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_SELFOCUS, GUI_WHITE);
    
    WIDGET_SetEffect(hItem, &WIDGET_Effect_None);

    ItemNbr = 0;
    if(WavList.ptr > 0)
    {
      while(ItemNbr < WavList.ptr)
      {
        if(AUDIOPLAYER_GetFileInfo((char *)WavList.file[ItemNbr].name, &WavInfo) == 0)
        {
          
          LISTVIEW_AddRow(hItem, NULL);  
          FILEMGR_GetFileOnly (tmp, (char *)WavList.file[ItemNbr].name);
          
          LISTVIEW_SetItemText(hItem, 0,ItemNbr, tmp);
          
          
          duration = WavInfo.FileSize / WavInfo.ByteRate; 
          sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
          LISTVIEW_SetItemText(hItem, 1, ItemNbr, tmp);
          LISTVIEW_SetItemTextColor(hItem, 1, ItemNbr, LISTVIEW_CI_UNSEL, GUI_STCOLOR_DARKBLUE);              
          
          ItemNbr++;
        }
      }
    }
            
    /* Title Initialization in play list */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
    TEXT_SetText(hItem, "       No wav file selected");
    TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);
        
    /* Duration */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_ELAPSED_TIME);
    TEXT_SetText(hItem, "00:00");
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);
    
    /* Duration */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TOTAL_TIME);
    TEXT_SetText(hItem, "00:00");
    TEXT_SetFont(hItem, GUI_FONT_16_ASCII);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ADD);
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FILE);
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_OPEN);   
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FOLDER);  
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);
    
    /* Initialization of 'Add' Button */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_ADD_BUTTON);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
           
    PlayerSettings.b.mute = MUTE_OFF;  
    PlayerSettings.b.pause = PAUSE_INACTIVE;
    PlayerSettings.b.play = PLAY_INACTIVE;
    PlayerSettings.b.stop = STOP_INACTIVE;    
  
    hItem = BUTTON_CreateEx(310, 420, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_REPEAT_ALL_BUTTON);
    WM_SetCallback(hItem, _cbButton_repeat_all);

    hItem = BUTTON_CreateEx(365, 420, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_REPEAT_ONE_BUTTON);
    WM_SetCallback(hItem, _cbButton_repeat_one);

    hItem = BUTTON_CreateEx(420, 420, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_REPEAT_OFF_BUTTON);
    WM_SetCallback(hItem, _cbButton_repeat_off);
    
    hItem = BUTTON_CreateEx(580, 420, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_MUTE_BUTTON);
    WM_SetCallback(hItem, _cbButton_speaker);    
          
    hItem = BUTTON_CreateEx(7, 420, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_PREVIOUS_BUTTON);
    WM_SetCallback(hItem, _cbButton_previous);   
    
    hItem = BUTTON_CreateEx(58, 420, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_PAUSE_BUTTON);
    WM_SetCallback(hItem, _cbButton_pause);     
    
    hItem = BUTTON_CreateEx(112, 420, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_BUTTON);
    WM_SetCallback(hItem, _cbButton_play);
    
    hItem = BUTTON_CreateEx(165, 420, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_STOP_BUTTON);
    WM_SetCallback(hItem, _cbButton_stop);     
    
    hItem = BUTTON_CreateEx(220, 420, 40, 40, pMsg->hWin, WM_CF_SHOW, 0, ID_NEXT_BUTTON);
    WM_SetCallback(hItem, _cbButton_next);
    
    hItem = WM_GetClientWindow(pMsg->hWin);
    hItem = BUTTON_CreateEx(10, 293, 50, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_WAV_BUTTON);
    WM_SetCallback(hItem, _cbButton_add); 
    
    hItem = BUTTON_CreateEx(118, 290, 50, 70, pMsg->hWin, WM_CF_SHOW, 0, ID_ADD_BUTTON);
    WM_SetCallback(hItem, _cbButton_open);      
       
    hItem = BUTTON_CreateEx(580, 0, 60, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_EXIT_BUTTON);
    WM_SetCallback(hItem, _cbButton_close);    

#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
    hItem = BUTTON_CreateEx(507, 420, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_EQUAL_BUTTON);
#else /* !AUDIO_USE_SPIRIT_EQUALIZER */
    /* Hide the Eq button */
    hItem = BUTTON_CreateEx(507, 420, 50, 50, pMsg->hWin, WM_CF_HIDE, 0, ID_EQUAL_BUTTON);
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
    WM_SetCallback(hItem, _cbButton_equal); 
    
    hTimerWin = WM_CreateWindowAsChild(0,
                           100,
                           10,
                           10,
                           pMsg->hWin, 
                           WM_CF_SHOW | WM_CF_HASTRANS,
                           _cbAudioProcess, 
                           0);
    
    WM_CreateWindowAsChild(639, 460, 1, 1, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMediaConnection , 0); 
    
    hItem = SLIDER_CreateEx(10, 400, 493, 12, pMsg->hWin, WM_CF_SHOW, 0, ID_SLIDER_DURATION);
    SLIDER_SetFocusColor(hItem, 0x00DCA939);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetValue(hItem, 0);
    SLIDER_SetWidth(hItem, 0);
    SLIDER_SetSTSkin(hItem); 
    
    hItem = SLIDER_CreateEx(590, 64, 25, 333, pMsg->hWin, WM_CF_SHOW, SLIDER_CF_VERTICAL, ID_SLIDER_VOLUME);
    SLIDER_SetFocusColor(hItem, GUI_STCOLOR_DARKBLUE);
    SLIDER_SetBkColor (hItem, GUI_WHITE);
    SLIDER_SetValue(hItem, 100 - PlayerSettings.b.volume);
    SLIDER_SetWidth(hItem, 0);
    SLIDER_SetSTSkin(hItem); 
    break;
    
  case WM_PAINT:
    GUI_DrawBitmap(&bmaudio_pochette, 7, 67);
    break;
    
  case WM_DELETE:
    AUDIOPLAYER_DeInit();   
    PlayerSettings.b.mute = MUTE_OFF;  
    PlayerSettings.b.pause = PAUSE_INACTIVE;
    PlayerSettings.b.play = PLAY_INACTIVE;
    PlayerSettings.b.stop = STOP_INACTIVE;       
    k_BkupSaveParameter(CALIBRATION_AUDIOPLAYER_SETTING_BKP, PlayerSettings.d32);
    
    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;

    switch(Id) {      
      
    /* Notifications sent by 'Repeat' Button */
    case ID_REPEAT_ALL_BUTTON:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        PlayerSettings.b.repeat = REPEAT_ALL;
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_ONE_BUTTON);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);  
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_OFF_BUTTON);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);  
      }
      break;
      
      /* Notifications sent by 'Repeat' Button */
    case ID_REPEAT_ONE_BUTTON:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        PlayerSettings.b.repeat = REPEAT_ONCE;
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_OFF_BUTTON);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);  
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_ALL_BUTTON);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);          
      }
      break;
      
      /* Notifications sent by 'Repeat' Button */
    case ID_REPEAT_OFF_BUTTON:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        PlayerSettings.b.repeat = REPEAT_NONE;
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_ALL_BUTTON);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);  
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_ONE_BUTTON);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);          
      }
      break;      
      
    /* Notifications sent by 'Mute' Button */
    case ID_MUTE_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_MUTE_BUTTON);         
        if(PlayerSettings.b.mute == MUTE_OFF)
        {          
          AUDIOPLAYER_Mute(MUTE_ON);
          PlayerSettings.b.mute = MUTE_ON;
        }
        else
        {         
          AUDIOPLAYER_Mute(MUTE_OFF);
          PlayerSettings.b.mute = MUTE_OFF;
        }
      }
      break;
      
      
    /* Notifications sent by 'Volume' Slider */
    case ID_SLIDER_VOLUME: 
      if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_VOLUME);
        AUDIOPLAYER_SetVolume(100 - SLIDER_GetValue(hItem));
        PlayerSettings.b.volume = AUDIOPLAYER_GetVolume();
        if(PlayerSettings.b.mute == MUTE_ON)
        {          
          AUDIOPLAYER_Mute(MUTE_OFF);
          PlayerSettings.b.mute = MUTE_OFF;
        }        
      }
      break;      
      
    /* Notifications sent by 'progress' Slider */
    case ID_SLIDER_DURATION: 
      if(NCode == WM_NOTIFICATION_CLICKED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
        AUDIOPLAYER_SetPosition(SLIDER_GetValue(hItem));
        
        if(PlayerSettings.b.mute == MUTE_ON)
        {
          AUDIOPLAYER_Mute(MUTE_ON);
        }
      }
      break;
      
      
    /* Notifications sent by 'ListView' */
    case ID_WAVFILE_LIST: 
      if(NCode == WM_NOTIFICATION_CLICKED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
        index = LISTVIEW_GetSel(hItem);
        
        if(index < WavList.ptr)
        {
          if(playlist_select == 0)
          {
            hPlaylistTimer = WM_CreateTimer(hTimerWin, ID_PLAYLIST_TIMER, 500, 0);           
            playlist_select = (index + 1);
          }
          
          else if(playlist_select == (index + 1))
          {
            WM_DeleteTimer(hPlaylistTimer); 
            hPlaylistTimer = 0;          
            playlist_select = 0;
            
            if(index < WavList.ptr)
            {
                
              if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
              {             
                AUDIOPLAYER_Stop();
              }
              
              PlayerSettings.b.pause = PLAY_ACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);
              
              file_pos = index;
              _PlayFile((char *)WavList.file[index].name); 
              
              PlayerSettings.b.play = PLAY_ACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem); 

              PlayerSettings.b.pause = PAUSE_INACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);
            }
          }
        }
      }
      break;
      
    /* Notifications sent by 'Play' Button */
    case ID_PLAY_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {        
        if(AUDIOPLAYER_GetState() == AUDIOPLAYER_STOP)
        {
          if(WavList.ptr > 0)
          {
            _PlayFile((char *)WavList.file[file_pos].name);
            PlayerSettings.b.play = PLAY_ACTIVE;
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);               
            LISTVIEW_SetSel(WM_GetDialogItem(hMainWin, ID_WAVFILE_LIST), file_pos);
          }
          else
          {
            FileInfo.pfGetData = k_GetData;
            FileInfo.pMask = acMask_audio;     
            hItem = ST_CHOOSEFILE_Create(pMsg->hWin,  95, 115, 450, 250, apDrives, GUI_COUNTOF(apDrives), 0, "Open an audio file", 0, &FileInfo); 
            WM_MakeModal(hItem);
            r = GUI_ExecCreatedDialog(hItem);
            if (r == 0) 
            {
              if((strstr(FileInfo.pRoot, ".wav")) || (strstr(FileInfo.pRoot, ".WAV")))
              {         
                if(AUDIOPLAYER_GetFileInfo(FileInfo.pRoot, &WavInfo) == 0)
                {
                  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
                  {
                    /* Stop current audio sample */
                    AUDIOPLAYER_Stop();   
                  }
                  
                  WavList.ptr = 0;
                  file_pos = 0;
                  
                  strcpy((char *)WavList.file[WavList.ptr].name, FileInfo.pRoot);
                  FILEMGR_GetFileOnly (tmp, FileInfo.pRoot);
                  hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
                  
                  /* Update Play list */
                  strcpy((char *)WavList.file[WavList.ptr].name, FileInfo.pRoot);
                  
                  ItemNbr = LISTVIEW_GetNumRows(hItem);
                  while(ItemNbr--)
                  {
                    LISTVIEW_DeleteRow(hItem, ItemNbr);
                  }
                  
                  LISTVIEW_AddRow(hItem, NULL);         
                  LISTVIEW_SetItemText(hItem, 0, WavList.ptr, tmp);
                  TEXT_SetText(WM_GetDialogItem(hMainWin, ID_TITLE), tmp); 
                  duration = WavInfo.FileSize / WavInfo.ByteRate; 
                  sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
                  LISTVIEW_SetItemText(hItem, 1, WavList.ptr, tmp);
                  LISTVIEW_SetItemTextColor(hItem, 1, WavList.ptr, LISTVIEW_CI_UNSEL, GUI_STCOLOR_DARKBLUE);   
                  
                  
                  WavList.ptr++;  
                  
                  LISTVIEW_SetSel(hItem, 0);
                  _PlayFile(FileInfo.pRoot);
                  
                  PlayerSettings.b.play = PLAY_ACTIVE;
                  hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
                  WM_InvalidateWindow(hItem);
                  WM_Update(hItem); 
                  
                }
              }
            }
          }
        }
        else if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
        {
          if(PlayerSettings.b.pause == PAUSE_ACTIVE)
          {   
            PlayerSettings.b.pause = PAUSE_INACTIVE;
            PlayerSettings.b.play = PLAY_ACTIVE;
            AUDIOPLAYER_Resume();
            
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem); 
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem);   
          }
        }  
      }
      break;
 
    /* Notifications sent by 'pause' Button */
    case ID_PAUSE_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
        {
          if(PlayerSettings.b.pause == PAUSE_INACTIVE)
          {   
            PlayerSettings.b.pause = PAUSE_ACTIVE;
            PlayerSettings.b.play = PAUSE_INACTIVE;
            AUDIOPLAYER_Pause();
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem); 
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
            WM_InvalidateWindow(hItem);
            WM_Update(hItem); 
          }
        }
      }
      break;      
 
    /* Notifications sent by 'equal' Button */
    case ID_EQUAL_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
        hEquWin = WM_CreateWindowAsChild(0, 0, 640, 480, pMsg->hWin, WM_CF_SHOW, _cbEqualDialog , 0);   
#else /* !AUDIO_USE_SPIRIT_EQUALIZER  */
#if 0 /* Don't show any pop-up window */
        WM_HWIN Hint = WM_CreateWindowAsChild(120,
                                            106,
                                            240, 60,
                                            pMsg->hWin,
                                            WM_CF_SHOW , 
                                            _cbHintEqNotAvailable, 
                                            0);
        WM_Update(Hint);  
        
        GUI_Delay(2000);

        /* Delete "equalizer initialization" message */
        WM_DeleteWindow(Hint);
#endif /* no pop-up window */
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
      }
      break; 

    /* Notifications sent by 'STOP' Button */
    case ID_STOP_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
        {
          PlayerSettings.b.pause = PAUSE_INACTIVE;
          PlayerSettings.b.play = PLAY_INACTIVE;
          
          AUDIOPLAYER_Stop();
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ELAPSED_TIME);
          TEXT_SetText(hItem, "00:00");            
          
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_DURATION);
          SLIDER_SetValue(hItem, 0);  
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);          
        }
      }
      break;
      
      
    /* Notifications sent by 'Next' Button */
    case ID_NEXT_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        if( WavList.ptr > 0)
        {        
          if(file_pos < (WavList.ptr - 1))
          {
            file_pos++;
            LISTVIEW_IncSel(WM_GetDialogItem(hMainWin, ID_WAVFILE_LIST)); 
          }
          else if(PlayerSettings.b.repeat == REPEAT_ALL)
          {
            file_pos = 0; 
            LISTVIEW_SetSel(WM_GetDialogItem(hMainWin, ID_WAVFILE_LIST), file_pos);
          }
          
          if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
          {    
            if(PlayerSettings.b.pause == PAUSE_ACTIVE)
            {  
              PlayerSettings.b.pause = PAUSE_INACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);

              PlayerSettings.b.play = PLAY_ACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);
            }
            
            AUDIOPLAYER_Stop();
            _PlayFile((char *)WavList.file[file_pos].name); 
          }
        }
      }
      break;
      
      /* Notifications sent by 'Previous' Button */
    case ID_PREVIOUS_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        if( WavList.ptr > 0)
        {
          if(file_pos > 0)
          {   
            file_pos--;
            LISTVIEW_DecSel(WM_GetDialogItem(hMainWin, ID_WAVFILE_LIST));             
          }
          else if(PlayerSettings.b.repeat == REPEAT_ALL)
          {
            file_pos = (WavList.ptr - 1); 
            LISTVIEW_SetSel(WM_GetDialogItem(hMainWin, ID_WAVFILE_LIST), file_pos);
          }          
          
          if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
          {  
            if(PlayerSettings.b.pause == PAUSE_ACTIVE)
            {  
              PlayerSettings.b.pause = PAUSE_INACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PAUSE_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);
              PlayerSettings.b.play = PLAY_ACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);
            }
            
            AUDIOPLAYER_Stop();
            _PlayFile((char *)WavList.file[file_pos].name);              
          }
        }
      }
      break;    
      
    /* Notifications sent by 'Open' Button */
    case ID_WAV_BUTTON: 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        
        break;
      case WM_NOTIFICATION_RELEASED:
        FileInfo.pfGetData = k_GetData;
        FileInfo.pMask = acMask_dir;     
        hItem = ST_CHOOSEFILE_Create(pMsg->hWin,  95, 115, 450, 250, apDrives, GUI_COUNTOF(apDrives), 0, "Add a folder", 0, &FileInfo);        
        r = GUI_ExecCreatedDialog(hItem);
        if (r == 0) 
        {
          _AddEntireFolder(FileInfo.pRoot, 1);
        }
        break;
        
      }
      break;
            
    /* Notifications sent by 'Add' Button */
    case ID_ADD_BUTTON: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        FileInfo.pfGetData = k_GetData;
        FileInfo.pMask = acMask_audio;     
        hItem = ST_CHOOSEFILE_Create(pMsg->hWin,  95, 115, 450, 250, apDrives, GUI_COUNTOF(apDrives), 0, "Add to playlist", 0, &FileInfo);
        r = GUI_ExecCreatedDialog(hItem);
        if (r == 0) 
        {
          if((strstr(FileInfo.pRoot, ".wav")) || (strstr(FileInfo.pRoot, ".WAV")))
          {
            if(WavList.ptr < FILEMGR_LIST_DEPDTH)
            {
              strcpy((char *)WavList.file[WavList.ptr].name, FileInfo.pRoot);
              FILEMGR_GetFileOnly (tmp, FileInfo.pRoot);
              hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
              
              if(AUDIOPLAYER_GetFileInfo(FileInfo.pRoot, &WavInfo) == 0)
              {
                LISTVIEW_AddRow(hItem, NULL);         
                LISTVIEW_SetItemText(hItem, 0, WavList.ptr, tmp);
                duration = WavInfo.FileSize / WavInfo.ByteRate; 
                sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
                LISTVIEW_SetItemText(hItem, 1, WavList.ptr, tmp);
                LISTVIEW_SetItemTextColor(hItem, 1, WavList.ptr, LISTVIEW_CI_UNSEL, GUI_STCOLOR_DARKBLUE);
                WavList.ptr++;      
              }
            }
          }
        }
        break;
      }
      break;
          /* Notification sent by "Button_Close" */  
    case ID_EXIT_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {  
        GUI_EndDialog(pMsg->hWin, 0);
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
  * @brief  Callback routine of informing user about exploring the disk
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbHint(WM_MESSAGE * pMsg) 
{ 
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    GUI_SetBkColor(0x00DCA939);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font16_1HK);
    GUI_DispStringHCenterAt("Audio Initialization ...", 110 , 10);
    GUI_SetFont(GUI_DEFAULT_FONT);
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;  
  }
}

/**
  * @brief  Audio player window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  WM_HWIN Hint;
    
  PlayerSettings.d32 = k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_SETTING_BKP);
  if(WavList.ptr == 0)
  {
    _AddEntireFolder("0:", 0);
    _AddEntireFolder("0:/Audio", 0);
  }
  
  /* Prevent Volume 0 */
  if(PlayerSettings.b.volume == 0)
  {
    PlayerSettings.b.volume = DEFAULT_AUDIO_VOLUME;
  }
  
  
  Hint = WM_CreateWindowAsChild(220,
                                190,
                                200, 40,
                                hWin,
                                WM_CF_SHOW , 
                                _cbHint, 
                                0);
  WM_Update(Hint);  
  
  /*Initialize audio Interface */
  AUDIOPLAYER_Init(PlayerSettings.b.volume);
  
  /* Delete "camera initialisation" message */
  WM_DeleteWindow(Hint);     

  hMainWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbMainDialog, hWin, xpos, ypos);
  
 
}

/**
  * @brief  Retrieve the file name from a full file path
  * @param  file: pointer to base path
  * @param  path: pointer to full path
  * @retval None
*/
static void FILEMGR_GetFileOnly (char *file, char *path)
{
  char *baseName1, *baseName2;
  baseName1 = strrchr(path,'/');
  baseName2 = strrchr(path,':');
  
  if(baseName1++) 
  { 
    strcpy(file, baseName1);
  }
  else 
  {
    if (baseName2++) 
    {
      strcpy(file, baseName2);
    }
    else
    {
      strcpy(file, path);
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
