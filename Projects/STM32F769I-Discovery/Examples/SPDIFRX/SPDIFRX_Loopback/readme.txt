/**
  @page SPDIFRX SPDIFRX_Loopback  Description of the SPDIFRX_Loopback example

  @verbatim
  ******************************************************************************
  * @file    SPDIFRX/SPDIFRX_Loopback/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the SPDIFRX_Loopback example.
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

This example shows how to use the SPDIFRX HAL APIs to receive a data buffer
in polling mode.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.

This example aims to explain the basic initialization procedure of the SPDIFRX
Peripheral. Hence, it is limited to a basic data buffer tranmission between SPDIF_TX
(SAI configured in SPDIF-Tx mode) and SPDIFRX on the same board.

The SPDIFRX peripheral's configuration is ensured by HAL_SPDIFRX_Init() function.
The latter is calling HAL_SPDIFRX_MspInit() function who's implementing
the configuration of the needed SPDIFRX resources according to the used hardware (CLOCK).
User can update HAL_SPDIFRX_Init() input parameters to change the SPDIFRX configuration.

STM32F769I_Discovery LEDs are used to monitor the data transfer's status:
  - LED2 (GREEN) is ON when a correct buffer is received
  - LED1 (RED) is ON when a communication or an initialization error occurs.

@par Keywords

Communication, Audio, Spdif, Rx, S/PDIF, IEC-60958, IEC-61937, Loopback, Polling, SAI, SPDIF-Tx,
Data Transfer

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

  - SPDIFRX/SPDIFRX_Loopback/Src/main.c                  Main program
  - SPDIFRX/SPDIFRX_Loopback/Src/system_stm32f7xx.c      STM32F7xx system source file
  - SPDIFRX/SPDIFRX_Loopback/Src/stm32f7xx_it.c          Interrupt handlers
  - SPDIFRX/SPDIFRX_Loopback/Src/stm32f7xx_hal_msp.c     HAL MSP module
  - SPDIFRX/SPDIFRX_Loopback/Inc/main.h                  Main program header file
  - SPDIFRX/SPDIFRX_Loopback/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - SPDIFRX/SPDIFRX_Loopback/Inc/stm32f7xx_it.h          Interrupt handlers header file

@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx/STM32F777xx/STM32F767xx devices.

  - This example has been tested with STMicroelectronics STM32F769I_Discovery
    board and can be easily tailored to any other supported device
    and development board.

  - STM32F769I_Discovery Set-up :
    - Connect SPDIF_Tx (CN8) to SPDIF_Rx (CN12) with an AV cable.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 */
