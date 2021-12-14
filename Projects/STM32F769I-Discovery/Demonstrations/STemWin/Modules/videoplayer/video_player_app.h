/**
  ******************************************************************************
  * @file    JPEG/MJPEG_VideoDecoding/Inc/jpeg_decode_hw.h
  * @author  MCD Application Team
  * @brief   Header for decode_dma.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __JPEG_DECODE_HW_H
#define __JPEG_DECODE_HW_H

/* Includes ------------------------------------------------------------------*/
#include "GUI.h"
#include "audio_if.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define CHUNK_SIZE_OUT ((uint32_t)(768 * 10))

#define AVI_VIDEO_BUF_SIZE    (60 * 1024)
#define AUDIO_BUFFER_SIZE      (5 * 1024) 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void          HW_JPEG_Init(void);
U32           HW_JPEG_Draw (const void * pFileData, U32 DataSize, U32 x0, U32 y0);
void          HW_JPEG_DeInit(void);


U32           AUDIO_Init(U8 volume, U32 frequency);
U32           AUDIO_Play(U8 *pData, U16 Size);
U32           AUDIO_Stop   (void);
U32           AUDIO_DeInit (void);
U32           AUDIO_Notify(uint8_t msg);

#endif /* __JPEG_DECODE_HW_H */

