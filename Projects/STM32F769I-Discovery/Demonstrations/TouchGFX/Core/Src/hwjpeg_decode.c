/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include <stm32f7xx_hal.h>

#include <cmsis_os.h>

#include "ff.h"
#include "hwjpeg_decode.h"

static osMessageQId DecodeEvent;

void HwJPEG_MspInit(struct HwJPEG_Context_s *context)
{
  /* Enable JPEG clock */
  __HAL_RCC_JPEG_CLK_ENABLE();

  HAL_NVIC_SetPriority(JPEG_IRQn, 0x06, 0x0F);
  HAL_NVIC_EnableIRQ(JPEG_IRQn);

  /* Input DMA */    
  /* Set the parameters to be configured */
  context->hdmaIn.Init.Channel = DMA_CHANNEL_9;
  context->hdmaIn.Init.Direction = DMA_MEMORY_TO_PERIPH;
  context->hdmaIn.Init.PeriphInc = DMA_PINC_DISABLE;
  context->hdmaIn.Init.MemInc = DMA_MINC_ENABLE;
  context->hdmaIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  context->hdmaIn.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  context->hdmaIn.Init.Mode = DMA_NORMAL;
  context->hdmaIn.Init.Priority = DMA_PRIORITY_HIGH;
  context->hdmaIn.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  context->hdmaIn.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  context->hdmaIn.Init.MemBurst = DMA_MBURST_INC4;
  context->hdmaIn.Init.PeriphBurst = DMA_PBURST_INC4;

  context->hdmaIn.Instance = DMA2_Stream3;

  /* Associate the DMA handle */
  __HAL_LINKDMA(&context->hjpeg, hdmain, context->hdmaIn);

  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0x07, 0x0F);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

  /* DeInitialize the DMA Stream */
  HAL_DMA_DeInit(&context->hdmaIn);
  /* Initialize the DMA stream */
  HAL_DMA_Init(&context->hdmaIn);

  /* Output DMA */
  /* Set the parameters to be configured */ 
  context->hdmaOut.Init.Channel = DMA_CHANNEL_9;
  context->hdmaOut.Init.Direction = DMA_PERIPH_TO_MEMORY;
  context->hdmaOut.Init.PeriphInc = DMA_PINC_DISABLE;
  context->hdmaOut.Init.MemInc = DMA_MINC_ENABLE;
  context->hdmaOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  context->hdmaOut.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  context->hdmaOut.Init.Mode = DMA_NORMAL;
  context->hdmaOut.Init.Priority = DMA_PRIORITY_VERY_HIGH;
  context->hdmaOut.Init.FIFOMode = DMA_FIFOMODE_ENABLE;         
  context->hdmaOut.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  context->hdmaOut.Init.MemBurst = DMA_MBURST_INC4;
  context->hdmaOut.Init.PeriphBurst = DMA_PBURST_INC4;

  context->hdmaOut.Instance = DMA2_Stream4;
  /* DeInitialize the DMA Stream */
  HAL_DMA_DeInit(&context->hdmaOut);
  /* Initialize the DMA stream */
  HAL_DMA_Init(&context->hdmaOut);

  /* Associate the DMA handle */
  __HAL_LINKDMA(&context->hjpeg, hdmaout, context->hdmaOut);

  HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0x07, 0x0F);
  HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
}

uint32_t JPEG_InitDecode_HW(struct HwJPEG_Context_s *context)
{
  JPEG_InitColorTables();

  HwJPEG_MspInit(context);

  context->hjpeg.Instance = JPEG;
  HAL_JPEG_Init(&context->hjpeg);

  osMessageQDef(decode_queue, 1, uint32_t);
  DecodeEvent = osMessageCreate(osMessageQ(decode_queue), NULL);

  return 0;
}

void JPEG_StopDecode_HW(struct HwJPEG_Context_s *context)
{
  vQueueDelete(DecodeEvent);

  HAL_JPEG_Abort(&context->hjpeg);
}

int JPEG_Decode_DMA(struct HwJPEG_Context_s *context, uint8_t *buffer, uint32_t length, uint32_t MCUBufferAddress, uint32_t DestAddress)
{
  /* Initialize decode context */
  context->input_buffer = buffer;
  context->input_buffer_size = length;
  context->output_buffer = (uint8_t*)MCUBufferAddress;
  context->output_buffer_size = MAX_CHUNK_SIZE;
  context->input_offset = 0;
  context->output_offset = 0;
  context->pYCbCr_RGB = NULL;

  /* Input DMA can only fetch up-to 65535 32bit word at once */
  int size = length > MAX_CHUNK_SIZE ? MAX_CHUNK_SIZE : length;

  /* Clean DCache prior to hw JPEG access to data */
  SCB_CleanDCache();

  HAL_StatusTypeDef ret = HAL_JPEG_Decode_DMA(&context->hjpeg,
                                              context->input_buffer,
                                              size,
                                              context->output_buffer, MAX_CHUNK_SIZE);

  if (ret == HAL_OK) {
    osMessageGet(DecodeEvent, 0); /* make sure the queue is empty */
    osEvent event = osMessageGet(DecodeEvent, osWaitForever);

    if (event.status == osEventMessage)
    {
      if (event.value.v == DECODE_DECODE_COMPLETE_MESSAGE)
      {
        uint32_t MCU_count;

        if (context->pYCbCr_RGB) {
          /* Invalidate DCache prior the YCbCr/RGB software conversion */
          SCB_CleanInvalidateDCache();
          context->pYCbCr_RGB((uint8_t*)MCUBufferAddress,
                              (uint8_t*)DestAddress,
                              0, context->output_offset, &MCU_count);
        }
        else
          return -1; /* No HAL_JPEG_InfoReadyCallback? */

        return 0;
      }
      else if (event.value.v == DECODE_ERROR_MESSAGE)
             return -1;
    }

    return -1;
  }

  return -1;
}

void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{
  struct HwJPEG_Context_s *context =
      container_of(hjpeg, struct HwJPEG_Context_s, hjpeg);

  JPEG_GetDecodeColorConvertFunc(pInfo, &context->pYCbCr_RGB, &context->MCU_Total);
}

void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
  struct HwJPEG_Context_s *context =
      container_of(hjpeg, struct HwJPEG_Context_s, hjpeg);

  context->input_offset += NbDecodedData;

  int length = context->input_buffer_size - context->input_offset;

  if (length > 0) {
    int size = (length + 4) > MAX_CHUNK_SIZE ? MAX_CHUNK_SIZE : length;

    HAL_JPEG_ConfigInputBuffer(hjpeg,
                               context->input_buffer
                                   + context->input_offset,
                               size);
  } else
      HAL_JPEG_ConfigInputBuffer(hjpeg,
                                 context->input_buffer
                                     + context->input_offset,
                                 0);
}

void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
  struct HwJPEG_Context_s *context =
      container_of(hjpeg, struct HwJPEG_Context_s, hjpeg);

  context->output_offset += OutDataLength;

  HAL_JPEG_ConfigOutputBuffer(hjpeg,
                              context->output_buffer
                                  + context->output_offset,
                              MAX_CHUNK_SIZE);
}

void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
  osMessagePut(DecodeEvent, DECODE_ERROR_MESSAGE, 0);
}

void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{
  osMessagePut(DecodeEvent, DECODE_DECODE_COMPLETE_MESSAGE, 0);
}
