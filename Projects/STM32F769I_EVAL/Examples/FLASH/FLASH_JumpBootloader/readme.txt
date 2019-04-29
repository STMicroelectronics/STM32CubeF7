/**
  @page FLASH_JumpBootloader FLASH Jump to BootLoader example
  
  @verbatim
  ******************************************************************************
  * @file    FLASH/FLASH_JumpBootloader/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FLASH Jump to BootLoader example.
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

This example describes how to jump to BootLoader. 

In this example:

    - PJ.00 IO (configured in output pushpull mode) toggle in a forever loop.
      On UNIBOARD TFBGA216, this IO should be connected to LED2.

    - EXTI15_10 is connected to PC.13 pin
      - when falling edge is detected on EXTI15_10 by pressing S1 button, the jump to bootloader
	    is executed, and LED2 is turned off.
		
    - Check that the BootlLoader is recognized with the "DFU Tester" tool.
      This tool can be downloaded from www.st.com	

In this example, HCLK is configured at 216 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Memory, Flash, Jump, Bootloader,

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

  - FLASH/FLASH_JumpBootloader/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - FLASH/FLASH_JumpBootloader/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - FLASH/FLASH_JumpBootloader/Inc/main.h                  Header for main.c module  
  - FLASH/FLASH_JumpBootloader/Src/stm32f7xx_it.c          Interrupt handlers
  - FLASH/FLASH_JumpBootloader/Src/main.c                  Main program
  - FLASH/FLASH_JumpBootloader/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with UNIBOARD TFBGA216 13X13X0.8 CS board and can be
    easily tailored to any other supported device and development board.
	
  - UNIBOARD TFBGA216 Set-up
    - Connect LED2 pin to PJ0,
    - Connect S1 button pin to PC13,
	- Connect the board USB Micro B Connector CN6 to the PC,

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
