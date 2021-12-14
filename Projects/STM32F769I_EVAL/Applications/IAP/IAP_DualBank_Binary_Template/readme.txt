/**
  @page IAP_DualBank_Binary_Template IAP Dual Bank Binary Template Readme file

  ******************************************************************************
  * @file    IAP/IAP_DualBank_Binary_Template/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IAP_DualBank_Binary_Template directory.
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
  */

@par Example Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the program load address at 0x08100000, using your toolchain linker file
2. Relocate the vector table at address 0x08100000, using the "NVIC_SetVectorTable"
   function.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
Four LEDs are toggled with a timing defined by the Delay function.

@par Keywords

IAP, In-Application Programming, Dual Bank, Binary, Flash memory, USART

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

 - IAP\IAP_DualBank_Binary_Template/Inc/stm32f7xx_hal_conf.h          HAL Configuration file
 - IAP\IAP_DualBank_Binary_Template/Inc/main.h                        Header for main.c module
 - IAP\IAP_DualBank_Binary_Template/Inc/stm32f7xx_it.h                Header for stm32f7xx_it.c
 - IAP\IAP_DualBank_Binary_Template/Src/main.c                        Main program
 - IAP\IAP_DualBank_Binary_Template/Src/stm32f7xx_it.c                Interrupt handlers
 - IAP\IAP_DualBank_Binary_Template/Src/system_stm32f7xx.c            STM32F7xx system clock configuration file 

     
@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This example has been tested with STM32F769I-EVAL and can be
    easily tailored to any other supported device and development board.

@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:
 
 - EWARM:
    - Open the Project.eww workspace
    - Rebuild all files
    - A binary file "STM32F769I-EVAL_SysTick.bin" will be generated under "STM32F769I-EVAL/Exe" folder.  
    - Finally load this image with IAP application

 - MDK-ARM:
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "/IAP_DualBank_Binary_Template/MDK-ARM" directory and run "axftobin.bat"
      (Fromelf Exe path might have to be updated in "axftobin.bat" file, according to your Keil setup).
    - A binary file "STM32F769I-EVAL_SysTick.bin" will be generated under "STM32F769I-EVAL" folder. 
    - Finally load this image with IAP application

 - System Workbench for STM32: 
    - Open System Workbench for STM32 toolchain
    - Browse to the SW4STM32 workspace directory, select the project 
      (.project file in \IAP_DualBank_Binary_Template\SW4STM32\STM32F769I-EVAL directory).
    - Rebuild all project files: Select the project in the "Project explorer" window 
      then click on Project->build project menu.
    - Load this image with the IAP application (Select option 1 in IAP menu)

 */
