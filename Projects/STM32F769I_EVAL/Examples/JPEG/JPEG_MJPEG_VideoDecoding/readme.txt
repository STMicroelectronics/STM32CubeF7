/**
  @page JPEG_MJPEG_VideoDecoding : MJPEG video decoding example

  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_MJPEG_VideoDecoding/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the MJPEG video decoding example.
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

This example demonstrates how to use the HW JPEG decoder to decode an MJPEG video file
and display it on the DSI screen.
Note That the DSI peripheral is configured in command mode with tearing effect
set on external pin, and with partial refresh. The partial refresh is initialized to refresh
the left part of the screen only by sending the appropriate DSI command.

The example performs the following actions :
  - Link the uSD disk I/O driver.
  - Register the file system object (mount) to the FatFs module for the uSD drive.
  - Open the mjpeg file "video.avi" in read access.
  - if the file open succeed display a down count from 5 to 1 and then proceed to
    the video decoding loop.

  - In the video decoding loop and for each iteration , search for the offset of
     the next jpeg frame in the mjpeg file (by looking for the next JPEG SOI marker).
     If the frame offset is valid then start the decoding using the function "JPEG_Decode_DMA"
     (Non blocking method).
    Function "JPEG_Decode_DMA" is implemented in a separate module decode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver

  - Once the decoding ends,
    - if the decoded frame is the first one, call HAL routine "HAL_JPEG_GetInfo"
      to retrieve the image parameters :
      image width, image height, image quality (from 1% to 100%) color space and Chroma sampling.

    - copy the decoded ARGB8888 result buffer to the LCD Frame buffer for display:
        Using the DMA2D and positioning the image in the center of the LCD screen.
        The DMA2D checks first if a DSI refresh of the LCD frame buffer is ongoing .
        If it is the case, it waits for the end of the refresh before copying the new decoded frame
        to the LCD frame buffer.

    -  Once the DMLA2D transfer ends , ask for a display refresh using the following technique :
        The DMA2D transfer is done in interrupt mode (not blocking). Once the DMA2D transfer is complete,
        the corresponding callback is called. This latter callback sets the variable "pending_buffer" to "1".
        to inform that the LCD frame buffer is ready for refresh then Unmask the Tearing effect pin
        (by setting this pin to the right DSI alternate function).

        When the Tearing Effect callback is asserted, first it masks the TE pin (setting the AF pin to 0).
        Then ask for DSI refresh (calling HAL_DSI_Refresh function).
        As consequence the DSI refreshes the LEFT part of the screen only . Once the left part of the screen
        is refreshed , the refresh callback is asserted. This callback updates the LTDC setting and sends the
        appropriate DSI command to switch the partial refresh to the RIGHT part of the screen, then as for a refresh again.
        Once the RIGHT part of the screen is refreshed (so the full frame is refreshed left and right), the refresh
        callback is asserted again . In this case, this callback switches the partial refresh setting back to LEFT part,
        and sets the variable "pending_buffer" to "-1" to inform that the refresh has ends.

     - The decoding and display has ends, go to the next frame processing.

     - When the end of file is reached, close the jpg file and display the file decoding characteristics :
       Resolution, JPEG quality, Chroma sampling, average decoding time and number of decoded frames.


@note To Run this example , you can use an MJPEG “.avi” file from
      \Utilities\Media\Video or use your own MJPEG file.
     This file must have the following properties:
        - renamed as "video.avi"
        - resolution : less or equal to the LCD-DSI screen (i.e 800x480 for the STM32F769I_EVAL board)
        - Width  : <= 800
        - Height : <= 480
      The MJPEG file can be in “AVI” container format(without audio), or a sample MJPEG row file.

@note The jpeg HW decoder outputs are organized in YCbCr blocks.
      The jpeg utilities functions implemented in "jpeg_utils.c" file are used to convert these YCbCr blocks
      to RGB pixels to the destination JPEG output buffer.
      The JPEG utilities files allowing to convert YCbCr blocks to RGB pixels are available under \Utilities\JPEG

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed
      at a maximum system clock frequency of 200MHz.

@note The compiler optimizations level could be set to High/Balanced giving a good trade-off between the code size and the performance (decode time).

@note In case of EVAL's LCD use in example code, the DATA_IN_ExtSDRAM define
      need to be added  in the project preprocessor to avoid SDRAM multiple configuration
     (the LCD uses SDRAM as frame buffer, and its configuration is done by the BSP_SDRAM_Init()).

@par Keywords

Display, Graphic, MJPEG, Video, Decoding, LCD, Hardware Decoder, DSI, Command mode, tearing effect,
DMA, FatFS, File system, avi, YCbCr, RGB

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

  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/stm32f7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/stm32f7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/main.h                        Header for main.c module
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/sd_diskio.h                   FatFS  sd diskio driver header file.
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/decode_dma.h                  Header for decode_dma.c module
  - JPEG/JPEG_MJPEG_VideoDecoding/Inc/jpeg_utils_conf.h             JPEG utilities module configuration file

  - JPEG/JPEG_MJPEG_VideoDecoding/Src/stm32f7xx_it.c                Interrupt handlers
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/main.c                        Main program
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/sd_diskio.c                   FatFS sd diskio driver implementation
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/decode_dma.c                  JPEG Decoding with DMA module
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/stm32f7xx_hal_msp.c           HAL MSP module
  - JPEG/JPEG_MJPEG_VideoDecoding/Src/system_stm32f7xx.c            STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx devices.

  - This example has been tested with STMicroelectronics STM32F769I_EVAL revB
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32F769I_EVAL Set-up
  - Copy an MJPEG video file to the uSD card (under the root)
      The file must have the following properties:
        - named as "video.avi"
        - resolution : less or equal to the LCD-DSI screen (i.e 800x480 for the STM32F769I_EVAL board)
        - Width  : <= 800
        - Height : <= 480
      The MJPEG file can be in “AVI” container format(without audio), or a sample MJPEG raw file.
  - Connect the uSD Card to the uSD1 connector (CN17).

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
