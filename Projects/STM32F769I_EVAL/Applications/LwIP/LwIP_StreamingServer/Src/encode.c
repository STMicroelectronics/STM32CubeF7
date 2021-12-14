/**
  ******************************************************************************
  * @file    LwIP/LwIP_StreamingServer/Src/encode.c 
  * @author  MCD Application Team
  * @brief   This file contain the compress method.
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

