/**
  @page I2C_TwoBoards_MasterTx_SlaveRx_DMA I2C example (DMA Mode)
  
  @verbatim
  ******************************************************************************
  * @file    Examples_LL/I2C/I2C_TwoBoards_MasterTx_SlaveRx_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C_TwoBoards_MasterTx_SlaveRx_DMA I2C example (DMA Mode).
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
  
How to transmit data bytes from an I2C master device using DMA mode 
to an I2C slave device using DMA mode. The peripheral is initialized 
with LL unitary service functions to optimize for performance and size.

This example guides you through the different configuration steps by mean of LL API
to configure GPIO, DMA and I2C peripherals using two NUCLEO-F767ZI.

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
  I2C1 Peripheral is configured in Master mode with DMA (Clock 400Khz).
  And GPIO associated to User push-button is linked with EXTI. 
- Slave Board
  I2C2 Peripheral is configured in Slave mode with DMA (Clock 400Khz, Own address 7-bit enabled).

The user can choose between Master and Slave through "#define SLAVE_BOARD"
in the "main.h" file:
- Comment "#define SLAVE_BOARD" to select Master board.
- Uncomment "#define SLAVE_BOARD" to select Slave board.

A first program launch, BOARD SLAVE waiting Address Match code through Handle_I2C_Slave() routine.
LED1 blinks quickly on BOARD MASTER to wait for user-button press. 

Example execution:
Press the User push-button on BOARD MASTER to initiate a write request by Master through Handle_I2C_Master() routine.
This action will generate an I2C start condition with the Slave address and a write bit condition.
When address Slave match code is received on I2C2 of BOARD SLAVE, an ADDR event occurs.
Handle_I2C_Slave() routine is then checking Address Match Code and direction Write.
This will allow Slave to enter in receiver mode and then acknowledge Master to send the bytes through DMA.
When acknowledge is received on I2C1 (Master), DMA transfer the data from flash memory buffer to I2C1 TXDR register (Master).
This will allow Master to transmit a byte to the Slave.
Each time a byte is received on I2C2 (Slave), DMA transfer the data from I2C2 RXDR register to RAM memory buffer (Slave).
And so each time the Slave acknowledge the byte received,
DMA transfer the next data from flash memory buffer to I2C1 TXDR register (Master) until Transfer completed.
Master auto-generate a Stop condition when DMA transfer is achieved.

The STOP condition generate a STOP event and initiate the end of reception on Slave side.
Handle_I2C_Slave() and Handle_I2C_Master() routine are then clearing the STOP flag in both side.

LED1 is On :
- Slave side if data are well received.
- Master side if transfer sequence is completed.

In case of errors, LED1 is blinking.

@par Keywords

Connectivity, Communication, I2C, DMA, Master Rx, Slave Tx, Transmission, Reception, Fast mode, SDA, SCL

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

  - I2C/I2C_TwoBoards_MasterTx_SlaveRx_DMA/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx_DMA/Inc/main.h                  Header for main.c module
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx_DMA/Inc/stm32_assert.h          Template file to include assert_failed function
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx_DMA/Src/stm32f7xx_it.c          Interrupt handlers
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx_DMA/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_MasterTx_SlaveRx_DMA/Src/system_stm32f7xx.c      STM32F7xx system source file

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

  - Launch the program. Press User push-button to initiate a write request by Master 
      then Slave receive bytes.

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
