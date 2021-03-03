/**
  @page CEC CEC_DataExchange example
  
  @verbatim
  ******************************************************************************
  * @file    CEC/CEC_DataExchange/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CEC Data Exchange example.
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

How to configure and use the CEC peripheral to receive and transmit messages.

- Hardware Description

To use this example, two STM32746G-DISCOVERY boards (called Device_1 and 
Device_2) are loaded with the same software then connected through CEC lines

 /|\  In the firmware file main.h, uncomment the dedicated line to use
/_!_\ the CEC peripheral as STM32 device_1 or STM32 device_2.

@verbatim
*------------------------------------------------------------------------------*
|       STM32746G-DISCOVERY RevB                STM32746G-DISCOVERY RevB       |
|         Device Address :0x01                    Device Address :0x03         |
|         _____________________                  _____________________         |
|        |                     |                |                     |        |
|        |                     |                |                     |        | 
|        |  __________         |                |         __________  |        |
|        | |   CEC    |________|____CECLine_____|________|   CEC    | |        |
|        | | Device_1 | PA15   |                | PA15   | Device_2 | |        |
|        | |__________| CN7.D9 |                | CN7.D9 |__________| |        |
|        |                     |                |                     |        |
|        |   _____________     |                |   _____________     |        |
|        |  | Screen      |    |                |  | Screen      |    |        |
|        |  |             |    |                |  |             |    |        |
|        |  |_____________|    |                |  |_____________|    |        |
|        |                     |                |                     |        |
|        |             GND O---|----------------|--O GND              |        |
|        |_____________________|                |_____________________|        |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------**
@endverbatim


- Software Description

The test unrolls as follows.

On TX side, four possible messages can be transmitted and are indicated on the screen :
 - when touching top of the screen, send CEC command with opcode 0x44 and operand 0x41
 - when touching bottom of the screen, send CEC command with opcode 0x44 and operand 0x42
 - when touching left of the screen, send CEC command with opcode 0x46
 - when touching right of the screen, send CEC command with opcode 0x9F

On the RX side in case of successful reception, it displays the content of the received 
CEC command or displays an error message if transmition fails.

The two boards transmits commands to other by touching the screen and receives the 
commands from the other and displays the received command on the screen.

In this example, HCLK is configured at 200 MHz.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@par Keywords

Connectivity, CEC, Transmission, Reception, Joystick, Data exchange

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

  - CEC/CEC_DataExchange/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CEC/CEC_DataExchange/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - CEC/CEC_DataExchange/Inc/main.h                  Header for main.c module  
  - CEC/CEC_DataExchange/Src/stm32f7xx_it.c          Interrupt handlers
  - CEC/CEC_DataExchange/Src/system_stm32f7xx.c      STM32F7xx system source file
  - CEC/CEC_DataExchange/Src/main.c                  Main program
  - CEC/CEC_DataExchange/Src/stm32f7xx_hal_msp.c     IP hardware resources initialization
  
@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx  Devices.
    
  - This example has been tested with STM32746G-DISCOVERY RevB board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - in main.h, uncomment DEVICE_1 for first board, uncomment DEVICE_2 for second board
 - Rebuild all files and load your image into target memory
 - With a wire, connect PA15 signal connected to CN7.D9 between the 2 boards
 - Connect the ground of the 2 boards
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
