/**
  @page DMA2D_MemToMemWithPFC DMA2D Memory to Memory with PFC example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithPFC/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D Memory to Memory with PFC example.
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

  This example provides a description of how to configure DMA2D peripheral for transfer 
  in Memory_to_Memory with Pixel Format Conversion (PFC) Mode.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 216 MHz.
 
  In this basic example the goal is to explain the different fields of the DMA2D 
  structure in the case of Memory_to_Memory with pixel format conversion (PFC) transfer mode
  and the difference between pixel coded on 32bits (ARGB8888 format) and coded on 16bits (ARGB4444 format).
 
  An image is transferred with DMA2D from Flash memory to internal SRAM and during the transfer,
  a pixel format conversion is applied from ARGB8888 (32 bpp) to ARGB4444 (16 bpp). 
  The original image and the DMA2D output image are displayed on the LCD in two layers 
  translated on screen in order to visually see the difference between an image coded 
  on 16 bits (ARGB4444) and an image of higher color resolution coded on 32 bits (ARGB8888).
 
 In this example two LTDC layers are used to display the original ARGB8888 and the DMA2D color converted
 ARGB4444 images as following :
  - Layer 1 is configured to display the original image with ARGB8888 as 
    pixel format and 320x120 size.
  - Layer 2 is configured to display the converted image with ARGB4444 as 
    pixel format and 320x120 size.

  @note :
  how to calculate the size of the transferred data ? 
  => selected color mode gives the number of bits per pixel and we have 
    the number of pixel per line and the number of line, therefore :
    
    data_size = (bits per pixel) X (pixel per line) X (number of line)
    
 How to convert pixel format from ARGB8888 to ARGB4444 ?
 => only the four MSB are taken into account 
 eg : 0x AB  CD  12  34 --> 0x A   C   1   3    
        |_| |_| |_| |_|       |_| |_| |_| |_|
         A   R   G   B         A   R   G   B
         
 In general, 
 => if the number of bits per pixel in source data is more then the number of 
    bits per pixel in destination data, only the MSB are taken into account
    
 => else, if the number of bits per pixel in source data is less then the number 
    of bits per pixel in destination data, a bit replication of MSB in LSB is applied
    
    eg : from ARGB1555 to ARGB8888       PFC
    0xAB35 --> 0b1 01010 11001 10101   -------> 0b11111111 01010010 11001110 10101101     
                 A   R     G     B          --> 0xFF       52       CE       AD

STM327x6G-EVAL board revB's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error.
 - LED3 is ON when there is an error in LTDC transfer/Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, DMA2D, LCD, SRAM, ARGB4444, PFC, Memory to memory, Foreground,
Background, RGB565, LTDC, Pixel format conversion, pixel

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

    - DMA2D/DMA2D_MemToMemWithPFC/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_MemToMemWithPFC/Inc/stm32f7xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithPFC/Inc/stm32f7xx_hal_conf.h  HAL configuration file
    - DMA2D/DMA2D_MemToMemWithPFC/Inc/ARGB8888_300x120.h    image to be converted and transferred by DMA2D. 
    - DMA2D/DMA2D_MemToMemWithPFC/Src/main.c                Main program  
    - DMA2D/DMA2D_MemToMemWithPFC/Src/stm32f7xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithPFC/Src/stm32f7xx_hal_msp.c   HAL MSP module    
    - DMA2D/DMA2D_MemToMemWithPFC/Src/system_stm32f7xx.c    STM32F7xx system clock configuration file

@par Hardware and Software environment  

  - This example runs on STM32F756xx/STM32F746xx devices.
  
  - This example has been tested with STM327x6G-EVAL board revBs and can be easily 
    tailored to any other supported device and development board. 


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   