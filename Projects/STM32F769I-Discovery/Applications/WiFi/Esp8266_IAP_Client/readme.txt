/**
  @page Esp8266_IAP_Client Esp8266 WiFi module IAP Demo
  
  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Esp8266 IAP over WiFi demo.
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


This application shows how to use the ESP8266 WiFi module to perform an IAP ("In 
Application Programming") using STM32 Cube HAL.
This application may be used with a Smartphone or PC server (more detailed setup
instructions are described in section "Hardware and Software environment").

- If the "User" Button is pressed when the application starts, then the WiFi IAP module
  is started. Otherwise, the user code downloaded into Flash offset is executed.
  If no code is downloaded yet into Flash memory offset, then the WiFi IAP application 
  will start.

- At startup, the WiFi module is initialized, the LCD shows an "O.K!" message in case 
  of success. Otherwise LED1 is blinking and an ERROR message is displayed on  the LCD screen.

- Once initialized, the board attempts to join WiFi access point using the predefined 
  SSID and password setup by user into configuration file at compilation time.
  In case of success the acquired IP address is displayed, otherwise a message informing about
  retrying the connection (up to 10 times) is displayed. 
  After the 10 failing trials an error message is displayed and the LED1 is blinking.

- After joining the access point, the board attempts to establish a connection to the 
  remote http server using the predefined address and port setup by user in configuration file.
  A number of retrials is done in case of failure.

- As soon as the connection to the remote server is established, the "hello.bin" file is downloaded. 
  On success the following messages is displayed on the LCD screen: 
  "Downloaded firmware ' hello.bin ', size = 22440 bytes", otherwise a failure message is 
  shown and the LED1 is blinking.
 
- When downloading is finished, the message "State: Programming..." is displayed on the LCD. 
  The "hello.bin" fw is being written into the flash. 
  On Success the message "Programming Done!" is displayed on the LCD and LED2 is blinking.
  If the flashing operation fails, an error message is displayed and the LED1 is blinking instead.
 
- Reset the board (using "RESET" button) and keep "USER" button unpressed in order to execute 
  the binary file downloaded into Flash memory.
 
The following parameters should be configured by user at compilation time through 
configuration file "main.h":
 - Network SSID
 - Network Password
 - Host (server) address
 - Host Port number
 - File name
 - Maximum number of trials
 
@note:
This Application is using the ESP8266 as a WiFi client, i.e  an accessible WiFi hotspot should
be available and the HTTP server is accessible in the same local network.
It is possible to configure a mobile phone to run as a wifi hotspot alongside a webserver.

For mobile phones you can use the Palpa web server: http://alfanla.com/palapa-web-server
For Windows PC you can use the Wamp server http://www.wampserver.com/en

@par Keywords

Connectivity, Wifi, ESP8266, Hotspot, IAP Client, HTTP, Web server, 

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
 - WiFi/Esp8266_IAP_Client/binary/hello.bin           A precompiled binary file to be used as IAP fw.
 - WiFi/Esp8266_IAP_Client/Inc/main.h                 Header containing config parameters for the application.
 - WiFi/Esp8266_IAP_Client/Inc/esp8266_io.h           Header for the Esp8266 IO functions and defines used by the esp8266_io.c
 - WiFi/Esp8266_IAP_Client/Inc/esp8266.h              Header for the esp8266.c containing functions and data structures for ESP8266_XXX() API 
 - WiFi/Esp8266_IAP_Client/Inc/flash_if.h             Header used by flash_if.c
 - WiFi/Esp8266_IAP_Client/Inc/stm32f7xx_hal_conf.h   HAL configuration file
 - WiFi/Esp8266_IAP_Client/Inc/lcd_log_conf.h         LCD Log configuration file
 - WiFi/Esp8266_IAP_Client/Inc/stm32f7xx_it.h         STM32 interrupt handlers header file
 - WiFi/Esp8266_IAP_Client/Src/esp8266.c              Implementation of the ESP8266_XXX() API.
 - WiFi/Esp8266_IAP_Client/Src/esp8266_io.c           Implementation of the ESP8266_IO_XXX() API.
 - WiFi/Esp8266_IAP_Client/Src/flash_if.c             Implementation of the FLASH routines  to erase, write and check FLASH sectors.
 - WiFi/Esp8266_IAP_Client/Src/main.c                 Main Program
 - WiFi/Esp8266_IAP_Client/Src/stm32f7xx_hal_msp.c    HAL MSP module
 - WiFi/Esp8266_IAP_Client/Src/system_stm32f7xx.c     STM32F7xx system clock configuration file
 - WiFi/Esp8266_IAP_Client/Src/stm32f7xx_it.c         STM32 interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32F769xx devices.
    
  - This application has been tested with the following environment:
     - STM32F769I_DISCOVERY Rev-B board. 
     - ESP8266-01 WiFi module plugged on STM32F769I_DISCOVERY through connector CN2.
     - A remote host running a web server (either Linux or Windows PC or Android Mobile phone).

	 
  - Environment Setup
    - Connect the ESP8266 module to the STM32F769I_DISCOVERY board through connector CN2.
	
	- When using Linux or Windows PC:
	  - Start the Apache or Wamp server respectively on the Linux or Windows machine.
	  - Copy the "hello.bin" file under an accessible path on the webserver (i.e "esp8266/hello.bin")
	  - The PC should be connected to the local network and accessible via the WiFi access point.
	  
	- When using Android Phone
	  - Install the 'Palpa WebServer' on smartphone.
	  - Connect the phone to a PC using a USB cable and transfer the "hello.bin" on the phone SDCARD (ie. "esp8266" directory)
	  - Start the Palpa WebServer.
	  - Configure the Android phone as a WiFi access point.
	
   - edit the file "config.h" and adjust the defines:
     WIFI_SSID       The WiFi access point SSID.
     WIFI_PASSWORD   The WiFi access point password.
     HOST_ADDRESS    The IP Address of the PC or phone running the web server.
     HOST_PORT       The HTTP port used by the web server. When using Palpa WebServer on a mobile phone,
                     it is possible to edit the 'webserver root dir' under (WebServer Settings->Components->lighttpd->Port)
     FW_FILE         The relative path of the "hello.bin" w.r.t the web server root dir. When using Palpa WebServer on a mobile phone,
                     it is possible to edit the 'webserver root dir' under (WebServer Settings->Components->lighttpd->Document Root)
     DATA_MAX_SIZE   Data size required to hold the "hello.bin". (should take into account HTTP overhead size)
     MAX_NUM_TRIAL   Maximum number of trials to do before considering operation failure (used to 
	                 connect to the wifi AP and to the http web server)


In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Check that DK board is connected to a remote PC before running the application
 - Run the application

 @note
 - To be correctly written into the Flash the fw binary maximum size can't exceed 340 kb.
 - The application is based on the esp8266 AT FW 0.40.0, this fw version doesn't support connection to secure http server.
   This feature will be supported in future releases.


 */
