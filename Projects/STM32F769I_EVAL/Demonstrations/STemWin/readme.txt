/**
  @page STemWin   STM32F769I-EVAL STemWin Demonstration Firmware
 
  @verbatim
  ******************************************************************************
  * @file    Demonstrations/STemWin/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of STM32F769I-EVAL Demonstration
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

@par Demo Description

The STM32Cube Demonstration platform comes on top of the STM32CubeTM as a firmware
package that offers a full set of software components based on a modules architecture
allowing re-using them separately in standalone applications. All these modules are
managed by the STM32Cube Demonstration kernel allowing to dynamically adding new
modules and access to common resources (storage, graphical components and widgets,
memory management, Real-Time operating system)

The STM32Cube Demonstration platform is built around the powerful graphical library
STemWin and the FreeRTOS real time operating system and uses almost the whole STM32
capability to offer a large scope of usage based on the STM32Cube HAL BSP and several
middleware components.

@par Demonstration Overview

The STM32 F7 demonstration is running on STM32F769I evaluation boards.
Tow flavours of the Demonstration binaries are available: 
  - 'STM32CubeDemo_STM32769I-EVAL_V1.3.0.hex': based on StemWin and which source code 
     is provided within the STM32Cube_FW_F7 package. Its modules are listed below.
  - 'STM32769I-EVAL_DEMO_V1.0.0_FULL.hex' : an out of the box Demo integrating the StemWin
    demo in addition to third parties graphical Demo modules: 
	   - TouchGFX demonstration module based on Draupner Graphics commercial graphic library.
	     Free evaluation version is available at www.touchgfx.com/stmicroelectronics.
	   - Embedded Wizard demonstration module from TARA systems.
         Free evaluation version is available at www.embedded-wizard.de/stm32
		 
Below you find an overview of the different offered modules in the StemWin demonstration:

+ Audio
 -------
 The audio player module provides a complete audio solution based on the STM32F7xx and
 delivers a high-quality music experience. It supports playing music in WAV format but may
 be extended to support other compressed formats such as MP3 and WMA audio formats.
 You can use the *.wav audio provided under "Utilities/Media/Audio" or any other ones.
 
 + Video
 -------
 The video player module provides a video solution based on the STM32F7xx. 
 It supports playing movie in AVI format with 800x480 frame resolution.
 You can use the *.avi video file provided under "Utilities/Media/Video".
 
 + Audio Recorder
 ----------------
 The audio recorder module can be used to record audio frames in WAV format, 
 save them in " Record" folder under the storage unit and play them later.
    
 + VNC Server
 ------------
 The VNC server module allows to control the demo from a remote machine. It is based on
 the TCP/IP LwIP stacks. The background mode is supported.
 @note Launch any VNC Client or the emVNC software located under "Middlewares\ST\STemWin\Software" 
       to run the module.   
   
 + Home alarm
 ------------ 
 Control of Home alarm system, equipped with cameras.
 Static picture shown when a room is selected and then the camera icon pressed
 General room alarm activation/deactivation when pressed.
 
 + Gardening control
 -------------------
 The gardening control module provides a graphic garden watering system behaviour
 
 + Game
 ------
 The game coming in the STM32Cube demonstration is based on the Reversi game. It is a
 strategy board game for two players, played on an 8×8 board. The goal of the game is to
 have the majority of disks turned to display your colour when the last playable empty square
 is filled.
 
 + System Info
 --------------  
 The system info module provides information about the core, supported eval boards, 
 CPU speed and demonstration version.  

@note The AudioRecorder and AudioPlayer modules are not supported when VNC Server module 
      is used since Ethernet and Audio are sharing the same pins.
@note The VideoPlayer module is not supported when VNC server module is used since 
      the second layer of the VideoPlayer module cannot be displayed/controlled by 
      the remote machine.
@note For STM32769I-EVAL_DEMO_V1.0.0_FULL.hex binary, it is recommended to use an USB Key containing only set of
      media files available under "Utilities/Media" folder.

 For more details about the demonstration modules please refers to  STM32CubeF7 demonstration (UM1906)
    
@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this demonstration uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@par Keywords

Demonstration, STemWin, FreeRTOS, RTOS, Audio record, wav, Video Player, avi, MP3, Audio player, Graphic,
Game, System, CPU, VNC, SAI, QUADSPI, USB, Microphone 

@par Hardware and Software environment

  - This demonstration runs on STM32F769xx devices.    
  - This demonstration has been tested with STM32769I-EVAL RevB evaluation board.
  - Jumpers configuration:
  
    - JP1: must be not fitted 
           ==> The Bootloader_BOOT is managed by pin 6 of connector CN7(RS232 DSR signal) 
               when JP1 is closed. This configuration is used for boot loader application only.
    - JP2: must be fitted 
           ==> used to measure MCU current consumption manually by multi meter
    - JP3: Position: <1-2> when using the audio applications
                     ==> PA2 is connected to SAI2_SCKB 
           Position: <2-3> when using the vnc server application
                     ==>PA2 is connected to MII_MDIO (Ethernet)
    - JP6: Position: <1-2> when using the audio applications
                     ==> PC1 is connected to SAI1_SDA
           Position: <2-3> when using the vnc server application
                     ==> PC1 is connected to MII_MDC (Ethernet)
    - JP7: Position: <2-3> ==> PD6 is connected to DFSDM_DATA1
    - JP12: Position: <1-2> 25MHz clock is provided by external crystal X4
    - JP15: Position: <2-3> ==> VBAT is connected to battery
    - JP22: Position: <1-2> Digital microphone power source is connected to +3.3V
    - JP23: Position: <2-3> Data signal on digital microphone is connected to 
                      DFSDM of STM32F769NIH6
    - JP24: Position: <2-3> Clock signal on digital microphone is connected to 
                      DFSDM of STM32F769NIH6 
  
@par How to use it ? 

The QSPI external flash loader is not integrated with supported toolchains, its only supported with STM32
ST-Link Utility V3.9 or later
To load the demonstration, use STM32 ST-Link Utility to program both internal Flash and external QSPI memory.
To edit and debug the demonstration you need first to program the external QSPI memory using STLink utility
and then use your preferred toolchain to update and debug the internal flash content.

In order to program the demonstration you must do the following:
1- Open STM32 ST-Link Utility, click on "External Loader" from the bar menu then check 
   "MT25QL512A_STM32769I-EVAL" box 
2- Connect the STM32F769I-EVAL board to PC with USB cable through CN22
3- Use "STM32CubeDemo_STM32769I-EVAL_VX.Y.Z.hex" file provided under Binary with STM32 ST-Link Utility
   to program both internal Flash and external QSPI memory.
   The "STM32769I-EVAL_DEMO_V1.0.0_FULL.hex" file provided under 'Binary', 
   can also be used to take benefit from Touch-GFX and EmbeddedWizard third parties 
   demonstrations modules replacing Gardening control and Home alarm modules.
   
4- copy the audio and video files provided under 'Utilities/Media/' in the USB key
5- Plug a USB micro A-Male to A-Female cable on CN8 connector
6- Plug a headphone into CN28 Connector
-> The internal Flash and the external QSPI are now programmed and the demonstration is shown on the board.

In order to Edit and debug the program, you must do the following
- if not done, perform step 1, 2, 3, 4, 5 and 6 described above,
- Open your preferred toolchain,
- Use the IDE to update and load the internal flash content, 
- Run the demonstration.

@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 Managing memory protection unit (MPU) in STM32 MCUs
      Please refer to the AN4839 Level 1 cache on STM32F7 Series


 */
 
