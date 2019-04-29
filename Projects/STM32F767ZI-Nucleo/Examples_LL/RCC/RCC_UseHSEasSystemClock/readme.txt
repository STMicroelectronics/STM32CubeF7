/**
  @page RCC_UseHSEasSystemClock RCC Clock Config example

  @verbatim
  ******************************************************************************
  * @file    Examples_LL/RCC/RCC_UseHSEasSystemClock/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the RCC HSE start example.
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  @endverbatim

@par Example Description

Use of the RCC LL API to start the HSE and use it as system clock.

At start-up, HSI clock is used as system clock (default clock after reset) and then Systick is
configured at 1 ms using HSI_VALUE define.

Then, HSE is started and we wait for ready state. System clock is set to HSE thanks to IT
triggered by HSE ready.
- LED1 is turned ON to indicate that HSE is ready.
- LED1 is toggled with a timing of 1 second in case of error to switch SYSCLK to HSE or if HSE CSS
failure is detected (through NMI IT).

 @note In HSE bypass mode, input clock will come from the MCO from
       ST_LINK MCU. This frequency cannot be changed, and it is fixed
       at 8 MHz. To use MCO from ST_LINK you need to check the Board
       User Manual to know how to connect the MCO pin to the STM32 device.

@par Keywords

System, RCC, PLL, PLLCLK, SYSCLK, HSE, Clock, Oscillator, HSI

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

  - RCC/RCC_UseHSEasSystemClock/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - RCC/RCC_UseHSEasSystemClock/Inc/main.h                  Header for main.c module
  - RCC/RCC_UseHSEasSystemClock/Inc/stm32_assert.h          Template file to include assert_failed function
  - RCC/RCC_UseHSEasSystemClock/Src/stm32f7xx_it.c          Interrupt handlers
  - RCC/RCC_UseHSEasSystemClock/Src/main.c                  Main program
  - RCC/RCC_UseHSEasSystemClock/Src/system_stm32f7xx.c      STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F767xx devices.

  - This example has been tested with NUCLEO-F767ZI
    board and can be easily tailored to any other supported device
    and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
