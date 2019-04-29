/**
  @page CRC_PolynomialUpdate CRC operation with Polynomial update (HAL/LL mixed usage example)
  
  @verbatim
  ******************************************************************************
  * @file    CRC/CRC_PolynomialUpdate/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of CRC operation including user updates on generating polynomial.
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

How to use the CRC peripheral through the STM32F7xx CRC HAL and LL API.
The LL API is used for performance improvement.

The CRC (Cyclic Redundancy Check) calculation unit computes a 8-bit long 
CRC code of a given buffer of 32-bit data words, based on a user-defined generating polynomial; 
In this example, the polynomial is first set manually to 0x9B that is X^8 + X^7 + X^4 + X^3 + X + 1;
Then, in a second step, generating polynomial value and length are updated (set to 0x1021 that is
X^16 + X^12 + X^5 + 1), for new CRC calculation;
These updates are performed using CRC LL API.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK).

The CRC peripheral configuration is ensured by HAL_CRC_Init() function.
The latter is calling HAL_CRC_MspInit() function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK). 
You can update HAL_CRC_Init() input parameters to change the CRC configuration.

For each computation, calculated CRC code is stored in uwCRCValue variable.
Once calculated, the CRC value (uwCRCValue) is compared to the CRC expected value (uwExpectedCRCValue1 and uwExpectedCRCValue2).

STM32 board LEDs are used to monitor the example status:
  - LED1 is ON when correct CRC values are calculated
  - LED3 is ON when there is an error in initialization or if an incorrect CRC value is calculated.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Security, CRC, CRC Polynomial, IEC 60870-5, hardware CRC, user-defined, generating polynomial, polynomial update

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
  
  - CRC/CRC_PolynomialUpdate/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CRC/CRC_PolynomialUpdate/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - CRC/CRC_PolynomialUpdate/Inc/main.h                  Header for main.c module
  - CRC/CRC_PolynomialUpdate/Src/stm32f7xx_it.c          Interrupt handlers
  - CRC/CRC_PolynomialUpdate/Src/main.c                  Main program
  - CRC/CRC_PolynomialUpdate/Src/stm32f7xx_hal_msp.c     HAL MSP module 
  - CRC/CRC_PolynomialUpdate/Src/system_stm32f7xx.c      STM32F7xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 