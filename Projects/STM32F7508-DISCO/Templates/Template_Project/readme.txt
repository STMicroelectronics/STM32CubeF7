/**
  @page Templates  Description of the Templates example

  @verbatim
  ******************************************************************************
  * @file    readme.txt
  * @author  MCD Application Team
  * @brief   Description of the Template example.
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

This projects provides a reference template that can be used to build any firmware application with execution from external memory

This projects is configured for STM32F750xx devices using STM32CubeF7 HAL and running on 
STM32F7508-DISCO board from STMicroelectronics.

  This application provide different configuration with linker files which allows different eXecution schemas
  Supported configuration by STM32F7508-DISCO:
    - XiP From QSPI, Data on Internal SRAM
    - XiP From QSPI, Data on External SDRAM
    - BootROM : Execution From External SDRAM , Data on Internal SRAM

@note In BootROM configuration , application code is initially stored on sdcard or SPI nor then loaded to the SDRAM after boot.
@note Make sure that chosen configuration matches ExtMem_Boot configuration in memory.h file.

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

 - "Templates/Inc": contains the Templates firmware header files
    - Templates/Inc/main.h                   Main configuration file
    - Templates/Inc/stm32f7xx_it.h           Interrupt handlers header file
    - Templates/Inc/stm32f7xx_hal_conf.h     HAL Configuration file

 - "Templates/Src": contains the Templates firmware source files
    - Templates/Src/main.c                   Main program
    - Templates/Src/stm32f7xx_hal_msp.c      Microcontroller specific packages initialization file.
    - Templates/Src/stm32f7xx_it.c           Interrupt handlers
    - Templates/Src/system_stm32f7xx.c       STM32F7xx system clock configuration file


@par Hardware and Software environment

  - This application runs on STM32F750xx devices.

  - This application has been tested with STMicroelectronics STM32F7508-DISCO
    boards and can be easily tailored to any other supported device
    and development board.

 
@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with your application (this template , see below).
  4. Start debugging user application or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the user application (this template) to the external memory:
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
            - From Erasing & Programming menu, browse and open the output binary file relative to the application
       - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)

      - For a BootROM configuration (BootROM external SDRAM):
        - if BINARY_AREA is USE_SPI_NOR then use the STM32CubeProgarmmer tool, select QSPI external flash loader "N25Q128A_STM32F7508-DISCO"
          and load the Project.bin (application binary output file) to the QSPI memory at the address 0x90000000
        - if BINARY_AREA is USE_SDCARD then copy the Project.bin to a micro-SD to be plugged on CN3 and reset the board.
        - In order to debug the  Template project, user shall attach the debugger, and perform a reset


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 