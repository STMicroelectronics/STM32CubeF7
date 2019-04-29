/**
  @page FMC_SRAM_DataMemory SRAM memory functionnalities example
  
  @verbatim
  ******************************************************************************
  * @file    FMC/FMC_SRAM_DataMemory/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FMC SRAM example.
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

Guide through the different configuration steps by using HAL APIs to configure
the FMC controller to access the IS61WV102416BLL-10MLI SRAM memory mounted 
on STM327x6G-EVAL revB evaluation board (including heap and stack).
 
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.
  
The example scenario does not reflect a real application case, its purpose is to
provide only the procedure to follow to use the external SRAM as data memory.

This example does not use the default library startup file. It uses a modified 
startup file provided with the example. The user has to add the new startup 
file in the project source list. While startup, the SRAM memory is configured 
and initialized to be ready to contain data.

The user has to configure his preferred toolchain using the provided linker file.
The RAM zone is modified in order to use the external SRAM memory as a RAM.

At this stage, all the used data can be located in the external SRAM memory.

The user can use the debugger's watch to evaluate "uwTabAddr" and "MSPValue" variables
values which should be above 0x68000000.

If uwTabAddr and MSPValue values are in the external SRAM memory, LED1 is ON, otherwise the LED3 is ON.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, FMC, SRAM, Read, Write, Initialization, Access, Data Memory, Heap, Stack, 

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

  - FMC/FMC_SRAM_DataMemory/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - FMC/FMC_SRAM_DataMemory/Inc/main.h                  Header for main.c module  
  - FMC/FMC_SRAM_DataMemory/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - FMC/FMC_SRAM_DataMemory/Src/main.c                  Main program
  - FMC/FMC_SRAM_DataMemory/Src/stm32f7xx_it.c          Interrupt handlers
  - FMC/FMC_SRAM_DataMemory/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.
    
  - To use LED1, please ensure that JP24 is set in 2-3 position.
  - To use LED3, please ensure that JP23 is set in 2-3 position.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
