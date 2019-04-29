/**
  @page ADC_RegularConversion_Polling conversion using Polling

  @verbatim
  ******************************************************************************
  * @file    ADC/ADC_RegularConversion_Polling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the ADC RegularConversion Polling example.
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

How to use the ADC in Polling mode to convert data through the HAL API.

When the end of conversion occurs, the converted data of ADC3 DR register is 
affected to the uhADCxConvertedValue variable.

In this example, the system clock is 216MHz, APB2 = 108MHz and ADC clock = APB2/4. 
Since ADC3 clock is 27 MHz and sampling time is set to 3 cycles, the conversion 
time to 12bit data is 12 cycles so the total conversion time is (12+3)/27= 0.55us (1.8Msps).

User can vary the ADC_CHANNEL_8 voltage using the Eval Board potentiometer (RV1) connected to PF.10.
The converted value is monitored through debugger: uhADCxConvertedValue variable.

The uhADCxConvertedValue read value is coded on 12 bits, the Vref+ reference voltage is connected 
on the board to VDD (+3.3V), the Vref- reference voltage is connected on the board to the ground.
To convert the read value in volts, here is the equation to apply :
Voltage = uhADCxConvertedValue * (Vref+ - Vref-) / (2^12) = uhADCxConvertedValue * 3.3 / 4096

STM32 Eval board's LEDs can be used to monitor the transfer status:
  - LED3 is ON when there are an error in initialization.

@par Keywords

Analog, ADC, Analog to Digital Converter, Regular Conversion, Polling, Single Conversion

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

  - ADC/ADC_RegularConversion_Polling/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_RegularConversion_Polling/Inc/stm32f7xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_RegularConversion_Polling/Inc/main.h                  Header for main.c module  
  - ADC/ADC_RegularConversion_Polling/Src/stm32f7xx_it.c          DMA interrupt handlers
  - ADC/ADC_RegularConversion_Polling/Src/main.c                  Main program
  - ADC/ADC_RegularConversion_Polling/Src/stm32f7xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_RegularConversion_Polling/Src/system_stm32f7xx.c      STM32F7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F756xx/STM32F746xx devices.
  
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.
    
  - STM327x6G-EVAL revB Set-up
    - Use the Potentiometer (RV1) of the Eval board (connected to  PF.10).
    - JP24 must be fitted in 1-2 positions
    - JP23 must be fitted in 2-3 positions
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
