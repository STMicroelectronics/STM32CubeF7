/** 
  @page CORTEXM_MPU_Config Cortex-M7 MPU Write Through/Write Back Configuration example

  @verbatim
  ******************************************************************************
  * @file    CORTEXM/CORTEXM_MPU_Config/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CortexM MPU WT/WB config example.
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

This example presents the MPU feature. The example purpose is to configure SDRAM 
memorie area in Write-back mode using the MPU attributes.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 200 MHz.

The external SDRAM memory is configured in Write-back. The address start is 0xC0000000 with a size of 32 Mbyes.

The CPU L1-Cache is enabled since the AXI interface is used. It allows to increase
CPU performance.

In this example, the user can perform read/write operation on the SDRAM device.
A data buffer is written to the SDRAM memory, then read back and checked to
verify its correctness.

If the data is read correctly from SDRAM, the LED1 is ON, otherwise the LED3 is Toggled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, Cortex, Memory regions, Write-through, Write-back, SRAM, SDRAM, AXI, CPU L1-Cache, 

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

  - CortexM/CORTEXM_MPU_WTWBConfig/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CortexM/CORTEXM_MPU_WTWBConfig/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - CortexM/CORTEXM_MPU_WTWBConfig/Inc/main.h                  Header for main.c module  
  - CortexM/CORTEXM_MPU_WTWBConfig/Src/stm32f7xx_it.c          Interrupt handlers
  - CortexM/CORTEXM_MPU_WTWBConfig/Src/main.c                  Main program
  - CortexM/CORTEXM_MPU_WTWBConfig/Src/system_stm32f7xx.c      STM32F7xx system source file

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
