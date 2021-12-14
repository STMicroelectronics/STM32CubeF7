/**
  @page Templates  Description of the Templates example
  
  @verbatim
  ******************************************************************************
  * @file    Templates/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Templates example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

This projects provides a reference template that can be used to build any firmware application.

This projects is configured for STM32F769xx devices using STM32CubeF7 HAL and running on 
STM32F769I-EVAL board from STMicroelectronics.

This template provides two workspaces:
 - STM32769I_EVAL(AXIM-FLASH): using the Flash AXIM interface (Cache & MPU features)
 - STM32769I_EVAL(ITCM-FLASH): using the Flash ITCM interface (ART Accelerator)
 
The template includes also function used to enable the CPU Cache : CPU_CACHE_Enable().
This function is provided as template implementation that User may integrate in his application, 
to enhance the performance in case of use of AXI interface with several masters.

@note Some code parts can be executed in the ITCM-RAM (16 KB) which decrease critical task execution time, compared  
      to code execution from Flash memory. This feature can be activated using '#pragma location = ".itcmram"' to be 
      placed above function declaration, or using the toolchain GUI (file options) to execute a whole source file in
      the ITCM-RAM.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Reference, Template, CPU Cache

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

  - Templates/Src/main.c                 Main program
  - Templates/Src/system_stm32f7xx.c     STM32F7xx system clock configuration file
  - Templates/Src/stm32f7xx_it.c         Interrupt handlers 
  - Templates/Src/stm32f7xx_hal_msp.c    HAL MSP module
  - Templates/Inc/main.h                 Main program header file  
  - Templates/Inc/stm32f7xx_hal_conf.h   HAL Configuration file
  - Templates/Inc/stm32f7xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STMicroelectronics STM32F769I-EVAL
    boards and can be easily tailored to any other supported device 
    and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

In order to load the Flash_ITCM configuration, you must follow the instructions below :
- Open EWARM/SW4STM32 toolchain
- Rebuild all files
- Open STM32 ST-Link Utility V3.9.0
- Connect the STM32F769I_EVAL board to PC with USB cable through CN22
- Use the corresponding hex file with ST-Link Utility to program ITCM Flash :
  - EWARM: "Project.hex"
  - SW4STM32: "STM32F7x9I_EVAL(ITCM-FLASH).hex"


 */
