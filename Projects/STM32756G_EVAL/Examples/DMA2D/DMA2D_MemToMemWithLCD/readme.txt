/**
  @page DMA2D_MemToMemWithLCD DMA2D Memory to Memory with LCD example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemToMemWithLCD/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D Memory to Memory with LCD example.
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

How to configure DMA2D peripheral in Memory-to-memory transfer mode and display
the result on the LCD.

  The source buffer in format ARGB4444 and size 150x150 located in Flash is copied in Memory to Memory mode 
  by the DMA2D in SRAM Destination buffer of same size and format.
  This SRAM buffer is then displayed by the LTDC centered on LCD screen.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 216 MHz.
 
  In this basic example the goal is to explain the different fields of the DMA2D 
  structure in the case of Memory_to_Memory transfer mode.

  After DMA2D configuration, the data transfer Memory to Memory is performed from FLASH memory to
  SRAM. Then the LTDC is configured to display obtained DMA2D output buffer in SRAM on LCD. 
  (more details about LCD configuration in LCD examples)
  
 => Output offset is the number of pixel to be skipped after each transferred line

 how to calculate the size of the transferred data ? 
 => selected color mode gives the number of bits per pixel and we have 
    (ARGB4444 => 16bits/pixel) the number of pixel per line and the number of line, 
    therefore :
    
    data_size = (bits per pixel) X (pixel per line + output offset) X (number of line) 
       
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

Display, Graphic, DMA2D, LCD, SRAM, ARGB4444, Blending, Memory to memory, LTDC, Pixel

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

    - DMA2D/DMA2D_MemToMemWithLCD/Inc/main.h                Main configuration file
    - DMA2D/DMA2D_MemToMemWithLCD/Inc/stm32f7xx_it.h        Interrupt handlers header file
    - DMA2D/DMA2D_MemToMemWithLCD/Inc/stm32f7xx_hal_conf.h  HAL configuration file
    - DMA2D/DMA2D_MemToMemWithLCD/Inc/ARGB4444_150x150.h    Texture in Flash to be transferred by DMA2D in SRAM
    - DMA2D/DMA2D_MemToMemWithLCD/Src/main.c                Main program  
    - DMA2D/DMA2D_MemToMemWithLCD/Src/stm32f7xx_it.c        Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithLCD/Src/stm32f7xx_hal_msp.c   HAL MSP module    
    - DMA2D/DMA2D_MemToMemWithLCD/Src/system_stm32f7xx.c    STM32F7xx system clock configuration file

@par Hardware and Software environment  

  - This example runs on STM32F756xx/STM32F746xx devices.
  
  - This example has been tested with STMicroelectronics STM327x6G-EVAL revB 
    evaluation board and can be easily tailored to any other supported device 
    and development board. 


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   