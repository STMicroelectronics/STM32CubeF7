/**
  @page GPS_Map_Tracker Nano GPS Click module 
  
  @verbatim
  ******************************************************************************
  * @file    GPS_Map_Tracker/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the GPS map tracker using nano GPS click module.
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

This application shows how to use the Nano GPS Click module plugged on Fanout board connected 
to STM32F723E-Discovery via STMOD+ to perform a real time positioning using STM32 Cube HAL.

The GPS map tracker contains a GPS module to receive the GPS signal and calculate the coordinates

This application communicate via St-link as VCP with GoogleEarth application installed on PC.
(See Hardware and Software environment section below)

The global positioning system is a satellite navigation system that uses multiple 
satellite signals to find a receiver’s position on earth. 

The GPS receivers use NMEA standard protocol to transmit data.
The GPS Frame is a sequence of characters containing geolocation information such as:
  - The latitude, longitude,
  - Speed, altitude
  - The number of satellites
  - Time, date
  
NMEA sentences are all ASCII, RS-232 compatible and are transmitted as frame at Baudrate 4800.
Each frame begins with the character $ followed by a group of 2 letters for the 
identifier of the receiver. Then a group of 3 letters for the identifier of the frame.
It ends with a carriage return linefeed (<CR><LF>). Data is comma delimited. 

GPS Frame Examples:
   $GPGGA - Global Positioning System Fix Data
   $GPGSA - GPS DOP and active satellites 
   $GPGSV - GPS Satellites in view
   $GPRMC - Recommended minimum specific GPS/Transit data

At startup, the LCD, ToochScreen and GPS module are initialized, and a message displayed. 
In case of fail, LED5 is blinking and an ERROR message is displayed on the LCD screen.

@note: Comment LCD_USE define in main.h file to disable the display on LCD.
  
Nano GPS click receiver finds this location by coordinating information from at 
least three satellite signals. 
It produces a NMEA frame. This frame is then sent to STM32F723xx via a serial link. 
Once this frame is decoded, the useful information (num of Sat, time, date, Longitude, 
Altitude, latitude, speed) are displayed on the LCD. 
If the GoogleEarth Application is runing on PC, the received frame is sent to PC 
via VCP( St-link in this application) and the correspondig location on Google earth map on run time.

@par Keywords

GPS, map tracker, STMOD+, Nano GPS Click module, global positioning, NMEA, Fanout board

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
 - GPS/GPS_Map_Tracker/Inc/nano_gps_click.h       Header for the nano_gps_click.c containing functions and data structures for GPS_XXX() API 
 - GPS/GPS_Map_Tracker/Inc/stm32f7xx_hal_conf.h   HAL configuration file
 - GPS/GPS_Map_Tracker/Inc/stm32f7xx_it.h         STM32 interrupt handlers header file
 - GPS/GPS_Map_Tracker/Src/nano_gps_click.c       Implementation of the GPS_XXX() API.
 - GPS/GPS_Map_Tracker/Src/main.c                 Main Program
 - GPS/GPS_Map_Tracker/Src/stm32f7xx_hal_msp.c    HAL MSP module
 - GPS/GPS_Map_Tracker/Src/system_stm32f7xx.c     STM32F7xx system clock configuration file
 - GPS/GPS_Map_Tracker/Src/stm32f7xx_it.c         STM32 interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32F723XX devices.
    
  - This application has been tested with the following environment:
     - STM32F723E_DISCOVERY Rev-C 01 board. 
     - Nano GPS click module plugged on STM32F723E_DISCOVERY through connector CN10, CN11.
	 
  - Environment Setup
    - Connect the Fanout Board MB1280 to STMOD+ connector (P1).
    - Plug Nano GPS click module on connector CN11 and CN10. Make sure that the Green PWR LED on the Nano Click Board is on.
    - Connect the STM32F723E_Discovery To PC via ST link CN1.
    - Open Google Earth Application on the PC.
    - Menu Tools => GPS => Realtime => Automatically follow => Start

@note: Note that Google Earth will not detect GPS COM port until you have a GPS fix.

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Check that DK board is connected to a remote PC before running the application
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
