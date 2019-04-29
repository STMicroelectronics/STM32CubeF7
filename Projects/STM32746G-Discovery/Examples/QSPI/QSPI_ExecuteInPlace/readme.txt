/**
  @page QSPI_ExecuteInPlace QSPI execute in place example
  
  @verbatim
  ******************************************************************************
  * @file    QSPI/QSPI_ExecuteInPlace/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the code execution from QSPI memory example.
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

How to execute code from QSPI memory after code loading.

This example describes how to execute a part of the code from a QSPI memory. To do this, 
a section is created where the function is stored.

During startup, the QSPI memory is erased. Data are then copied from the initialization
section of the flash to the QSPI memory. Finally, the QSPI is configured in memory-mapped 
mode and the code is executed in a forever loop.

LED1 is toggled in a forever loop.
As soon as an error is returned by HAL API, LED1 is turned on.

In this example, HCLK is configured at 216 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on a variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise, the caller ISR process will be blocked.
      To change the SysTick interrupt priority, you have to use the HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond,
      to have correct HAL operation.

@par Keywords

Memory, QSPI, Execute in place, Erase, section, sector, memory-mapped mode

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

  - QSPI/QSPI_ExecuteInPlace/Inc/stm32f7xx_hal_conf.h HAL configuration file
  - QSPI/QSPI_ExecuteInPlace/Inc/stm32f7xx_it.h       Interrupt handlers header file
  - QSPI/QSPI_ExecuteInPlace/Inc/main.h               Header for main.c module  
  - QSPI/QSPI_ExecuteInPlace/Src/stm32f7xx_it.c       Interrupt handlers
  - QSPI/QSPI_ExecuteInPlace/Src/main.c               Main program
  - QSPI/QSPI_ExecuteInPlace/Src/system_stm32f7xx.c   STM32F7xx system source file
  - QSPI/QSPI_ExecuteInPlace/Src/stm32f7xx_hal_msp.c  HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F7xx devices.
    
  - This example has been tested on STM32746G-DISCOVERY board and can be
    easily tailored to any other supported device and/or development board.

  - STM32746G-DISCOVERY Set-up :
    - Board is configured by default to access QSPI memory
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
