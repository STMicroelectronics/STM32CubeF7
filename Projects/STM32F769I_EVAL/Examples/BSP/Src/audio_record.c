/**
  ******************************************************************************
  * @file    BSP/Src/audio_record.c
  * @author  MCD Application Team
  * @brief   This example describes how to use DFSDM HAL API to realize
  *          audio recording.
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

/** @addtogroup BSP_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

/* Private define ------------------------------------------------------------*/
#define SCRATCH_BUFF_SIZE  512
/* Size of the recorder buffer */
#define RECORD_BUFFER_SIZE  2048

int32_t Scratch[SCRATCH_BUFF_SIZE];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer containing the PCM samples coming from the microphone */
int16_t RecordBuffer[RECORD_BUFFER_SIZE];

/* Buffer used to stream the recorded PCM samples towards the audio codec. */
int16_t PlaybackBuffer[RECORD_BUFFER_SIZE];

static BUFFER_StateTypeDef  RecordBufferOffset = BUFFER_OFFSET_NONE;

extern AUDIO_DrvTypeDef          *audio_drv;
extern SAI_HandleTypeDef         haudio_out_sai;
/* Private function prototypes -----------------------------------------------*/
static void AudioRecord_SetHint(void);
static void CopyBuffer(int16_t *pbuffer1, int16_t *pbuffer2, uint16_t BufferSize);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test Audio Hardware.
  *   The main objective of this test is to check the hardware connection of the 
  *   Audio peripheral.
  * @param  None
  * @retval None
  */

void AudioRecord_demo(void)
{ 
  /* Flag indicating whether the audio codec has already been initialized */
  uint32_t audio_loop_back_init = RESET ; 
  
  AudioRecord_SetHint();
  
  if(BSP_AUDIO_IN_Init(BSP_AUDIO_FREQUENCY_44K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK)
  {
    /* Record Error */
    Error_Handler();
  }  
  
  /* Allocate scratch buffer */
  BSP_AUDIO_IN_AllocScratch (Scratch, SCRATCH_BUFF_SIZE);
  /* Start the record */
  if (BSP_AUDIO_IN_Record((uint16_t*)&RecordBuffer[0], RECORD_BUFFER_SIZE) != AUDIO_OK)
  {
    /* Record Error */
    Error_Handler();
  }
  
  RecordBufferOffset = BUFFER_OFFSET_NONE; 
  
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(0, 130, (uint8_t *)"RECORD AND PLAYBACK ONGOING", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 145, (uint8_t *)"FREQ:  44 Khz", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"Press Wkup/Tamper push-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"to stop recording", CENTER_MODE);
  
  while(1)
  {
    /* 1st or 2nd half of the record buffer ready for being copied 
    to the playbakc buffer */
    if(RecordBufferOffset != BUFFER_OFFSET_NONE)
    {
      /* Copy half of the record buffer to the playback buffer */
      if(RecordBufferOffset == BUFFER_OFFSET_HALF)
      {
        CopyBuffer(&PlaybackBuffer[0], &RecordBuffer[0], RECORD_BUFFER_SIZE/2);
        
        if (audio_loop_back_init == RESET)
        {
          /* Initialize the audio device*/ 
          if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 
                                 90, 
                                 BSP_AUDIO_FREQUENCY_44K) != AUDIO_OK)
          {
            Error_Handler();
          }
          BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);

          /* Play the recorded buffer */
          BSP_AUDIO_OUT_Play((uint16_t *) &PlaybackBuffer[0], 2*RECORD_BUFFER_SIZE);
         
          /* Audio device is initialized only once */
          audio_loop_back_init = SET; 
        }
      }
      else /* if(RecordBufferOffset == BUFFER_OFFSET_FULL)*/
      {
        CopyBuffer(&PlaybackBuffer[RECORD_BUFFER_SIZE/2], &RecordBuffer[RECORD_BUFFER_SIZE/2], RECORD_BUFFER_SIZE/2);
        
      }
      
      /* Wait for next data */
      RecordBufferOffset = BUFFER_OFFSET_NONE;  
    }
    /* Check for User input */
    if(CheckForUserInput() > 0)
    {
      /* Stop audio device */
      if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW) != AUDIO_OK)
      {
        Error_Handler();
      }
      
      /* De-initialize audio device */
      BSP_AUDIO_OUT_DeInit();
      
      /* Stop audio input */
      if (BSP_AUDIO_IN_Stop() != AUDIO_OK)
      {
        Error_Handler();
      }
      
      /* De-initialize audio input */
      BSP_AUDIO_IN_DeInit();
         
      return;
    }
  } 
}

/**
  * @brief  Copy content of pbuffer2 to pbuffer1
  * @param  None
  * @retval None
  */
static void CopyBuffer(int16_t *pbuffer1, int16_t *pbuffer2, uint16_t BufferSize)
{
  uint32_t i = 0;
  for(i = 0; i < BufferSize; i++)
  {
    pbuffer1[i] = pbuffer2[i];
  }
}
                                
/**
  * @brief  Display Audio Play demo hint
  * @param  None
  * @retval None
  */
static void AudioRecord_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO RECORD", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example is recording", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"audio samples on Micros", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"and play directly on CN28", CENTER_MODE); 

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
}

/**
  * @brief Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
    RecordBufferOffset = BUFFER_OFFSET_FULL;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
    RecordBufferOffset = BUFFER_OFFSET_HALF;
}

/**
  * @brief  Audio IN Error callback function
  * @param  pData
  * @retval None
  */
void BSP_AUDIO_IN_Error_Callback(void)
{
  /* Stop the program with an infinite loop */
  Error_Handler();
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
