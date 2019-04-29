/**
  @page ADC_TemperatureSensor conversion using the internal Temperature Sensor

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_TemperatureSensor/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC RegularConversion DMA example.
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

This example describes how to use the ADC1 and the Temperature Sensor to calculate
the junction temperature of the device.

The ADC1 is configured to convert continuously ADC_CHANNEL_TEMPSENSOR.

Each time an end of conversion occurs the DMA transfers, in circular mode, the
converted data from ADC1 DR register to the ConvertedValue variable.

The ConvertedValue read value is coded on 12 bits.

The Junction Temperature of the device is calculated via the formula:
JTemp = ((Vsens - V25)/Avg_Slope) + 25°C
 - Vsens = ConvertedValue * (Vref+ - Vref-) / (2^12 - 1) = ConvertedValue * 3.3 / 4095
 - V25 = 760 mV
 - Avg_Slope = 2.5 mV/C°

The calculated temperature is stored in the JTemp variable. The value is displayed
on the LCD and refreshed each 1s.

The internal temperature sensor supports a temperature range of –40 to 125°C with
an accuracy of +/-1.5°C.

STM32F769I-EVAL board's LEDs can be used to monitor the transfer status:
  - LED1 (Green LED) toggles each 1s when the ADC is converting.
  - LED3 (Red LED) blinks when error occurs in initialization.

@par Keywords

Analog, ADC, Temperatuer Sensor, Single Conversion, Polling, Software trigger, Signal measurement,
  
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

  - ADC/ADC_TemperatureSensor/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_TemperatureSensor/Inc/stm32f7xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_TemperatureSensor/Inc/main.h                  Header for main.c module  
  - ADC/ADC_TemperatureSensor/Src/stm32f7xx_it.c          DMA interrupt handlers
  - ADC/ADC_TemperatureSensor/Src/main.c                  Main program
  - ADC/ADC_TemperatureSensor/Src/stm32f7xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_TemperatureSensor/Src/system_stm32f7xx.c      STM32F7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
  
  - This example has been tested with STM32F769I-EVAL board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
