/**
  @page I2C_TwoBoards_ComPolling I2C Two Boards Communication Polling Example
  
  @verbatim
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards_ComPolling/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the I2C Two Boards Communication Polling example.
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

How to handle I2C data buffer transmission/reception between two boards, 
in polling mode.

Board: STM32F722ZE-Nucleo (embeds a STM32F722ZE device)
SCL Pin: PB8 (CN7, D15)
SDA Pin: PB9 (CN7, D14)

   _________________________                       _________________________ 
  |           ______________|                     |______________           |
  |          |I2C1          |                     |          I2C1|          |
  |          |              |                     |              |          |
  |          |          SCL |_____________________| SCL          |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |              |                     |              |          |
  |          |          SDA |_____________________| SDA          |          |
  |          |              |                     |              |          |
  |          |______________|                     |______________|          |
  |                         |                     |                         |
  |                      GND|_____________________|GND                      |
  |_STM32_Board 1___________|                     |_STM32_Board 2___________|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts: the Master Board and the Slave Board
- Master Board
  The HAL_I2C_Master_Receive() and the HAL_I2C_Master_Transmit() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode.
- Slave Board
  The HAL_I2C_Slave_Receive() and the HAL_I2C_Slave_Transmit() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode.
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
- Uncomment "#define MASTER_BOARD" to select Master board.
- Comment "#define MASTER_BOARD" to select Slave board.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button on the Master Board,
I2C Master starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit()
to I2C Slave which receives aRxBuffer through HAL_I2C_Slave_Receive(). 
The second step starts when the user press the User push-button on the Master Board,
the I2C Slave sends aTxBuffer through HAL_I2C_Slave_Transmit()
to the I2C Master which receives aRxBuffer through HAL_I2C_Master_Receive().
The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32F722ZE-Nucleo's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transmission process is complete.
 - LED1 is OFF when the reception process is complete.
 - LED3 is ON when there is an error in transmission/reception process.
 - LED3 is ON on slave board in case of Timeout error

@note I2Cx instance used and associated resources can be updated in "main.h"
file depending hardware configuration used.

 @note Timeout is set to 10 Seconds which means that if no communication occurs 
       during 10 Seconds, a Timeout Error will be generated.


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Connectivity, Communication, I2C, Polling, Master, Slave, Transmission, Reception, Fast mode plus

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

  - I2C/I2C_TwoBoards_ComPolling/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_ComPolling/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - I2C/I2C_TwoBoards_ComPolling/Inc/main.h                  Header for main.c module  
  - I2C/I2C_TwoBoards_ComPolling/Src/stm32f7xx_it.c          Interrupt handlers
  - I2C/I2C_TwoBoards_ComPolling/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_ComPolling/Src/system_stm32f7xx.c      STM32F7xx system source file
  - I2C/I2C_TwoBoards_ComPolling/Src/stm32f7xx_hal_msp.c     HAL MSP file    

@par Hardware and Software environment

  - This example runs on STM32F722xx/STM32F723xx/STM32F732xx/STM32F733xx devices.
    
  - This example has been tested with STM32F722ZE-Nucleo board and can be
    easily tailored to any other supported device and development board.    

  - STM32F722ZE-Nucleo Set-up
    - Connect I2C_SCL line of Master board (PB8, CN7, D15) to I2C_SCL line of Slave Board (PB8, CN7, D15).
    - Connect I2C_SDA line of Master board (PB9, CN7, D14) to I2C_SDA line of Slave Board (PB9, CN7, D14).
    - Connect GND of Master board to GND of Slave Board.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example


 */
