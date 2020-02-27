/**
  @page LedToggling Led toggling Application

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ExtMem_CodeExecution/ExtMem_Application/LedToggling/readme.txt
  * @author  MCD Application Team
  * @brief   Description of Led Toggling application with execution from external memory
  *          AN5188.
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
   @endverbatim

@par Application Description
This application provides sample LED toogling program with execution from external memory.

  This application toggle trun on the LED_RED for 1 second after boot. Then
  toggle the LED_GREEN each 200ms for 20 times and finally reset the microcontroller.

  This application provide different configuration for linker files which allows different eXecution in Place (XiP) schemas
  Supported configuration by STM32F723E-Discovery:
    - XiP From QSPI, Volatile on Internal SRAM
    - XiP From QSPI, Volatile on External PSRAM
  Make sure that chosen config matches ExtMem_Boot config in memory.h file.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this application uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed
      at a maximum system clock frequency of 200MHz.

@par Keywords

System, GPIO, Output, Alternate function, Toggle, AN5188, SDRAM, SRAM

@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      In case of constraints it is possible to configure the MPU as "Write through/not shareable" to guarantee the cache coherence at write access but the user
      has to ensure the cache maintenance at read access though.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 (Managing memory protection unit (MPU) in STM32 MCUs)
      Please refer to the AN4839 (Level 1 cache on STM32F7 Series)

@par Directory contents

 - "LedToggling/Inc": contains the FreeRTOS firmware header files
    - LedToggling/Inc/main.h                   Main configuration file
    - LedToggling/Inc/stm32f7xx_it.h           Interrupt handlers header file
    - LedToggling/Inc/stm32f7xx_hal_conf.h     HAL Configuration file

 - "LedToggling/Src": contains the LedToggling firmware source files
    - LedToggling/Src/main.c                   Main program
    - LedToggling/Src/stm32f7xx_hal_msp.c      Microcontroller specific packages initialization file.
    - LedToggling/Src/stm32f7xx_it.c           Interrupt handlers
    - LedToggling/Src/system_stm32f7xx.c       STM32F7xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32F730xx devices.

  - This example has been tested with STMicroelectronics STM32F723 Discovery
    boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in ExtMem_CodeExecution\ExtMem_Boot application.
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Use corresponding project configuration for ExtMem_Application/LedToggling.
  4. Program the external QSPI memory with "ExtMem_Application/LedToggling" application (see below).
  5. Start debugging user application or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the ExtMem_Application/LedToggling to th external memory:
   - Open your preferred toolchain
      - Open the Project
      - Use project matching ExtMem_Boot selected configuration
      - Rebuild all files:
      - Run & debug the program:
      - For an XiP configuration (eXecute in Place from QSPI):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using SW4STM32 :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "MX25L512G_STM32F723E-DISCO"
            - From Erasing & Programming menu, browse and open the output hex file relative to the ExtMem_Application\LedToggling configuration
            - Load the hex file into the external QSPI flash using "Start Programming"


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

