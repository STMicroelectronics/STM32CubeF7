/**
  @page FMC_SRAM_PreInitConfig FMC PSRAM PreInit Config example
  
  @verbatim
  ******************************************************************************
  * @file    FMC/FMC_PSRAM_PreInitConfig/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the code execution from FMC PSRAM memory example.
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

This example describes how to execute a part of the code from the PSRAM external memory.
To do this, a section is created where the function is stored.

The PSRAM is configured before to call main() function and the memory is then available
after the system reset. 

The configuration of the PSRAM is done in the file system_stm32f7xx.c file in the 
function SystemInit_ExtMemCtl(). It is called by the function SystemInit(), which
is the first function called by the Reset_Handler.

The code in executed in a forever loop. It is a LEDs toggling example, with some 
code mapped in the PSRAM and other in Flash, to show the jump from one memory 
to the other.

When a power-off is performed, the PSRAM memory content is corrupted.

LED5, and LED6 toggle in this forever loop.

In this example, HCLK is configured at 200 MHz.
The configuration of the Clock is done in the system_stm32f7xx.c by calling SetSysClk().
A specefic flashloader is required for the programming of the PSRAM memory.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

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

  - FMC/FMC_PSRAM_PreInitConfig/Inc/stm32f7xx_hal_conf.h HAL configuration file
  - FMC/FMC_PSRAM_PreInitConfig/Inc/stm32f7xx_it.h       Interrupt handlers header file
  - FMC/FMC_PSRAM_PreInitConfig/Inc/main.h               Header for main.c module  
  - FMC/FMC_PSRAM_PreInitConfig/Src/stm32f7xx_it.c       Interrupt handlers
  - FMC/FMC_PSRAM_PreInitConfig/Src/main.c               Main program
  - FMC/FMC_PSRAM_PreInitConfig/Src/system_stm32f7xx.c   STM32F7xx system source file
  - FMC/FMC_PSRAM_PreInitConfig/Src/stm32f7xx_hal_msp.c  HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F722xx/STM32F723xx/STM32F732xx/STM32F733xx devices.
    
  - This example has been tested with STM32F723E-DISCO board and can be
    easily tailored to any other supported device and development board.

  - STM32F723E-DISCO Set-up :
      Board is configured by default to access PSRAM external memory

  - A specific region for the PSRAM memory has been added in the scatter file 
    (STM32F722xx/STM32F723xx/STM32F732xx/STM32F733xx_flash.icf for IAR toolchain, 
    STM32F723E-DISCO.sct for Keil toolchain and linker file for SW4STM32)
    
The PSRAM external flash loader is not integrated with supported toolchains, it’s only supported with STM32
ST-Link Utility V4.1.0
To load the example, use STM32 ST-Link Utility to program both internal Flash and external PSRAM memory:
1- Open STM32 ST-Link Utility V4.1.0, click on "External Loader" from the bar menu then check 
   "IS66WV51216EBLL_STM32723-DISCO" box 
2- Connect the STM32F723E-DISCO board to PC with USB cable through CN1
3- Use the generated file named "STM32F723E-DISCO.hex" with STM32 ST-Link Utility
   to program both internal Flash and external PSRAM memory

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
