/**
  @page LibJPEG_Decoding LibJPEG decoding application
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 STMicroelectronics *******************
  * @file    LibJPEG/LibJPEG_Decoding/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the LibJPEG decoding application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
   @endverbatim

@par Application Description

This application demonstrates how to read jpeg file from USB disk memory, decode it 
and display the final BMP image on the LCD.

In this application, the code is executed from QSPI external memory while data are in internal
SRAM memory.

The JPEG images should be copied at the USB disk root.

The images must have the following properties:
 - named as "image.jpg"
 - 240*240 size
 
An image under Utilities\Media\Pictures\JPG root could be used.

Unomment "SWAP_RB" define in "main.h" file if the displayed image colors 
are inverted

The application starts with initializing the USB MSC. Its information will be 
displayed on the LCD.
Once the application is ready, a check on the "image.jpg" file is done. If the
file doesn't exist, a message will be displayed and Red Led is ON.
For each line decoded, the 240 pixels are transformed from RGB888 to RGB565 and
then displayed.

@note The decode is using integer method. This is recommended from speed point a view. 
      The decode is faster but less accurate. The precision impact is not remarkable 
      for such low resolution. In this case 240x240.

@note If USB key is unpluged, a message will be displayed. User must plug the USB
      key and Reset the board.
      
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Display, Graphic, LibJPEG, Decoding, Flash memory, RGB565, LCD, RGB, BMP, JPG, USB disk

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

    - LibJPEG/LibJPEG_Decoding/Inc/main.h                      Main program header file
    - LibJPEG/LibJPEG_Decoding/Inc/stm32f7xx_it.h              Interrupt handlers header file
    - LibJPEG/LibJPEG_Decoding/Inc/stm32f7xx_hal_conf.h        HAL Configuration file 
    - LibJPEG/LibJPEG_Decoding/Inc/ffconf.h                    FAT file system module configuration file
    - LibJPEG/LibJPEG_Decoding/Inc/decode.h                    decoder header file
    - LibJPEG/LibJPEG_Decoding/Inc/jmorecfg.h                  Lib JPEG configuration file (advanced configuration).
    - LibJPEG/LibJPEG_Decoding/Inc/jconfig.h                   Lib JPEG configuration file.
    - LibJPEG/LibJPEG_Decoding/Inc/jdata_conf.h                Write/Read methods definition
    - LibJPEG/LibJPEG_Decoding/Inc/lcd_log_conf.h              LCD log configuration file
    - LibJPEG/LibJPEG_Decoding/Inc/usbh_diskio.h               USB diskio header file 
    - LibJPEG/LibJPEG_Decoding/Inc/usbh_conf.h                 USB Host driver Configuration file     
    - LibJPEG/LibJPEG_Decoding/Src/main.c                      Main program  
    - LibJPEG/LibJPEG_Decoding/Src/stm32f7xx_it.c              Interrupt handlers
    - LibJPEG/LibJPEG_Decoding/Src/system_stm32f7xx.c          STM32F7xx system clock configuration file
    - LibJPEG/LibJPEG_Decoding/Src/decode.c                    decoder file
    - LibJPEG/LibJPEG_Decoding/Src/jdata_conf.c                Write/Read methods definition
    - LibJPEG/LibJPEG_Decoding/Src/usbh_diskio.c               USB diskio interface for FatFs 
    - LibJPEG/LibJPEG_Decoding/Src/usbh_conf.c                 General low level driver configuration

@par Hardware and Software environment

  - This example runs on STM32F730xx devices.
    
  - This example has been tested with STMicroelectronics STM32F7308-DISCO
    boards and can be easily tailored to any other supported device.
    
    
  - STM32F7308-DISCO Set-up
    - Connect a USB key to the USB HS connector (CN19).
    - USB_HS jumper should be fitted on CN8 connector.


@par How to use it ?

In order to make the program work, you must do the following:

  1. Select required configuration in memory.h in Templates\ExtMem_Boot\Inc.
  2. Program the internal Flash with the ExtMem_Boot (see below).
  3. Program the external QSPI memory with this application (see below).
  4. Start debugging user application or reset for free running.

In order to load the ExtMem_Boot code :
   - Open your preferred toolchain :
      - Open the Project
      - Rebuild all files
      - Load project image

In order to load this application to the external memory:
   - Open your preferred toolchain
      - Open the Project
      - Rebuild all files:
      - Run & debug the program:
      - For an XiP configuration (eXecute in Place from QSPI):
          - Using EWARM or MDK-ARM : Load project image from the IDE: Project->Debug
          - Using SW4STM32 :
            - Open the STM32CubeProgrammer tool
            - Select the QSPI external flash loader "MX25L512G_STM32F7308-DISCO"
            - From Erasing & Programming menu, browse and open the output binary file relative to your application
            - Load the file into the external QSPI flash using "Start Programming" at the address APPLICATION_ADDRESS (0x90000000)


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   
