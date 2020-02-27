/**
  @page RTC_Alarm RTC Alarm Example

  @verbatim
  ******************************************************************************
  * @file    RTC/RTC_Alarm/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RTC Alarm example.
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

How to configure and generate an RTC alarm.

In this example, the code is executed from QSPI external memory while data is
in internalSRAM memory.

In this example, the Time is set to 16:30:00 and the Alarm must be generated after 
30 seconds at 16:30:30.

Before the 30 seconds are elapsed, LED1 is toggled with a period of one second.
It is turned ON when the RTC Alarm is generated correctly.
The current time is updated and displayed on the debugger in aShowTime variable.

In case of error, LED1 is toggled with a period of two hundred milliseconds.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock.

The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This latter is calling the HAL_RTC_MspInit()function which core is implementing
the configuration of the needed RTC resources according to the used hardware (CLOCK, 
PWR, RTC clock source and Backup). You may update this function to change RTC configuration.

@note LSE oscillator clock is used as RTC clock source by default.
      The user can use also LSI as RTC clock source. The user uncomment the adequate 
      line on the main.h file.
      @code
        #define RTC_CLOCK_SOURCE_LSE  
        /* #define RTC_CLOCK_SOURCE_LSI */
      @endcode

      LSI oscillator clock must be calibrated first to deliver exactly a 32 kHz signal. 
      LSE (when available on board) is delivered by a 32.768 kHz crystal.

This project  is configured for STM32F750xx devices using STM32CubeF7 HAL and running on 
STM32F7508-DISCO board from STMicroelectronics.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this application uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed
      at a maximum system clock frequency of 200MHz.

@note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      In case of constraints it is possible to configure the MPU as "Write through/not shareable" to guarantee the cache coherence at write access but the user
      has to ensure the cache maintenance at read access though.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 (Managing memory protection unit (MPU) in STM32 MCUs)
      Please refer to the AN4839 (Level 1 cache on STM32F7 Series)

@par Keywords

System, RTC, Alarm, wakeup timer, Backup domain, Counter, LSE, LSI

@par Directory contents

 - "RTC/RTC_Alarm/Inc": contains the example firmware header files
    - RTC/RTC_Alarm/Inc/main.h                   Main configuration file
    - RTC/RTC_Alarm/Inc/stm32f7xx_it.h           Interrupt handlers header file
    - RTC/RTC_Alarm/Inc/stm32f7xx_hal_conf.h     HAL Configuration file

 - "RTC/RTC_Alarm/Src": contains the example firmware source files
    - RTC/RTC_Alarm/Src/main.c                   Main program
    - RTC/RTC_Alarm/Src/stm32f7xx_hal_msp.c      Microcontroller specific packages initialization file.
    - RTC/RTC_Alarm/Src/stm32f7xx_it.c           Interrupt handlers
    - RTC/RTC_Alarm/Src/system_stm32f7xx.c       STM32F7xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32F750xx devices.
  - This example has been tested with STMicroelectronics STM32F7508-DISCO
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
     The default configuration is the right one: 
     - DATA_AREA set to USE_INTERNAL_SRAM
     - CODE_AREA set to USE_QSPI
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external memory with the example "Examples\RTC\RTC_Alarm" (see below).
  4. Start debugging user example or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load the example "Examples\RTC\RTC_Alarm" to the external memory:
   - Open your preferred toolchain
   - Open the Project
   - Use project matching ExtMem_Boot selected configuration
   - Rebuild all files
   - Run & debug the program:
      - For an XiP configuration (eXecute in Place from QSPI):
         - Using EWARM or MDK-ARM : Load project image from the IDE (Project->Debug)
         - Using SW4STM32 :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "N25Q128A_STM32F7508-DISCO"
            - From Erasing & Programming menu, browse and open the output binary file relative to this example
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */