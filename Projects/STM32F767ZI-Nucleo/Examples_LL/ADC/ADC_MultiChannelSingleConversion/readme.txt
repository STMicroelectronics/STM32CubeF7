/**
  @page ADC_MultiChannelSingleConversion ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_MultiChannelSingleConversion/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_MultiChannelSingleConversion example.
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

How to use an ADC peripheral to convert several channels. ADC conversions are 
performed successively in a scan sequence.

This example is based on the STM32F7xx ADC LL API; 
peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).

Example configuration:
ADC is configured in single conversion mode, from SW trigger.
Sequencer of group regular (default group available on ADC of all STM32 devices)
is configured to convert 3 channels: 1 channel from GPIO, 2 internal channels:
temperature sensor and VrefInt.
DMA is configured to transfer conversion data in an array of size three (one array
address for conversion data of each channel), in circular mode to update
results array at each ADC sequence conversions.

Example execution:
At each press on push button, the ADC executes the scan sequence: 
ADC performs conversion of the three selected channels successively,
in burst (discontinuous mode not enabled).
The 3 ADC conversions data are transferred by DMA into the results array
"aADCxConvertedData".
ADC conversion data of each sequence rank is at an address of the array:
 - aADCxConvertedData[0]: ADC channel set on rank1 (GPIO as analog input)
  GENERATOR_ PCH: Error missing target in ProjectGenerator. Align on targets used with "aADCxConvertedData[1]" in main.c
LED1 is turned on when ADC conversions and DMA transfer of the sequence
are completed.

ADC conversions raw data are computed to physical values
using LL ADC driver helper macro:
 - Value of voltage on GPIO pin (on which is mapped ADC channel) (unit: mV)
 - Value of internal voltage reference VrefInt (unit: mV)
 - Value of temperature (unit: degree Celcius)
 - Value of analog reference voltage (Vref+), connected to analog voltage supply Vdda (unit: mV)

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data
 - "ubDmaTransferStatus": status of DMA transfer of ADC group regular conversions
 - "uhADCxConvertedData_VoltageGPIO_mVolt":        Value of voltage on GPIO pin (on which is mapped ADC channel) (unit: mV)
 - "uhADCxConvertedData_VrefInt_mVolt":            Value of internal voltage reference VrefInt (unit: mV)
 - "hADCxConvertedData_Temperature_DegreeCelsius": Value of temperature (unit: degree Celcius)
 - "uhADCxConvertedData_VrefAnalog_mVolt":         Value of analog reference voltage (Vref+), connected to analog voltage supply Vdda (unit: mV)

Connection needed:
None.
Note: Optionally, a voltage can be supplied to the analog input pin (cf pin below),
      between 0V and Vdda=3.3V, to perform a ADC conversion on a determined
      voltage level.
      Otherwise, this pin can be let floating (in this case ADC conversion data
      will be undetermined).

Other peripherals used:
  1 GPIO for push button
  1 GPIO for LED1
  1 GPIO for analog input: PA.04 (Right connector CN7 pin 17)
  DMA

@par Keywords

Analog, ADC, Analog to Digital, Multi Channel, Conversion, DMA, Single, Measurement

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

  - ADC/ADC_MultiChannelSingleConversion/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - ADC/ADC_MultiChannelSingleConversion/Inc/main.h                  Header for main.c module
  - ADC/ADC_MultiChannelSingleConversion/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_MultiChannelSingleConversion/Src/stm32f7xx_it.c          Interrupt handlers
  - ADC/ADC_MultiChannelSingleConversion/Src/main.c                  Main program
  - ADC/ADC_MultiChannelSingleConversion/Src/system_stm32f7xx.c      STM32F7xx system source file


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
