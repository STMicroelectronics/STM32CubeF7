/**
  @page ADC_SingleConversion_TriggerSW_IT ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_MIX/ADC/ADC_SingleConversion_TriggerSW_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_SingleConversion_TriggerSW_IT example.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

How to use the ADC to perform a single ADC channel conversion at each 
software start. This example uses the interrupt programming model (for 
polling and DMA programming models, please refer to other examples). It is 
based on theSTM32F7xx ADC HAL and LL API. The LL API is used 
for performance improvement.

Example configuration:
ADC is configured to convert a single channel, in single conversion mode,
from SW trigger.
ADC interruption enabled: EOC (end of conversion of ADC group regular).

Example execution:
At each press on push button, the ADC performs 1 conversion of the selected channel.
When conversion is completed, ADC interruption occurs.
IRQ handler callback function reads conversion data from ADC data register
and stores it into a variable, LED1 is turned on.

For debug: variables to monitor with debugger watch window:
 - "uhADCxConvertedData": ADC group regular conversion data
 - "uhADCxConvertedData_Voltage_mVolt": ADC conversion data computation to physical values

Connection needed:
None, if ADC channel and DAC channel are selected on the same GPIO.
Otherwise, connect a wire between DAC channel output and ADC input.
See comment below related to literal "WAVEFORM_GENERATION".

Other peripherals used:
  1 GPIO for push button
  1 GPIO for LED
  1 GPIO for analog input: PA.04 (connector CN7 pin 17)
  1 GPIO for DAC channel output PA.04 (connector CN7 pin 17)

Compilation switches are available to customize configuration and execution of this example:

One compilation switch is available to generate a waveform voltage
for test (located in main.h):
 - If literal "WAVEFORM_GENERATION" is defined: For this example purpose, generates a
   waveform voltage on a spare DAC channel LL_DAC_CHANNEL_1 (pin PA.04)
   so user has just to connect a wire between DAC channel output and ADC input to run this example.
   If ADC channel and DAC channel are selected on the same GPIO, then no external wire is required.
   Waveform generation: waveform circular, shape of ramp: Voltage is increasing at each press on push button, 
                        from 0V to maximum range (Vdda) in 5 steps (with vdda=3.3V: 0V, 0.8V, 1.65V, 2.5V, 3.3V),
                        then starting back from 0V.
   and follows circular cycles: At clicks counter maximum value reached, counter is set back to 0.
 - If literal "WAVEFORM_GENERATION" is not defined: no voltage is generated, user has
   to connect a voltage source to the selected ADC channel input to run this example.

@par Keywords

Analog, ADC, Analog to Digital, single conversion, Software trigger, interrupt

@Note�If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
 �����then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
������The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
����� It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
������In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
������Even though the user must manage the cache coherence for read accesses.
������Please refer to the AN4838 �Managing memory protection unit (MPU) in STM32 MCUs�
������Please refer to the AN4839 �Level 1 cache on STM32F7 Series�

@par Directory contents 

  - ADC/ADC_SingleConversion_TriggerSW_IT/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - ADC/ADC_SingleConversion_TriggerSW_IT/Inc/main.h                  Header for main.c module
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/stm32f7xx_it.c          Interrupt handlers
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/main.c                  Main program
  - ADC/ADC_SingleConversion_TriggerSW_IT/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
