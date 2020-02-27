/**
  @page ExtMem_CodeExecution Boot application for External memory execution Readme file

  @verbatim
  ******************************************************************************
  * @file    ExtMem_CodeExecution/ExtMem_Boot/readme.txt
  * @author  MCD Application Team
  * @brief   Description of External memory boot
  *          with reduced internal flash (ExtMem_CodeExecution on STM32F7xx devices) AN5188.
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

This directory contains a set of sources files and pre-configured projects that
describes how to build an application for execution from external memory using
the ExtMem_Boot firmware.

@note You must not touch any part of the system that been initialized by the
      ExtMem_Boot firmware to avoid system failure, more details in the AN5188.

The STM32F730xx and STM32F750xx value line come with only 64KB internal flash.
These value line are intended for code execution from external memories.
This application shows how to boot from internal flash, configure external memories
then jump to user application located on external memory.
on The STM32F723E-Discovery, user can select QSPI flash for code execution.

Define "CODE_AREA" , "DATA_AREA" and "BINARY_AREA" in file memory.h are used respectively
to select the Code execution area, the application data area and the binary storage area.
The ExtMem_Boot program is located at 0x08000000 (Internal flash).

Please Refer to the bellow "Table 1. IAP implementation on STM32F723E-Discovery" for different configurations and corresponding settings.

@par Keywords

External Memory Execution, Loader, Boot

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

 - "ExtMem_Boot/Inc": contains the ExtMem_Boot firmware header files
    - ExtMem_Boot/Inc/main.h                 The main include file of the project.
    - ExtMem_Boot/Inc/memory.h               This file provides all the headers for all external memory
                                                       control function along with configuration defines
    - ExtMem_Boot/Inc/memory_msp.h           Header for msp function needed for external memory initialization.
    - ExtMem_Boot/Inc/stm32f7xx_hal_conf.h   Library Configuration file
    - ExtMem_Boot/Inc/stm32f7xx_it.h         Header for stm32f7xx_it.c

 - "ExtMem_Boot/Src": contains the ExtMem_Boot firmware source files
    - ExtMem_Boot/Src/main.c                 Main program
    - ExtMem_Boot/Src/stm32f7xx_it.c         Interrupt handlers
    - ExtMem_Boot/Src/stm32f7xx_hal_msp.c    Microcontroller specific packages initialization file.
    - ExtMem_Boot/Src/system_stm32f7xx.c     STM32F7xx system source file
    - ExtMem_Boot/Src/memory_msp.c           This file contains external memory control function along
                                                       with configuration defines
    - ExtMem_Boot/Src/qspi.c                 This file contains configuration required to initialize QSPI
                                                       and configure in memory mapped mode.
    - ExtMem_Boot/Src/fmc.c                  This file contains configuration for all kind of memory
                                                       controlled by the FMC

@par Hardware and Software environment

  - This application runs on STM32F730xx devices.

  - This application has been tested with STM32F723E-Discovery and can be
    easily tailored to any other supported device and development board.

Table 1. Boot implementation on STM32F723E-Discovery
/****************|***** Configuration ********* |************* Implementation **************************|
|                |                              | QSPI Flash is used for code execution                 |
|                | DATA_AREA: USE_INTERNAL_SRAM | The user application image is linked and stored       |
|                |            USE_EXTERNAL_PSRAM| on the QSPI flash located at 0x90000000.              |
|                |                              |                                                       |
|                | CODE_AREA: USE_QSPI          |                                                       |
|----------------|----------------------------- |-------------------------------------------------------|

(1) User application location address is defined in the memory.h file as:
      -QSPI:   #define APPLICATION_ADDRESS           ((uint32_t)0x90000000)

Following configuration are supported by this boot firmware, Please refer to fmc.c, qspi.c
for exact memories part number:
  - XiP (Execute In Place):
      External Flash is configured and set to memory mapped mode.
      Set the Main Stack Pointer and then jump to application entry point.
        - QSPI : @ 0x90000000
            CODE_AREA   USE_QSPI

  - Volatile Memory :
      Memory Configuration (memory mapped mode)
        - Internal SRAM  : @ 0x20000000
             DATA_AREA    USE_INTERNAL_SRAM
        - External PSRAM : @ 0x60000000
             DATA_AREA    USE_EXTERNAL_PSRAM


@Note Configuration used in ExtMem_Boot and ExtMem_Application must be inline otherwise required resources
      will not be initialized as required.


@par How to use it?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h.
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Use corresponding project configuration for ExtMem_CodeExecution/ExtMem_Application/..
  4. Program the external memory with "ExtMem_CodeExecution/ExtMem_Applicaiton/.." (see below).
  5. Start debugging user application as usual or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the ExtMem_Application/... to th external memory:
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
            - From Erasing & Programming menu, browse and open the output hex file relative to the ExtMem_Application\... configuration
            - Load the hex file into the external QSPI flash using "Start Programming"

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
