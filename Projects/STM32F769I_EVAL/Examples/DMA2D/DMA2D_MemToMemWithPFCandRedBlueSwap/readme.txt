/**
  @page DMA2D_MemToMemWithPFCandRedBlueSwap DMA2D Memory to Memory with LCD example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D Memory to Memory with Pixel Format Conversion and Red/Blue swap example.
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

How to configure the DMA2D peripheral in Memory-to-memory transfer mode with
pixel format conversion and red and blue swap, and display the result on the
LCD. 

  The source buffer in format RGB565 and size 320x240 located in Flash is copied 
  in Memory to Memory with PFC mode by the DMA2D to the SDRAM LCD frame buffer for display. 
  The DMA2D output offset and destination address are calculated in a way 
  to locate the image in the center of the LCD screen.
 

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 200 MHz.
 
  In this basic example the goal is to explain the utility of the DMA2D field "RedBlueSwap" 
  in the case of Memory_to_Memory with PFC transfer mode.
  User can easily see the difference on the displayed image between "RedBlueSwap" set to DMA2D_RB_REGULAR and DMA2D_RB_SWAP:
  Red and Blue pixels components are mutually swapped.
    
  The example acts as follow :
  In an infinite while loop
  - first the LTDC, DSI and LCD screen are initialized
  - The DMA2D is configured to perform a transfer from input RGB565 image to output ARGB8888 display frame buffer.
  - The parameter "RedBlueSwap" is changed for each iteration ( from DMA2D_RB_REGULAR to DMA2D_RB_SWAP and vice versa).
  - then start the DMA2D and wait for the end of the transfer.
  - wait for 2 sec and go to the next loop.
  
  when the parameter "RedBlueSwap" is set to DMA2D_RB_REGULAR : display the message "Red and Blue Swap : OFF"
  when the parameter "RedBlueSwap" is set to DMA2D_RB_SWAP : display the message "Red and Blue Swap : ON "
  
 => Output offset is the number of pixel to be skipped after each transferred line
                      
 how to calculate the size of the transferred data ? 
 => selected colour mode gives the number of bits per pixel and we have 
    (RGB565 => 16bits/pixel) the number of pixel per line and the number of line, 
    therefore :
    
    data_size = (bits per pixel) X (pixel per line + output offset) X (number of line) 
       
STM32F769I-EVAL board's LEDs can be used to monitor the transfer status:
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

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
 @note  The header files of the images used in this example can be generated with 
        STemWin bitmap converter released with this package.
        \Middlewares\ST\STemWin\Software\BmpCvtST.exe      

@par Keywords

Display, Graphic, DMA2D, LCD, SRAM, RGB565, Blending, Memory to memory, Pixel Format Conversion,
Red Blue swap, LTDC, Pixel

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

    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/stm32f7xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/stm32f7xx_hal_conf.h  HAL configuration file
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Inc/RGB565_320x240.h      RGB565 input image to be transferred by DMA2D
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Src/main.c                Main program  
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Src/stm32f7xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Src/stm32f7xx_hal_msp.c   HAL MSP module    
    - DMA2D/DMA2D_MemToMemWithPFCandRedBlueSwap/Src/system_stm32f7xx.c    STM32F7xx system clock configuration file

@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32F769I-EVAL board and can be
    easily tailored to any other supported device and development board. 


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                    
