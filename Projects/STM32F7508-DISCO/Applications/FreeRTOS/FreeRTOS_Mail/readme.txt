/**
  @page FreeRTOS_Mail FreeRTOS Mail Application

  @verbatim
  ******************** (C) COPYRIGHT 2018STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_Mail/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the FreeRTOS Mail example.
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

How to use mail queues with CMSIS RTOS API.

This application provide different configuration with linker files which allows different eXecution schemas
Supported configuration by STM32F7508-DISCO:
  - XiP From QSPI, Data on Internal SRAM
  - XiP From QSPI, Data on External SDRAM
  - BootROM : Execution From External SDRAM , Data on Internal SRAM

This application creates two threads that send and receive mail the mail to send/receive
is a structure that holds three variables (var1 and var2 are uint32, var3 is a uint8)

One thread acts as a producer and the other as the consumer.
The consumer has a higher priority than the producer and is set to block on mail receiving.

The Mail queue has space for one item. The producer allocates the mail and puts it in the mail queue.
As soon as the producer posts a mail to the queue the consumer will unblock, preempt the producer,
get the mail and free it.

Add the following variables to LiveWatch, the three producer values must respectively remain
equal to the three consumer values all the time:
 - ConsumerValue1 must remain equal to ProducerValue1
 - ConsumerValue2 must remain equal to ProducerValue2
 - ConsumerValue3 must remain equal to ProducerValue3

The inboard's LEDs can be used to monitor the application status:
  - LED1 should toggle when the example runs successfully.
  - LED1 is ON when any error occurs.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to
      the OS resources memory requirements of the application with +10% margin and rounded to the
	  upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications
on STM32Cube with RTOS".


@par Keywords

RTOS, FreeRTOS, Thread, Mail, Queues,

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

    - FreeRTOS/FreeRTOS_Mail/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_Mail/Inc/stm32f7xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Mail/Inc/stm32f7xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Mail/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_Mail/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_Mail/Src/stm32f7xx_it.c        Interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32F7508xx devices.

  - This application has been tested with STM32F7508-DISCO boards and can be
    easily tailored to any other supported device and development board.

  - STM32F7508-DISCO Set-up
  - None

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
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
