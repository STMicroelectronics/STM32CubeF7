/**
  @page LCD_PicturesFromSDCard LCD application
  
  @verbatim  
  ******************************************************************************
  * @file    Display/LCD_PicturesFromSDCard/Media/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LCD_PicturesFromSDCard application used pictures.
  ******************************************************************************
  *
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Description 
  - The bitmap images should be copied inside a dedicated directory named as "Media" 
    at the micro SD card root.
    The images should have the following properties:
     file type    : *.bmp
     resolution   : up to 800x480.
     pixel format : RGB565, RGB888 or ARGB8888

@par Keywords

Graphic, Display, LCD, Pictures, SD Card, BMP, RGB565, RGB888, ARGB8888

@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”
	 

 */
