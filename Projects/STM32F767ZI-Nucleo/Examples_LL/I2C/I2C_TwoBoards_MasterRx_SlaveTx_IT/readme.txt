/**
  @page I2C_TwoBoards_MasterRx_SlaveTx_IT I2C example (IT Mode)
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_TwoBoards_MasterRx_SlaveTx_IT I2C example (IT Mode).
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
  
How to handle the reception of one data byte from an I2C slave device 
by an I2C master device. Both devices operate in interrupt mode. The peripheral 
is initialized with LL unitary service functions to optimize for performance 
and size.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO and I2C peripherals using two NUCLEO-F767ZI.

Boards: NUCLEO-F767ZI (embeds a STM32F767ZI device)
SCL Pin: PB.10 (CN10, pin 32)
SDA Pin: PB.11 (CN10, pin 34)

   ______BOARD SLAVE_____                       _____BOARD MASTER_____
  |        ______________|                     |______________        |
  |       |I2C2          |                     |          I2C1|       |
  |       |              |                     |              |       |
  |       |          SCL |_____________________| SCL          |       |
  |       |              |                     |              |       |
  |       |              |                     |              |       |
  |       |          SDA |_____________________| SDA          |       |
  |       |______________|                     |______________|       |
  |         __           |                     |             __       |
  |        |__|          |                     |            |__|      |
  |        USER       GND|_____________________|GND         USER      |
  |___STM32F7xx_Nucleo_144___|                     |___STM32F7xx_Nucleo_144___|

The project is splitted in two parts the Master Board and the Slave Board
- Master Board
  I2C1 Peripheral is configured in Master mode with EXTI (Fast Mode @400kHz).
And GPIO associated to User push-button is linked with EXTI. 
- Slave Board
  I2C2 Peripheral is configured in Slave mode with EXTI (Fast Mode @400kHz, Own address 7-bit enabled).

The user can choose between Master and Slave through "#define SLAVE_BOARD"
in the "main.h" file:
- Comment "#define SLAVE_BOARD" to select Master board.
- Uncomment "#define SLAVE_BOARD" to select Slave board.

LED1 blinks quickly on BOARD MASTER to wait for user-button press. 

Example execution:
Press the User push-button on BOARD MASTER to initiate a read request by Master.
This action will generate an I2C start condition with the Slave address and a read bit condition.
When address Slave match code is received on I2C2 of BOARD SLAVE, an ADDR interrupt occurs.
I2C2 Slave IRQ Handler routine is then checking Address Match Code and direction Read.
This will allow Slave to enter in transmitter mode and then send a byte when TXIS interrupt occurs.
When byte is received on I2C1 of BOARD MASTER, an RXNE interrupt occurs.
When RXDR register is read, Master auto-generate a NACK and STOP condition
to inform the Slave that the transfer is finished.
The NACK condition generate a NACK interrupt in Slave side treated in the I2C2 Slave IRQ handler routine by a clear of NACK flag.
The STOP condition generate a STOP interrupt in both side (Slave and Master). Both I2C2 IRQ handler routine are then
clearing the STOP flag in both side.

LED1 is On :
- Slave side if transfer sequence is completed.
- Master side if data is well received.

In case of errors, LED1 is blinking.

@par Keywords

Connectivity, Communication, I2C, Interrupt, Master Rx, Slave Tx, Transmission, Reception, Fast mode, SDA, SCL

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

  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Inc/main.h                  Header for main.c module
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Src/stm32f7xx_it.c          Interrupt handlers
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_MasterRx_SlaveTx_IT/Src/system_stm32f7xx.c      STM32F7xx system source file

@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-F767ZI Set-up
    - Connect GPIOs connected to Board Slave I2C2 SCL/SDA (PB.10 and PB.11)
    to respectively Board Master SCL and SDA pins of I2C1 (PB.6 and PB.9).
      - I2C2_SCL  PB.10 (CN10, pin 32) : connected to I2C1_SCL PB.6 (CN10, pin 13) 
      - I2C2_SDA  PB.11 (CN10, pin 34) : connected to I2C1_SDA PB.9 (CN7, pin 4)
    - Connect Master board GND to Slave Board GND

  - Launch the program. Press User push-button to initiate a read request by Master 
      then Slave send a byte.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory (The user can choose between Master 
   and Slave target through "#define SLAVE_BOARD" in the "main.h" file)
    o Comment "#define SLAVE_BOARD" and load the project in Master Board
    o Uncomment "#define SLAVE_BOARD" and load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
