/**
  ******************************************************************************
  * @file    LwIP/LwIP_StreamingServer/Src/encode.c 
  * @author  MCD Application Team
  * @brief   This file contain the compress method.
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
#include "encode.h"

/* Private typedef -----------------------------------------------------------*/
   /* This struct contains the JPEG compression parameters */
   static struct jpeg_compress_struct cinfo; 
   /* This struct represents a JPEG error handler */
   static struct jpeg_error_mgr jerr; 
  uint32_t JPEG_ImgSize = 0;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Jpeg Encode
  * @param  file:          pointer to the bmp file
  * @param  file1:         pointer to the jpg file  
  * @param  width:         image width
  * @param  height:        image height
  * @param  image_quality: image quality
  * @retval None
  */
void jpeg_encode(uint8_t *RGB_buffer, uint8_t *JPEG_buffer, uint32_t width, uint32_t height, uint32_t image_quality)
{ 
  /* Encode BMP Image to JPEG */
  JSAMPROW row_pointer;    /* Pointer to a single row */
  uint8_t* outbuffer = NULL;
  unsigned long outsize = 0;

  /* Step 1: allocate and initialize JPEG compression object */
  /* Set up the error handler */
  cinfo.err = jpeg_std_error(&jerr);
  
  /* Initialize the JPEG compression object */  
  jpeg_create_compress(&cinfo);
  
  /* Step 2: specify data destination */
  outbuffer = JPEG_buffer;  
  outsize = JPEG_BUFFER_SIZE;  
  
  jpeg_mem_dest(&cinfo, &outbuffer, &outsize);
  
  /* Step 3: set parameters for compression */
  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  
  /* Set default compression parameters */
  jpeg_set_defaults(&cinfo);
  
  cinfo.dct_method  = JDCT_FLOAT;    
  
  jpeg_set_quality(&cinfo, image_quality, TRUE);
  
  /* Step 4: start compressor */
  jpeg_start_compress(&cinfo, TRUE);
  
 
  while (cinfo.next_scanline < cinfo.image_height)
  {          
      row_pointer=(JSAMPROW)(&RGB_buffer[cinfo.next_scanline*width*3]);
      jpeg_write_scanlines(&cinfo, &row_pointer, 1); 
  }
 
  /* Step 5: finish compression */
  jpeg_finish_compress(&cinfo);
  
  JPEG_ImgSize = JPEG_BUFFER_SIZE - cinfo.dest->free_in_buffer;
  
  /* Step 6: release JPEG compression object */
  jpeg_destroy_compress(&cinfo);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
