/**
  @page PWR_CurrentConsumption PWR STANDBY example

  @verbatim
  ******************************************************************************
  * @file    PWR/PWR_CurrentConsumption/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the PWR Current Consumption example.
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

How to enter the Standby mode and wake-up from this mode by using an external
reset or the wakeup pin.

This projects is configured for STM32F750xx devices using STM32CubeF7 HAL and running on
STM32F7508-DISCO board from STMicroelectronics.

In this example, the code is executed from QSPI external memory while data are in internal
SRAM memory.

In the associated software, the system clock is set to 216 MHz and the SysTick is
programmed to generate an interrupt each 1 ms.

The system automatically enters STANDBY mode 3 sec. after start-up. The wake-up pin
is configured before enter low-power mode.

After wake-up from STANDBY mode, program execution restarts in the same way as after
a software RESET.

LED1 is used to monitor the system state as following:
 - LED1 ON  : system in RUN mode
 - LED1 OFF : system in STANDBY mode

These steps are repeated in an infinite loop.

@note This example can not be used in DEBUG mode due to the fact
      that the Cortex-M7 core is no longer clocked during low power mode
       so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, RTC, External reset, LSI,

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

  - PWR/PWR_STANDBY/Inc/stm32f7xx_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/Inc/stm32f7xx_it.h           Header for stm32f7xx_it.c
  - PWR/PWR_STANDBY/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY/Src/system_stm32f7xx.c       STM32F7xx system clock configuration file
  - PWR/PWR_STANDBY/Src/stm32f7xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                   Main program

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
  3. Program the external memory with your example (see below).
  4. Start debugging user application or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the user application (this example) to the external memory:
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
            - From Erasing & Programming menu, browse and open the output binary file relative to this example
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
