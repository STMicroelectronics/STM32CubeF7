/**
  @page QSPI_ReadWriteDual_DMA QSPI Read/Write in DMA mode example
  
  @verbatim
  ******************************************************************************
  * @file    QSPI/QSPI_ReadWriteDual_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the QSPI Read/Write in DMA mode example.
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

This example describes how to use QSPI interface in dual mode. It erases part of the QSPI memory, 
writes data in DMA mode, reads data in DMA mode and compares the result in a forever loop.
In dual-flash mode,two external Quad-SPI Flash memories are accessed simultaneously.
Each of the Flash memories use the same CLK and optionally the same nCS signals, but
each have separate IO0, IO1, IO2, and IO3 signals.

LED1 toggles each time a new comparison is good
LED2 is on as soon as a comparison error occurs
LED3 is on as soon as an error is returned by HAL API

In this example, HCLK is configured at 216 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, QSPI, Erase, Read, Write, DMA, Dual mode

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

  - QSPI/QSPI_ReadWriteDual_DMA/Inc/stm32f7xx_hal_conf.h HAL configuration file
  - QSPI/QSPI_ReadWriteDual_DMA/Inc/stm32f7xx_it.h       Interrupt handlers header file
  - QSPI/QSPI_ReadWriteDual_DMA/Inc/main.h               Header for main.c module  
  - QSPI/QSPI_ReadWriteDual_DMA/Src/stm32f7xx_it.c       Interrupt handlers
  - QSPI/QSPI_ReadWriteDual_DMA/Src/main.c               Main program
  - QSPI/QSPI_ReadWriteDual_DMA/Src/system_stm32f7xx.c   STM32F7xx system source file
  - QSPI/QSPI_ReadWriteDual_DMA/Src/stm32f7xx_hal_msp.c  HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32769I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32769I-EVAL Set-up :
    - A Daughter Board, in which two external QSPI Flash memories are mounted, is needed for this example.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
