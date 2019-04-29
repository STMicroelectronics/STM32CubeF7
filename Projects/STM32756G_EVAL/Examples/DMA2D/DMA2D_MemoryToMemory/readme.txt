/**
  @page DMA2D_MemoryToMemory DMA2D Memory to Memory example
  
  @verbatim
  ******************************************************************************
  * @file    DMA2D/DMA2D_MemoryToMemory/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the STM32F7xx DMA2D Memory to Memory example.
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

How to configure the DMA2D peripheral in Memory-to-memory transfer mode.

The example transfers with DMA2D an internal SRAM static texture array aBufferInput[] of size 16x32 
and format ARGB4444 (16 bpp) to destination buffer in internal SRAM in array aBufferResult[].
The number of bytes transferred is then 16x32x2=1024 bytes.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the SYSTICK.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.

In this basic example the goal is to explain the different fields of the DMA2D 
structure in the case of Memory_to_Memory transfer mode

After DMA2D configuration, the data transfer is performed and then the transferred 
data is checked by byte to byte memory comparing of the source and destination buffers aBufferInput[] and aBufferResult[].   

STM327x6G-EVAL board revB's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the DMA2D transfer is complete.
 - LED2 is ON when there is a DMA2D transfer error or when data are not correctly transferred.
 - LED3 is ON when there is an error in DMA2D transfer/Init process.
 - LED4 is ON when the data are correctly transferred : that is no error in comparison source and destination buffers.
If everything all right, at the end of the test, LED1 and LED4 must be ON, LED2 and LED3 must be OFF.
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, DMA2D, LCD, SRAM, ARGB4444, Memory to Memory

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

  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - DMA2D/DMA2D_MemoryToMemory/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_MemoryToMemory/Inc/main.h                  Header for main.c module  
  - DMA2D/DMA2D_MemoryToMemory/Src/stm32f7xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_MemoryToMemory/Src/stm32f7xx_hal_msp.c     HAL MSP module
  - DMA2D/DMA2D_MemoryToMemory/Src/main.c                  Main program
  - DMA2D/DMA2D_MemoryToMemory/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
