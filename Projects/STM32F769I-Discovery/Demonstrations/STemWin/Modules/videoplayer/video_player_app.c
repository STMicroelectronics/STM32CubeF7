/**
  ******************************************************************************
  * @file    videoplayer_app.c
  * @author  MCD Application Team
  * @brief   This file provides routines for JPEG decoding with DMA method.
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
#include "video_player_app.h"
#include "jpeg_utils.h"
#include "LCDConf.h"
#include "GUI_AVI.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup MJPEG_VideoDecoding
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DECODE_CONVERT_OUTPUT_BUFF    0x11
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
JPEG_YCbCrToRGB_Convert_Function pConvert_Function;
JPEG_HandleTypeDef    JPEG_Handle;

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma location=0x2000A000
uint32_t MCU_Data_OutBuffer[CHUNK_SIZE_OUT/4];

#elif defined ( __CC_ARM ) /* Keil Compiler */
uint32_t MCU_Data_OutBuffer[CHUNK_SIZE_OUT/4] __attribute__((at(0x2000A000)));

#elif defined ( __GNUC__ ) /* GNU Compiler */
uint32_t MCU_Data_OutBuffer[CHUNK_SIZE_OUT/4] __attribute__((section(".MCU_Data_section")));
#endif

extern GUI_AVI_HANDLE havi;
static uint32_t MCU_BlockIndex = 0;
osThreadId hOutputThread;
static osMessageQId OutputEvent = 0;
uint32_t MCU_TotalNb = 0, IsFirstTime = 0;
osSemaphoreId osVidSemph;
/* Private function prototypes -----------------------------------------------*/
static void OutputThread(void const *argument);
extern void LCD_LL_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine);
void TransferComplete_CallBack(void);
void HalfTransfer_CallBack(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init HArdware Decoder
  * @param  None
  * @retval None
  */
void HW_JPEG_Init(void)
{
  static DMA_HandleTypeDef   hdmaIn;
  static DMA_HandleTypeDef   hdmaOut;
  
  JPEG_InitColorTables();
  
    /* Enable JPEG clock */
  __HAL_RCC_JPEG_CLK_ENABLE();
    /* Enable DMA clock */
  __DMA2_CLK_ENABLE();  
  
  HAL_NVIC_SetPriority(JPEG_IRQn, 0x06, 0x0F);
  HAL_NVIC_EnableIRQ(JPEG_IRQn);
  
  /* Input DMA */    
  /* Set the parameters to be configured */
  hdmaIn.Init.Channel = DMA_CHANNEL_9;
  hdmaIn.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdmaIn.Init.PeriphInc = DMA_PINC_DISABLE;
  hdmaIn.Init.MemInc = DMA_MINC_ENABLE;
  hdmaIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaIn.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdmaIn.Init.Mode = DMA_NORMAL;
  hdmaIn.Init.Priority = DMA_PRIORITY_HIGH;
  hdmaIn.Init.FIFOMode = DMA_FIFOMODE_ENABLE;         
  hdmaIn.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdmaIn.Init.MemBurst = DMA_MBURST_INC4;
  hdmaIn.Init.PeriphBurst = DMA_PBURST_INC4;      
  
  hdmaIn.Instance = DMA2_Stream3;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(&JPEG_Handle, hdmain, hdmaIn);
  
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0x06, 0x0F);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);    
  
  /* DeInitialize the DMA Stream */
  HAL_DMA_DeInit(&hdmaIn);  
  /* Initialize the DMA stream */
  HAL_DMA_Init(&hdmaIn);
  
  
  /* Output DMA */
  /* Set the parameters to be configured */ 
  hdmaOut.Init.Channel = DMA_CHANNEL_9;
  hdmaOut.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdmaOut.Init.PeriphInc = DMA_PINC_DISABLE;
  hdmaOut.Init.MemInc = DMA_MINC_ENABLE;
  hdmaOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdmaOut.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdmaOut.Init.Mode = DMA_NORMAL;
  hdmaOut.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  hdmaOut.Init.FIFOMode = DMA_FIFOMODE_ENABLE;         
  hdmaOut.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdmaOut.Init.MemBurst = DMA_MBURST_INC4;
 
  hdmaOut.Init.PeriphBurst = DMA_PBURST_INC4;

  hdmaOut.Instance = DMA2_Stream4;
  /* DeInitialize the DMA Stream */
  HAL_DMA_DeInit(&hdmaOut);  
  /* Initialize the DMA stream */
  HAL_DMA_Init(&hdmaOut);

  /* Associate the DMA handle */
  __HAL_LINKDMA(&JPEG_Handle, hdmaout, hdmaOut);
  
  HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0x07, 0x0F);
  HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);  
  
   /* Init the HAL JPEG driver */
  JPEG_Handle.Instance = JPEG;
  HAL_JPEG_Init(&JPEG_Handle);

    /* Output FRAME Queue */
  osMessageQDef(OUTPUT_Queue, 3, uint16_t);
  OutputEvent = osMessageCreate (osMessageQ(OUTPUT_Queue), NULL); 
  
  /* Output Thread  definition */
  osThreadDef(OUTPUT_THREAD, OutputThread, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE);
  hOutputThread = osThreadCreate(osThread(OUTPUT_THREAD), NULL);
  
    /* Create the Semaphore used by the two threads */
    /* Create Semaphore lock */
  osSemaphoreDef(Semaphore);
  osVidSemph= osSemaphoreCreate(osSemaphore(Semaphore), 1);  
  osSemaphoreWait(osVidSemph , osWaitForever); 
}


/**
  * @brief  Stop HArdware Decoder
  * @param  None
  * @retval None
  */
void HW_JPEG_DeInit(void)
{ 
  HAL_JPEG_DeInit(&JPEG_Handle);
  HAL_DMA_DeInit(JPEG_Handle.hdmaout);  
  __HAL_RCC_JPEG_CLK_DISABLE();
  
  if(hOutputThread)
  {
    osThreadTerminate(hOutputThread);
    hOutputThread = 0;
  }
  
  if(osVidSemph)
  {
    osSemaphoreDelete(osVidSemph);
    osVidSemph = 0;
  }
  
  if(OutputEvent)
  {
    vQueueDelete(OutputEvent); 
    OutputEvent = 0;  
  }
}

/**
  * @brief  Output Jpeg task
  * @param  None
  * @retval None
  */
static void OutputThread(void const *argument)
{
  uint32_t ConvertedDataCount;
  osEvent event;
  
  for(;;)
  {
    event = osMessageGet(OutputEvent, osWaitForever );
    
    if( event.status == osEventMessage )
    {
      switch(event.value.v)
      {
      case DECODE_CONVERT_OUTPUT_BUFF:
        MCU_BlockIndex += pConvert_Function( (uint8_t *)MCU_Data_OutBuffer, (uint8_t *)0xC0000000, MCU_BlockIndex, CHUNK_SIZE_OUT, &ConvertedDataCount);    
        
        if((MCU_BlockIndex == MCU_TotalNb) && (MCU_TotalNb != 0))
        {
            LCD_LL_DrawBitmap16bpp(0, 0, 0, (uint16_t *)0xC0000000, 800, 480, 1600);
        }
        else
        {
          HAL_JPEG_Resume(&JPEG_Handle, JPEG_PAUSE_RESUME_OUTPUT); 
        }
        break;        
      }
    } 
  }
}

/**
  * @brief  Decode_DMA
  * @param hjpeg: JPEG handle pointer
  * @param  FileName    : jpg file path for decode.
  * @param  DestAddress : ARGB destination Frame Buffer Address.
  * @retval None
  */
U32 HW_JPEG_Draw (const void * pFileData, U32 DataSize, U32 x0, U32 y0)
{  
  MCU_BlockIndex = 0;
  MCU_TotalNb = 0;
  HAL_JPEG_Decode_DMA(&JPEG_Handle , (uint8_t *)pFileData ,DataSize ,(uint8_t *)MCU_Data_OutBuffer ,CHUNK_SIZE_OUT);
  if(osSemaphoreWait(osVidSemph , 1000) == osErrorOS)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  JPEG Data Ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pDataOut: pointer to the output data buffer
  * @param OutDataLength: length of output buffer in bytes
  * @retval None
  */
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
  HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
  HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t *)MCU_Data_OutBuffer, CHUNK_SIZE_OUT); 
  osMessagePut ( OutputEvent, DECODE_CONVERT_OUTPUT_BUFF, 0);
}

/**
  * @brief  JPEG Decode complete callback
  * @param hjpeg: JPEG handle pointer
  * @retval None
  */
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{
  osSemaphoreRelease(osVidSemph);
}

/**
  * @brief  JPEG Info ready callback
  * @param hjpeg: JPEG handle pointer
  * @param pInfo: JPEG Info Struct pointer
  * @retval None
  */
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
  if(JPEG_GetDecodeColorConvertFunc(pInfo, &pConvert_Function, &MCU_TotalNb) == HAL_OK)
  {
  }   
}
/**
  * @}
  */

/**
  * @}
  */

