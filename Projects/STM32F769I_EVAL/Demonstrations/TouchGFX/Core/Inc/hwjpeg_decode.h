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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DECODE_DMA_H
#define __DECODE_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f7xx_hal_dma.h>
#include <stm32f7xx_hal_jpeg.h>

#include "jpeg_utils.h"

#define DECODE_ERROR_MESSAGE              0xFFFF
#define DECODE_DECODE_COMPLETE_MESSAGE    0xFFD8

#define JPEG_SOI_MARKER                 (0xFFD8) /* JPEG Start Of Image marker*/
#define JPEG_SOI_MARKER_BYTE0           (JPEG_SOI_MARKER & 0xFF)
#define JPEG_SOI_MARKER_BYTE1           ((JPEG_SOI_MARKER >> 8) & 0xFF)

#define MAX_CHUNK_SIZE  8192

struct HwJPEG_Context_s {
    DMA_HandleTypeDef hdmaIn;
    DMA_HandleTypeDef hdmaOut;
    JPEG_HandleTypeDef hjpeg;
    uint8_t *input_buffer;
    uint32_t input_buffer_size;
    uint8_t *output_buffer;
    uint32_t output_buffer_size;
    uint32_t input_offset;
    uint32_t output_offset;
    JPEG_YCbCrToRGB_Convert_Function pYCbCr_RGB;
    uint32_t MCU_Total;
};

#define container_of(ptr, type, member) \
        (struct HwJPEG_Context_s *)( (char *)ptr - offsetof(type,member) )

uint32_t JPEG_InitDecode_HW(struct HwJPEG_Context_s *context);
int JPEG_Decode_DMA(struct HwJPEG_Context_s *context, uint8_t *buffer, uint32_t length, uint32_t MCUBufferAddress, uint32_t DestAddress);
void JPEG_StopDecode_HW(struct HwJPEG_Context_s *context);

#ifdef __cplusplus
}
#endif

#endif /* __DECODE_DMA_H */

