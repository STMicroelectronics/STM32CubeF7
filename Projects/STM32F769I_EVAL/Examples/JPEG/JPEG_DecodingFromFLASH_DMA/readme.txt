/**
  @page JPEG_DecodingFromFLASH_DMA : JPEG example : JPEG decoding with DMA of an image stored in FLASH example
  
  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_DecodingFromFLASH_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the JPEG decoding from FLASH with DMA example.
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This example demonstrates how to decode a JPEG image stored in the internal FLASH 
using the JPEG HW decoder in DMA mode and display the final ARGB8888 image on KoD LCD 
mounted on board or a HDMI monitor Connected through the DSI-HDMI bridge board MB1232.A.

The example can automatically detect if the KoD LCD is connected or if the DSI-HDMI 
bridge is connected to the DSI connector (CN15), if no DSI slave is connected the RED LED will turn On.


The example performs the following actions :
  - Init the JPEG decoder.
  - Init and configure the LCD_DSI screen
  - Call the function "JPEG_Decode_DMA" to start decoding the jpg buffer with DMA (Non blocking method).
    Function "JPEG_Decode_DMA" is implemented in a separate module decode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver
  
  - Once the decoding ends :
    - Call HAL routine "HAL_JPEG_GetInfo" to retrieve the image parameters :
      image width, image height, image quality (from 1% to 100%) color space and Chroma sampling.      
    - copy the ARGB8888 result buffer to the LCD Frame buffer for display:
        Using the DMA2D and positioning the image in the center of the LCD screen.

@note The jpeg HW decoder outputs are organized in YCbCr blocks. 
      The jpeg utilities functions implemented in "jpeg_utils.c" file are used to convert these YCbCr blocks 
      to RGB pixels to the destination JPEG output buffer.
      The JPEG utilities files allowing to convert YCbCr blocks to RGB pixels are available under \Utilities\JPEG

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@note In case of EVAL's LCD use in example code, the DATA_IN_ExtSDRAM define
      need to be added  in the project preprocessor to avoid SDRAM multiple configuration
     (the LCD uses SDRAM as frame buffer, and its configuration is done by the BSP_SDRAM_Init()). 

@note The compiler optimizations level could be set to High/Balanced giving a good tradeoff between the code size and the performance (decode time).

@par Keywords

Display, Graphic, JPEG, Decoding, Flash memory, DMA, ARGB8888, Hardware decoding, LCD, HDMI, YCbCr, RGB,
      
@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Directory contents

  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/stm32f7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/stm32f7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/main.h                        Header for main.c module  
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/decode_dma.h                  Header for decode_dma.c module   
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/image_320_240_jpg.h           JPEG Image 320x240 to be decoded and displayed on LCD
  - JPEG/JPEG_DecodingFromFLASH_DMA/Inc/jpeg_utils_conf.h             JPEG utilties module configuration file
  
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/stm32f7xx_it.c                Interrupt handlers
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/main.c                        Main program
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/decode_dma.c                  JPEG Decoding (from memory) with DMA module
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/stm32f7xx_hal_msp.c           HAL MSP module   
  - JPEG/JPEG_DecodingFromFLASH_DMA/Src/system_stm32f7xx.c            STM32F7xx system source file  
  

@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx devices.
    
  - This example has been tested with STMicroelectronics STM32F769I_EVAL revB 
    evaluation boards and can be easily tailored to any other supported device 
    and development board.
    
@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Mount either the KoD LCD pannel (Board MB1166 rev.A) or the DSI-HDMI bridge(Board MB1232 Rev.A)
   to the DSI connector (CN15). 
 - if using the DSI-HDMI bridge then connect a HDMI monitor using a HDMI cable to the 
   MB1232 board.  
 
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */