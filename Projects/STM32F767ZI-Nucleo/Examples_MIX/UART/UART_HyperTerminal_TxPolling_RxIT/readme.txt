/**
  @page UART_HyperTerminal_TxPolling_RxIT UART Hyperterminal Polling/IT example (HAL/LL mixed usage example)
  
  @verbatim
  ******************************************************************************
  * @file    UART/UART_HyperTerminal_TxPolling_RxIT/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the UART HAL/LL mixed Hyperterminal example.
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

Use of a UART to transmit data (transmit/receive) 
between a board and an HyperTerminal PC application both in Polling and Interrupt 
modes. This example describes how to use the USART peripheral through 
the STM32F7xx UART HAL and LL API, the LL API being used for performance improvement.

Board: NUCLEO-F767ZI
Tx Pin: PD.08
Rx Pin: PD.09
   _________________________ 
  |           ______________|                       _______________
  |          |USART3        |                      | HyperTerminal |
  |          |              |                      |               |
  |          |           TX |______________________|RX             |
  |          |              |                      |               |
  |          |              |  Virtual Com Port    |               |             
  |          |              |                      |               |
  |          |           RX |______________________|TX             |          
  |          |              |                      |               |           
  |          |______________|                      |_______________|          
  |                         |                       
  |                         |                    
  |                         |                      
  |                         |                      
  |_STM32_Board_____________|                      

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz for STM32F7xx Devices.

The UART peripheral configuration is ensured by the HAL_UART_Init() function.
This later is calling the HAL_UART_MspInit()function which core is implementing
the configuration of the needed UART resources according to the used hardware.
You may update this function to change UART configuration.

Reception under IT on UART/Hyperterminal interface is then enabled using LL APIs.
Each 0.5 sec, a predefined Tx buffer is sent from MCU to PC reception of Data from Hyperterminal.
The Asynchronous communication aspect of the UART is clearly highlighted as the  
data buffers transmission/reception to/from Hyperterminal are done simultaneously.
Transmit function which allow transmission of a predefined data buffer
in polling mode, is implemented using HAL UART API.
Receive function which allow the reception of Data from Hyperterminal in 
IT mode, is implemented using LL USART API.
A double buffer is available for received data, allowing continuous reception.
Data received in Buffer A could be echoed on Tx, while buffer B is used for reception. 
When buffer B is full, it is then echoed, while buffer A becomes active buffer for next reception, and so on.

For this example the TxBuffer is predefined and the RxBuffer size is limited to 
10 data by the mean of the RXBUFFERSIZE define in the main.c file.

Received data will be stored in the RxBuffer buffer. 
When Rx buffer is full, RX buffer is sent back to PC and led toggles.

The Asynchronous communication aspect of the UART is clearly highlighted as the  
data buffers transmission/reception to/from Hyperterminal are done simultaneously.

STM32 Nucleo board's LEDs can be used to monitor the transfer status:
 - LED1 toggles each time transmission/reception of 10 characters is completed..
 - LED3 stays ON when there is an error in transmission/reception process.  

The UART is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

@note USARTx/UARTx instance used and associated resources can be updated in "main.h"
      file depending hardware configuration used.
      For example, current example is provided in a configuration based on Virtual Com Port use (USART3).
      Another USART instance could be used, assuming corresponding TX and RX pins are properly wired 
      to PC Com port (updates to be done accordinlgy in main.h).

@note When the parity is enabled, the computed parity is inserted at the MSB
      position of the transmitted data.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@Note�If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
 �����then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
������The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@par Keywords

Connectivity, UART, Printf, Baud rate, RS-232, HyperTerminal, Polling, Transmission,
Reception, Asynchronous

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
����� It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
������In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
������Even though the user must manage the cache coherence for read accesses.
������Please refer to the AN4838 �Managing memory protection unit (MPU) in STM32 MCUs�
������Please refer to the AN4839 �Level 1 cache on STM32F7 Series�

@par Directory contents 

  - UART/UART_HyperTerminal_TxPolling_RxIT/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - UART/UART_HyperTerminal_TxPolling_RxIT/Inc/stm32f7xx_it.h          IT interrupt handlers header file
  - UART/UART_HyperTerminal_TxPolling_RxIT/Inc/main.h                  Main program header file  
  - UART/UART_HyperTerminal_TxPolling_RxIT/Src/stm32f7xx_it.c          IT interrupt handlers
  - UART/UART_HyperTerminal_TxPolling_RxIT/Src/main.c                  Main program
  - UART/UART_HyperTerminal_TxPolling_RxIT/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - UART/UART_HyperTerminal_TxPolling_RxIT/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F767xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-F767ZI board and can be
    easily tailored to any other supported device and development board.    
      
  - NUCLEO-F767ZI Set-up
    Example is delivered for using Virtual Com port feature of STLINK for connection between NUCLEO-F767ZI and PC,
    Please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled 
    on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)
    GPIOs connected to USART3 TX/RX (PD.08 and PD.09) are automatically mapped
    on RX and TX pins of PC UART Com port selected on PC side (please ensure VCP com port is selected).

  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration 
    - Word Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 9600 baud
    - flow control: None 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 */
