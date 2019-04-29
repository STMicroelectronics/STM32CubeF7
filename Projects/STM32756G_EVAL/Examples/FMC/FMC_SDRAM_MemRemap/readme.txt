/**
  @page FMC_SDRAM_MemRemap SDRAM memory remap functionnalities example
  
  @verbatim
  ******************************************************************************
  * @file    FMC/FMC_SDRAM_MemRemap/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FMC SDRAM memory remap example.
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

This example guides you through the different configuration steps to use the IS42S32800G 
SDRAM memory (mounted on STM327x6G-EVAL revB evaluation board) as code execution memory. 
In addition, in this example, the SDRAM is used as data memory.

A swap between the FMC SDRAM banks and FMC NOR/PSRAM is implemented in order to enable 
the code execution from SDRAM Banks without modifying the default MPU attribute.
 
At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.
  
The example scenario does not reflect a real application case, its purpose is to
provide only the procedure to follow to use the external SDRAM as data & execution memory.

This example does not use the default library startup file. It uses a modified 
startup file provided with the example. While startup, the SDRAM memory is configured 
and initialized to be ready to contain data.

The user has to configure his preferred toolchain using the provided linker file.
The RAM zone is modified in order to use the external SDRAM memory as a RAM.
In addition, in the linker file, a new memory section ".sdram" is added, where code
can be executed.

At this stage, all the used data can be located in the external SRAM memory.
In addition, a pragma instruction is used for SystemClock_Config() function to be executed on SDRAM.

The user can use the debugger's watch to evaluate "uwTabAddr" and "MSPValue" variables
values which should be above 0x60000000 (SDRAM offset after implementing memory mapping swap).

Eval board's LEDs can be used to monitor the example status:
1. Check SDRAM as Data Memory:
If uwTabAddr and MSPValue values are in the external SDRAM memory, LED1 is ON, otherwise the LED3 is toggling.

2. Check SDRAM as Execution Memory:
LED3 is ON when executing the SystemClock_Config function from SDRAM.

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
      
@par Keywords

Memory, FMC, SDRAM, Read, Write, Initialization, Access, Memory remap, Code execution, NOR, PSRAM,

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

  - FMC/FMC_SDRAM_MemRemap/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - FMC/FMC_SDRAM_MemRemap/Inc/main.h                  Header for main.c module  
  - FMC/FMC_SDRAM_MemRemap/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - FMC/FMC_SDRAM_MemRemap/Src/main.c                  Main program
  - FMC/FMC_SDRAM_MemRemap/Src/stm32f7xx_it.c          Interrupt handlers
  - FMC/FMC_SDRAM_MemRemap/Src/system_stm32f7xx.c      STM32F7xx system source file


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
