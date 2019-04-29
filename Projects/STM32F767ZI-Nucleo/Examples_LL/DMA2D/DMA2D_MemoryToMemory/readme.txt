/**
  @page DMA2D_MemoryToMemory DMA2D : Memory to Memory transfer example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/DMA2D/DMA2D_MemoryToMemory/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the DMA2D_MemoryToMemory example.
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

How to configure the DMA2D peripheral in Memory-to-memory transfer mode. The
example is based on the STM32F7xx DMA2D LL API. The peripheral
initialization uses LL unitary service functions for optimization purposes
(performance and size).

The example achieves a DMA2D transfer of an internal SRAM static texture array aBufferInput[] 
of size 16x32 and format ARGB4444 (16 bpp) to a destination buffer in internal SRAM in 
array aBufferResult[].
The number of bytes transferred is then 16x32x2=1024 bytes.

Peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).

After DMA2D configuration, the data transfer is performed in polling mode and then the transferred 
data are checked in comparing the source and destination buffers aBufferInput[] and aBufferResult[].

NUCLEO-F767ZI board LED1 is used to monitor the transfer status:
- LED1 is ON in case of successful transfer,
- LED1 is blinking in case of transfer error.

@par Keywords

Display, Graphic, DMA2D, SRAM, ARGB4444, Memory to Memory

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

  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_MemoryToMemory/Inc/main.h                  Header for main.c module
  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32_assert.h          Template file to include assert_failed function
  - DMA2D/DMA2D_MemoryToMemory/Src/stm32f7xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_MemoryToMemory/Src/main.c                  Main program
  - DMA2D/DMA2D_MemoryToMemory/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.

  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
