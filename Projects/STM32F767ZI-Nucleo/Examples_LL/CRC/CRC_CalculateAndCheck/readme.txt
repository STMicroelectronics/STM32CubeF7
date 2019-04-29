/**
  @page CRC_CalculateAndCheck CRC : CRC calculation and computed CRC value checking
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/CRC/CRC_CalculateAndCheck/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRC_CalculateAndCheck example.
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

How to configure the CRC calculation unit to compute a CRC code for a given data
buffer, based on a fixed generator polynomial (default value 0x4C11DB7). The
peripheral initialization is done using LL unitary service functions for
optimization purposes (performance and size).

CRC Peripheral is configured to work with default polynomial value (32-bit long).
Normal representation of this polynomial value is :
 X^32 + X^26 + X^23 + X^22 + X^16 + X^12 + X^11 + X^10 +X^8 + X^7 + X^5 + X^4 + X^2 + X + 1.
Generated CRC value is then 32 bits long.

Example execution:
After startup from reset and system configuration, CRC configuration is performed (use of default Polynomial and initialisation values).
CRC code of a given data buffer is computed. 
Data buffer length has been chosen as not an exact nb of u32 (32-bit words), in order to illustrate 
use of offered API for feeding the calculator (u32, u16 or u8 inputs).
The calculated CRC code is stored in uwCRCValue variable.
Once calculated, CRC value (uwCRCValue) is compared to the CRC expected value (uwExpectedCRCValue),
and if both are equal, LED1 is turned On. 
In case of errors, LED1 is blinking (1sec period).

@par Keywords

Security, CRC, CRC Polynomial, IEC 60870-5, hardware CRC, user-defined, generating polynomial, CRC Calculate

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

  - CRC/CRC_CalculateAndCheck/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - CRC/CRC_CalculateAndCheck/Inc/main.h                  Header for main.c module
  - CRC/CRC_CalculateAndCheck/Inc/stm32_assert.h          Template file to include assert_failed function
  - CRC/CRC_CalculateAndCheck/Src/stm32f7xx_it.c          Interrupt handlers
  - CRC/CRC_CalculateAndCheck/Src/main.c                  Main program
  - CRC/CRC_CalculateAndCheck/Src/system_stm32f7xx.c      STM32F7xx system source file


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
 