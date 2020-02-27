/**
  @page FreeRTOS_ThreadCreation FreeRTOS Thread Creation application

  @verbatim
  ******************** (C) COPYRIGHT 2018 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_ThreadCreation/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Thread Creation application with execution from external memory
  *          AN5188.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  @endverbatim

@par Application Description

How to implement thread creation using CMSIS RTOS API with execution from external memory.

This application creates two threads with the same priority, which execute in
a periodic cycle of 18 seconds.

In the first 6 seconds, the thread 1 toggles LED5 each 200 ms and the 
thread 2 toggles LED6 each 500 ms.
In the following 6 seconds, the thread 1 suspends itself and the thread 2
continue toggling LED6.
In the last 6 seconds, the thread 2 resumes execution of thread 1 then
suspends itself, the thread 1 toggles the LED5 each 400 ms.

  This application provide different configuration for linker files which allows different eXecution in Place (XiP) schemas
  Supported configuration by STM32F7308-Discovery:
    - XiP From QSPI, DATA on Internal SRAM
    - XiP From QSPI, DATA on External PSRAM
  Make sure that choosen config matches ExtMem_Boot config in memory.h file.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size config TOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to
      the OS resources memory requirements of the application with +10% margin and rounded to the
      upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications
on STM32Cube with RTOS".


@par Keywords

RTOS, FreeRTOS, Thread

@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      In case of constraints it is possible to configure the MPU as "Write through/not shareable" to guarantee the cache coherence at write access but the user
      has to ensure the cache maintenance at read access though.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@par Directory contents

 - "FreeRTOS_ThreadCreation/Inc": contains the FreeRTOS_ThreadCreation firmware header files
    - FreeRTOS_ThreadCreation/Inc/main.h                Main program header file
    - FreeRTOS_ThreadCreation/Inc/stm32f7xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS_ThreadCreation/Inc/stm32f7xx_it.h        Interrupt handlers header file
    - FreeRTOS_ThreadCreation/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file

 - "FreeRTOS_ThreadCreation/Src": contains the FreeRTOS_ThreadCreation firmware source files
    - FreeRTOS_ThreadCreation/Src/main.c                Main program
    - FreeRTOS_ThreadCreation/Src/stm32f7xx_it.c        Interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32F730xx devices.

  - This application has been tested with STMicroelectronics STM32F7308-DISCO
    boards and can be easily tailored to any other supported device
    and development board.


@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Use corresponding project configuration for this application (FreeRTOS_ThreadCreation).
  4. Program the external QSPI memory with this application application (see below).
  5. Start debugging user application or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the this application to the external memory:
   - Open your preferred toolchain
      - Open the Project
      - Use project matching ExtMem_Boot selected configuration
      - Rebuild all files:
      - Run & debug the program:
      - For an XiP configuration (eXecute in Place from QSPI):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using SW4STM32 :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "MX25L512G_STM32F7308-DISCO"
            - From Erasing & Programming menu, browse and open the output binary file relative to this application
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
