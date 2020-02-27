/**
  @page ADC_GroupsRegularInjected ADC example
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/ADC/ADC_GroupsRegularInjected/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC_GroupsRegularInjected example.
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

How to use an ADC peripheral with both ADC groups (regular and injected) 
in their intended use cases.

This example is based on the STM32F7xx ADC LL API; 
peripheral initialization done using LL unitary services functions
for optimization purpose (performance and size).

Description of ADC group regular and group injected intended use case:
 - group regular for a high number of conversions and continuous
   data stream, with DMA transfer capability.
 - group injected for punctual conversions inserted between
   conversions of group regular.


Example configuration:
ADC is configured to use the 2 groups:
 - ADC group regular:
   ADC is configured to convert a single channel (1 channel from a GPIO),
   in single conversion mode, from HW trigger: timer peripheral .
 - ADC group injected:
   ADC is configured to convert a single channel (1 internal channel: VrefInt),
   in single conversion mode, from SW trigger.
Other peripherals are configured to be used with ADC group regular:
DMA is configured to transfer conversion data in an array (DMA transfer can be used
only with ADC group regular).
A timer is configured in time base and to generate TRGO events (trig from
timer can be used with both ADC groups regular and injected).

Example execution:
From the main program execution, the ADC group regular converts the selected channel
at each trig from timer.
DMA transfers conversion data to the array, DMA transfer complete interruption occurs.
Results array is updated indefinitely (DMA in circular mode).

For debug: variables to monitor with debugger watch window:
 - "aADCxConvertedData": ADC group regular conversion data
 - "uhADCxGrpInjectedConvertedData": ADC group injected conversion data
 - "ubDmaTransferStatus": status of DMA transfer of ADC group regular conversions
 - "ubAdcGrpInjectedUnitaryConvStatus": status of ADC group injected unitary conversion

From the first press on push button, the ADC group injected converts the selected channel
from a SW conversion start command.
ADC group injected conversion complete interruption occurs, SW transfers
conversion data to a variable, LED1 is turned on.

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
  Timer

@par Keywords

Analog, ADC, Analog to Digital, regular group, single conversion mode, HW trigger, 

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

  - ADC/ADC_GroupsRegularInjected/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - ADC/ADC_GroupsRegularInjected/Inc/main.h                  Header for main.c module
  - ADC/ADC_GroupsRegularInjected/Inc/stm32_assert.h          Template file to include assert_failed function
  - ADC/ADC_GroupsRegularInjected/Src/stm32f7xx_it.c          Interrupt handlers
  - ADC/ADC_GroupsRegularInjected/Src/main.c                  Main program
  - ADC/ADC_GroupsRegularInjected/Src/system_stm32f7xx.c      STM32F7xx system source file


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
