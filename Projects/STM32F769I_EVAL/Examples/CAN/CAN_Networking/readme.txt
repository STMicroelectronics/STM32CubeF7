/**
  @page CAN_Networking CAN Networking example
  
  @verbatim
  ******************************************************************************
  * @file    CAN/CAN_Networking/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CAN Networking example.
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

This example shows how to configure the CAN peripheral to send and receive 
CAN frames in normal mode. 

The sent frames are used to control Leds by pressing Tamper push-button.
	
The CAN serial communication link is a bus to which a number of units may be
connected. This number has no theoretical limit. Practically the total number
of units will be limited by delay times and/or electrical loads on the bus line.

This program behaves as follows:
  - After reset, all Leds are OFF on the N eval boards
  - By Pressing on Tamper push-button : LED1 turns ON and all other Leds are OFF, on the N
    eval boards connected to the bus. 
  - Press on Tamper push-button again to send CAN Frame to command LEDn+1 ON, all other Leds 
    are OFF on the N eval boards.
    
@note This example is tested with a bus of 3 units. The same program example is 
      loaded in all units to send and receive frames.
@note Any unit in the CAN bus may play the role of sender (by pressing Tamper push-button)
      or receiver.

  The CAN is configured as follows:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = external clock (HSE)
    - ID Filter  = All identifiers are allowed
    - RTR = Data
    - DLC = 1 byte
    - Data: Led number that should be turned ON

@par Keywords

Connectivity, CAN, Communication, Transmission, Reception, Polling, Networking, Send, Receive, 

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

  - CAN/CAN_Networking/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CAN/CAN_Networking/Inc/stm32f7xx_it.h          DMA interrupt handlers header file
  - CAN/CAN_Networking/Inc/main.h                  Header for main.c module  
  - CAN/CAN_Networking/Src/stm32f7xx_it.c          DMA interrupt handlers
  - CAN/CAN_Networking/Src/main.c                  Main program
  - CAN/CAN_Networking/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - CAN/CAN_Networking/Src/system_stm32f7xx.c      STM32F7xx system source file
  
@par Hardware and Software environment

  - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
  - This example has been tested with STM32F769I-EVAL board and can be
    easily tailored to any other supported device and development board.

  - STM32F769I-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PI.15, PJ.00, PJ.01
      and PJ.03 pins
    - Use Tamper push-button connected to PC.13
    - Connect a male/male CAN cable between at least 2 EVAL CAN connectors (CN26)
    - You have to configure the jumpers as follows:

  @verbatim
  
       +-------------+-----------------+
       |   Jumper    |       CAN1      |
       +-------------+-----------------+
       |   JP14      |      fitted     |
       |   JP19      |      1-2        |
       |   JP18      |      fitted     |
       +-------------+-----------------+
  @endverbatim
  
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 