/**
  @page IAP_SingleBank_Binary_Template IAP Single Bank Binary Template Readme file

  ******************************************************************************
  * @file    IAP/IAP_SingleBank_Binary_Template/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the IAP_SingleBank_Binary_Template directory.
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

@par Example Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the program load address at 0x08008000, using your toolchain linker file
2. Relocate the vector table at address 0x08008000, using the "NVIC_SetVectorTable"
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

IAP, In-Application Programming, Single Bank, Binary, Flash memory, USART

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

 - IAP\IAP_SingleBank_Binary_Template/Inc/stm32f7xx_hal_conf.h          HAL Configuration file
 - IAP\IAP_SingleBank_Binary_Template/Inc/main.h                        Header for main.c module
 - IAP\IAP_SingleBank_Binary_Template/Inc/stm32f7xx_it.h                Header for stm32f7xx_it.c
 - IAP\IAP_SingleBank_Binary_Template/Src/main.c                        Main program
 - IAP\IAP_SingleBank_Binary_Template/Src/stm32f7xx_it.c                Interrupt handlers
 - IAP\IAP_SingleBank_Binary_Template/Src/system_stm32f7xx.c            STM32F7xx system clock configuration file 

     
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

 * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
 */
