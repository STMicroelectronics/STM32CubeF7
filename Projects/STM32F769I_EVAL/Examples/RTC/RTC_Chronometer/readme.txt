/**
  @page RTC_Chronometer RTC Chronometer Example
  
  @verbatim
  ******************************************************************************
  * @file    RTC/RTC_Chronometer/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the RTC Chronometer example.
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

This example illustrates how to simulate a precise chronometer with sub second feature.
The trials stored in the Backup registers (16 registers for time (second, minutes and hours) 
and 16 registers for subsecond).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.

The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This later is calling the HAL_RTC_MspInit()function which core is implementing
the configuration of the needed RTC resources according to the used hardware (CLOCK, 
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

LSE oscillator clock is used as RTC clock source. 

HAL_RTC_SetTime()and HAL_RTC_SetDate() functions are then called to initialize the 
time and the date.

For this example an interactive human interface is developed using LCD and Push 
Buttons to allow user to use StopWatch with real time display.

After startup, a default 00:00:00.000 chronometer counter is displayed on the 
LCD, it corresponds to [Hours]:[minutes]:[seconds].[milliseconds].
User can manipulate the chronometer features using touchscreen and Tamper buttons:
    - press START button to start counter. 
    - press SEL button to save trials in the backup registers (max 16 actions).
    - press Tamper button to reset all the backup registers. 

LED1 is turned ON when the RTC configuration is done correctly.
LED3 is turned ON when The RTC error configuration.

The Trials are displayed on the LCD.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

System, RTC, Chronometer, wakeup timer, Backup registers, Counter, LSE, LSI, sub-second, StopWatch, HID, LCD

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

  - RTC/RTC_Chronometer/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - RTC/RTC_Chronometer/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - RTC/RTC_Chronometer/Inc/main.h                  Header for main.c module  
  - RTC/RTC_Chronometer/Src/stm32f7xx_it.c          Interrupt handlers
  - RTC/RTC_Chronometer/Src/main.c                  Main program
  - RTC/RTC_Chronometer/Src/stm32f7xx_hal_msp.c     HAL MSP module
  - RTC/RTC_Chronometer/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx Devices.
  - This example has been tested with STMicroelectronics STM32769I-EVAL
    board and can be easily tailored to any other supported device and 
    development board. 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
