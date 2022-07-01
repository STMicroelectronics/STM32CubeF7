/**
  @page LwIP_IAP  IAP over Ethernet 
 
  @verbatim
  ******************************************************************************
  * @file    LwIP/LwIP_IAP/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the in-application programming (IAP) over 
  *          Ethernet.
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

@par Description

This application guides STM32Cube HAL API users to run In-Application Programming 
(IAP) over Ethernet. 

Two IAP options are provided: 
 - IAP using TFTP (Trivial File Transfer Protocol)
 - IAP using HTTP (Hypertext Transfer Protocol) 

User has select through compile options (in main.h file) to include one or both 
options.

LwIP/LwIP_IAP/Binary directory contains an application binary example (STM32F769I_EVAL_GPIO_Toggle.bin) 
which can be used to be transfered and executed. It toggles the 4 LEDs.

User has to reset the device while Pushing TAMPER button to enter IAP mode.

If the LCD is used (#define USE_LCD in main.h), log messages will be displayed 
to inform user about ethernet cable status and the IP address value, else this 
will be ensured by LEDs:
  + LED1: ethernet cable is connected.
  + LED2: ethernet cable is not connected.

In case of Invalid stack adress, LED3 turns ON.

If a DHCP server is available, a dynamic IP address can be allocated by enabling 
the DHCP process (#define LWIP_DHCP in lwipopts.h).

If a DHCP server is not available, after timeout connection, the device only gets a static 
IP address(the switch from static to dynamic IP adress is not available in this application).

Note: In this application the Ethernet Link ISR need the System tick interrupt 
to configure the Ethernet MAC, so the Ethernet Link interrupt priority must be 
set lower (numerically greater) than the Systick interrupt priority to ensure 
that the System tick increments while executing the Ethernet Link ISR.

Note: By default, the Ethernet Half duplex mode is not supported in the 
STM32F769I_EVAL board, for more information refer to the HAL_ETH_MspInit() 
function in the ethernetif.c file

Note : ETH_MDIO signal is connected to PA2 which is shared with other signals like SAI2_SCKB.
By default on STM32F769I_EVAL board, PA2 is connected to SAI2_SCKB, so to connect PA2 to ETH_MDIO :
    - jumper JP3 must be on the position 2-3

Note : ETH_MDC is connected to PC1 which is shared with other signals like SAI1_SDA.
By default on STM32F769I_EVAL board, PC1 is connected to SAI1_SDA, so to connect PC1 to ETH_MDC :
    - jumper JP6 must be on the position 2-3

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note The required userID and password are set, and can be modified, in the main.h file.
      
@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this application uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.

For more details about this application, refer to UM1709 "STM32Cube Ethernet IAP application"

@par Keywords

IAP, In-Application Programming, Ethernet, ETH, LwIP, TFTP, TCP/IP, HTTP, LCD

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

    - LwIP/LwIP_IAP/Inc/app_ethernet.h       header of app_ethernet.c file
    - LwIP/LwIP_IAP/Inc/ethernetif.h         header for ethernetif.c file
    - LwIP/LwIP_IAP/Inc/lcd_log_conf.h       LCD Log configuration file
    - LwIP/LwIP_IAP/Inc/main.h               Main program header file          
    - LwIP/LwIP_IAP/Inc/flash_if.h           header for flash_if.c
    - LwIP/LwIP_IAP/Inc/httpserver.h         header for httpserver.c
    - LwIP/LwIP_IAP/Inc/tftpserver.h         header for tftpserver.c
    - LwIP/LwIP_IAP/Inc/lwipopts.h           LwIP stack configuration options
    - LwIP/LwIP_IAP/Inc/fsdata.h             header for fsdata.c  
    - LwIP/LwIP_IAP/Inc/stm32f7xx_hal_conf.h Library Configuration file
    - LwIP/LwIP_IAP/Inc/stm32f7xx_it.h       Interrupt handlers header file    
    - LwIP/LwIP_IAP/Src/app_ethernet.c       Ethernet specific module
    - LwIP/LwIP_IAP/Src/main.c               Main program          
    - LwIP/LwIP_IAP/Src/flash_if.c           high level functions for flash access (erase, write)
    - LwIP/LwIP_IAP/Src/httpserver.c         IAP http server functions
    - LwIP/LwIP_IAP/Src/tftpserver.c         IAP tftpserver functions
    - LwIP/LwIP_IAP/Src/fsdata.c             ROM filesystem data (html pages)
    - LwIP/LwIP_IAP/Src/system_stm32f7xx.c   STM32F7xx system file
    - LwIP/LwIP_IAP/Src/stm32f7xx_it.c       STM32F7xx Interrupt handlers
    - LwIP/LwIP_IAP/Binary                   contains a binary image to be 
                                             loaded with the IAP code 
             
@par Hardware and Software environment  
   
  - This application runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This application has been tested with the following environments:
     - STM32F769I_EVAL board
     - Http clients: Firefox Mozilla (v24) or Microsoft Internet Explorer (v8 and later)
     - TFTP client and DHCP server: PC utility TFTPD32 (http://tftpd32.jounin.net/)
      
  - STM32F769I_EVAL Set-up
    - Connect the eval board to your local network through a straight ethernet cable
    - jumper JP3 must be on the position 2-3 (ETH signal)
    - jumper JP4 must be on the position 2-3 (MII_MDIO signal)
    - jumper JP6 must be on the position 2-3 (ETH signal)
    - jumper JP8 must be on the position 1-2 (MII_MDC signal) 
    - jumper JP12 must be on the position 1-2 (ETH signal)
  
  - Remote PC Set-up
    - Configure a static IP address for your remote PC 
      for example 192.168.0.11 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 

