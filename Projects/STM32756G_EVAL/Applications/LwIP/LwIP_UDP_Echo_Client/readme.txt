/**
  @page LwIP_UDP_Echo_Client LwIP UDP Echo Client Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    LwIP/LwIP_UDP_Echo_Client/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LwIP UDP Echo Client Application.
  ******************************************************************************
  *
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This application guides STM32Cube HAL API users to run a UDP Echo Client application 
based on Raw API of LwIP TCP/IP stack

To run this application, On the remote PC, open a command prompt window.
(In Windows, select Start > All Programs > Accessories > Command Prompt.)
At the command prompt, enter:
    C:\>echotool /p udp /s
where:
    – /p transport layer protocol used for communication (UDP)
    – /s is the actual mode of connection (Server mode)

In case the DHCP is used, user should define its PC Host IP address DEST_IP_ADDR[0..3] in main.h file.

Each time the user pushes the KEY button of the STM327x6G_EVAL board, an echo 
request is sent to the server

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed to inform user about ethernet cable status and the IP address value, else this will be ensured by LEDs:
 + LED1: ethernet cable is connected.
 + LED2: ethernet cable is not connected.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define LWIP_DHCP to 1 in lwipopts.h). 

If a DHCP server is not available, after timeout connection, the device only gets a static 
IP address(the switch from static to dynamic IP adress is not available in this application).

Note: In this application the Ethernet Link ISR need the System tick interrupt 
to configure the Ethernet MAC, so the Ethernet Link interrupt priority must be 
set lower (numerically greater) than the Systick interrupt priority to ensure 
that the System tick increments while executing the Ethernet Link ISR.

Note: By default, the Ethernet Half duplex mode is not supported in the 
STM327x6G_EVAL board, for more information refer to the HAL_ETH_MspInit() 
function in the ethernetif.c file

Note : ETH_MDIO signal is connected to PA2 which is shared with other signals like SAI2_SCKB.
By default on STM327x6G_EVAL board, PA2 is connected to SAI2_SCKB, so to connect PA2 to ETH_MDIO :
    - jumper JP21 must be on the position 2-3

Note : ETH_MDC is connected to PC1 which is shared with other signals like SAI1_SDA.
By default on STM327x6G_EVAL board, PC1 is connected to SAI1_SDA, so to connect PC1 to ETH_MDC :
    - jumper JP22 must be on the position 2-3

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this application uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.

For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"

@par Keywords

Connectivity, LwIP, Ethernet, UDP, Echo Client, TCP/IP, DHCP

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

  - LwIP/LwIP_UDP_Echo_Client/Inc/app_ethernet.h          header of app_ethernet.c file
  - LwIP/LwIP_UDP_Echo_Client/Inc/ethernetif.h            header for ethernetif.c file
  - LwIP/LwIP_UDP_Echo_Client/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - LwIP/LwIP_UDP_Echo_Client/Inc/stm32f7xx_it.h          STM32 interrupt handlers header file
  - LwIP/LwIP_UDP_Echo_Client/Inc/main.h                  Main program header file
  - LwIP/LwIP_UDP_Echo_Client/Inc/lwipopts.h              LwIP stack configuration options
  - LwIP/LwIP_UDP_Echo_Client/Inc/lcd_log_conf.h LCD      Log configuration file
  - LwIP/LwIP_UDP_Echo_Client/Inc/udp_echoclient.h        Header for UDP echoclient application
  - LwIP/LwIP_UDP_Echo_Client/Src/app_ethernet.c          Ethernet specific module
  - LwIP/LwIP_UDP_Echo_Client/Src/stm32f7xx_it.c          STM32 interrupt handlers
  - LwIP/LwIP_UDP_Echo_Client/Src/main.c                  Main program
  - LwIP/LwIP_UDP_Echo_Client/Src/system_stm32f7xx.c      STM32F7xx system clock configuration file
  - LwIP/LwIP_UDP_Echo_Client/Src/ethernetif.c            Interfacing LwIP to ETH driver
  - LwIP/LwIP_UDP_Echo_Client/Src/udp_echoclient.c        UDP echoclient application


@par Hardware and Software environment

  - This application runs on STM32F756xx/STM32F746xx devices.
    
  - This application has been tested with the following environments:
     - STM327x6G_EVAL board   
     - echotool: (http://bansky.net/echotool/) is used as echo server that sends
       back every incoming data.   
      
  - STM327x6G_EVAL Set-up
    - Connect the eval board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
    - jumper JP21 must be on the position 2-3 (ETH_MDIO signal)
    - jumper JP22 must be on the position 2-3 (ETH_MDC signal)
    - If LED1 is used, jumper 24 must be on the position 2-3
  
  - Remote PC Set-up
    - Configure a static IP address for your remote PC 
      this address must be 192.168.0.11 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


