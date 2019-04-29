/**
  @page Boot application for External memory execution Readme file

  @verbatim
  ******************************************************************************
  * @file    Templates/ExtMem_Boot/readme.txt
  * @author  MCD Application Team
  * @brief   Description of implementation of the AN5188 (External memory boot
  *          with reduced internal flash on STM32H7xx devices.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
   @endverbatim

@par Example Description

This directory contains a set of sources files and pre-configured projects that
describes how to build an application for execution from external memory using
the ExtMem_Boot firmware.

@note You must not touch any part of the system that been initialized by the
      ExtMem_Boot firmware to avoid system failure, more details in the AN5188.

The STM32F730xx and STM32F750xx value line come with only 64KB internal flash.
These value line are intended for code execution from external memories.
This example shows how to boot from internal flash, configure external memories
then jump to user application located on external memory.

On the STM32F7508_DISCO, user can select QSPI flash , external SDRAM for code execution.
In case of code execution from external SDRAM, the application binary can be stored
on the micro-SD card or on the QSPI flash.
The Boot example will then copy the application binary to the target execution memory.
User can select internal RAM or external SDRAM for application data memory

Define "CODE_AREA" , "DATA_AREA" and "BINARY_AREA" in file memory.h are used respectively
to select the Code execution area, the application data area and the binary storage area.
The ExtMem_Boot program is located at 0x08000000 (Internal flash).

Please Refer to the bellow "Table 1. IAP implementation on STM32F7508_Disco" for different configurations and corresponding settings.

@par Keywords

Middleware, External Memory Boot, Loader, Boot

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
    - ExtMem_Boot/Src/sdcard.c               This file contains configuration for µSD memory

@par Hardware and Software environment

  - This example runs on STM32F750xx devices.

  - This example has been tested with STM32F7508_DISCO and can be
    easily tailored to any other supported device and development board.

Table 1. IAP implementation on STM32F7508_DISCO
/****************|***** Configuration ********* |************* Implementation **************************|
|                |                              | QSPI Flash is used for code execution                 |
|                | DATA_AREA: USE_INTERNAL_SRAM | The user application image is linked and stored       |
|      XIP       |            USE_EXTERNAL_SDRAM| on the QSPI flash located at 0x90000000.              |
|                | CODE_AREA: USE_QSPI          |                                                       |
|----------------|----------------------------- |-------------------------------------------------------|
|                | BINARY_AREA: USE_SPI_NOR     | Code execution from external SDRAM                    |
|                |              USE_SDCARD      | The user application is linked to the SDRAM 0xC0000000|
|                | DATA_AREA: USE_INTERNAL_SRAM | the binary image stored on storage memory             |
|                | CODE_AREA: USE_EXTERNAL_SDRAM| either SDCard or SPI_Nor memory and will be copied    |
|                |                              | into SDRAM at boot                                    |
|    BootROM     |                              |                                                       |
|----------------|----------------------------- |-------------------------------------------------------|

(1) User application location address is defined in the memory.h file as:
      -QSPI:    #define APPLICATION_ADDRESS           ((uint32_t)0x90000000)
      -SDRAM:   #define APPLICATION_ADDRESS           ((uint32_t)0xC0000000)

To modify it, change the default value to the desired one. Note that the application must be linked
relatively to the new address too, for this application icf file shall be updated and APPLICATION_ADDRESS
in preprocessor defines shall be update as well.

Following configuration are supported by the firmware, Please refer to fmc.c and qspi.c for exact part number:
  - XiP (Execute In Place):
      External Flash is configured and set to memory mapped mode.
      Set the MSP and then jump to application start.
        - QSPI : @ 0x90000000
            CODE_AREA   USE_QSPI
        - SDRAM : @ 0xC0000000
            CODE_AREA   USE_EXTERNAL_SDRAM

  - Volatile Memory :
      Memory Configuration (memory mapped mode)
        - Internal SRAM  : @ 0x20000000
             DATA_AREA    USE_INTERNAL_SRAM
        - External SDRAM : @ 0xC0000000
             DATA_AREA    USE_EXTERNAL_SDRAM

Supported configuration by STM32F7508-DISCO:
    - XiP From QSPI, Data on Internal SRAM
    - XiP From QSPI, Data on External SDRAM
    - BootROM : Execution From External SDRAM , Data on Internal SRAM

@Note Configuration used in ExtMem_Boot and external memory application must be inline otherwise required resources
      will not be initialized as required.


@par How to use it?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h.
     The default configuration is: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with your application (see below).
  4. Start debugging user application or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the application to the external memory:
   - Open your preferred toolchain
      - Open the Project
      - Use project matching ExtMem_Boot selected configuration
      - Rebuild all files:
      - Run & debug the program:
      - For an XiP configuration (eXecute in Place from QSPI):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using SW4STM32 :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "N25Q128A_STM32F7508-DISCO" in case of XiP from QSPI
            - From Erasing & Programming menu, browse and open the output binary file relative to your application
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)

      - For a BootROM configuration (BootROM external SDRAM):
        - If BINARY_AREA is USE_SPI_NOR then use the STM32CubeProgarmmer tool, select QSPI external flash loader "N25Q128A_STM32F7508-DISCO"
          and load the Project.bin (application binary output file) to the QSPI memory at the address 0x90000000
        - If BINARY_AREA is USE_SDCARD then copy the Project.bin to a micro-SD to be plugged on CN3 and reset the board.
        - In order to debug the  external memory application, user shall attach the debugger, and perform a reset


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
