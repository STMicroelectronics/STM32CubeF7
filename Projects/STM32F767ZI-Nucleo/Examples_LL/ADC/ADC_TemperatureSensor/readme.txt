/**
  @page ADC_TemperatureSensor ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_TemperatureSensor/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_TemperatureSensor example.
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

How to use an ADC peripheral to perform a single ADC conversion on the 
internal temperature sensor and calculate the temperature in degrees Celsius. 
This example uses the polling programming model (for interrupt or DMA 
programming models, refer to other examples).

This example is based on the STM32F7xx ADC LL API; 
peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.
Sequencer of group regular (default group available on ADC of all STM32 devices)
is configured to convert 1 channel: internal channel temperature sensor.

Example execution:
At each press on push button, the ADC performs 1 conversion of the selected channel.
Software polls for conversion completion.
When conversion is completed, main program reads conversion data
from ADC data register and stores it into a variable, LED1 is turned on.

ADC conversions raw data are computed to physical values
using LL ADC driver helper macro:
 - Value of temperature (unit: degree Celcius)

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "hADCxConvertedData_Temperature_DegreeCelsius": Value of temperature (unit: degree Celcius)

Connection needed:
None.

Other peripherals used:
  1 GPIO for push button
  1 GPIO for LED1

@par Keywords

Analog, ADC, Temperature Sensor, Single Conversion, Polling, Software trigger, Signal measurement,

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

  - ADC/ADC_TemperatureSensor/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - ADC/ADC_TemperatureSensor/Inc/main.h                  Header for main.c module
  - ADC/ADC_TemperatureSensor/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_TemperatureSensor/Src/stm32f7xx_it.c          Interrupt handlers
  - ADC/ADC_TemperatureSensor/Src/main.c                  Main program
  - ADC/ADC_TemperatureSensor/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
