/**
  @page JPEG_EncodingUsingFs_DMA : JPEG example : JPEG encoding using Fatfs and with DMA example

  @verbatim
  ******************************************************************************
  * @file    JPEG/JPEG_EncodingUsingFs_DMA/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the JPEG encoding using Fatfs and with DMA example.
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

This example demonstrates how to read bmp file from SDCard memory using Fatfs, encode it
using the JPEG HW encoder in DMA mode and save it in SDCard.

The example performs the following actions :
  - Link the uSD disk I/O driver.
  - Register the file system object (mount) to the FatFs module for the uSD drive.
  - Open the bmp file "image.bmp" in read access.
  - Create the JPEG file with write access.
  - Call the function "JPEG_Encode_DMA" to start encoding the jpg file with DMA (Non blocking method).
    Function "JPEG_Encode_DMA" is implemented in a separate module encode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver
  - Save encoded data in jpg file
  - Once the encoding ends, close the bmp and jpg file.
  - The Green LED1 is On if the test ends properly.

@note To run this example , you can use a BMP “.bmp file from\Utilities\Media\Pictures\BMP_XXXxXXX
      or use your own bmp image. The input BMP file should be 24bpp (i.e pixels are RGB888 format).
      To double check that the input BMP file is compatible, right click on file, go to "Details"
      and check that "Bit depth" is "24".
      The BMP file width and height must verify the following requirement:
      - If encoding parameters YCbCr 4:2:0 then width and height must be multiple of 16 pixels.
      - For all other modes, width and height must be multiple of 8 pixels.
      The encoding parameters are set in the "main.h" (JPEG_COLOR_SPACE and JPEG_CHROMA_SAMPLING defines)

@note The jpeg HW encoder inputs are organized in YCbCr blocks.
      The jpeg utilities functions implemented in "jpeg_utils.c" file are used to convert the input RGB pixels
      to YCbCr blocks.
      The JPEG utilities files allowing to convert RGB pixels to YCbCr blocks are available under \Utilities\JPEG

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

@note The compiler optimizations level could be set to High/Balanced giving a good tradeoff between the code size and the performance (decode time).

@note In case of EVAL's LCD use in example code, the DATA_IN_ExtSDRAM define
      need to be added  in the project preprocessor to avoid SDRAM multiple configuration
     (the LCD uses SDRAM as frame buffer, and its configuration is done by the BSP_SDRAM_Init()).

@par Keywords

Display, Graphic, JPEG, Encoding, Flash memory, DMA, ARGB8888, Hardware Encoding, LCD, HDMI, YCbCr, RGB,
FatFS, File system, SD Card, SDRAM

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

  - JPEG/JPEG_EncodingUsingFs_DMA/Inc/stm32f7xx_hal_conf.h          HAL configuration file
  - JPEG/JPEG_EncodingUsingFs_DMA/Inc/stm32f7xx_it.h                Interrupt handlers header file
  - JPEG/JPEG_EncodingUsingFs_DMA/Inc/ffconf.h                      FAT file system module configuration file
  - JPEG/JPEG_EncodingUsingFs_DMA/Inc/main.h                        Header for main.c module
  - JPEG/JPEG_EncodingUsingFs_DMA/Inc/sd_diskio.h                   FatFS sd diskio header file
  - JPEG/JPEG_EncodingUsingFs_DMA/Inc/encode_dma.h                  Header for encode_dma.c module
  - JPEG/JPEG_EncodingUsingFs_DMA/Inc/jpeg_utils_conf.h             JPEG utilties module configuration file

  - JPEG/JPEG_EncodingUsingFs_DMA/Src/stm32f7xx_it.c                Interrupt handlers
  - JPEG/JPEG_EncodingUsingFs_DMA/Src/main.c                        Main program
  - JPEG/JPEG_EncodingUsingFs_DMA/Src/sd_diskio.c                   FatFS sd diskio driver implementation
  - JPEG/JPEG_EncodingUsingFs_DMA/Src/encode_dma.c                  JPEG Encoding with DMA module
  - JPEG/JPEG_EncodingUsingFs_DMA/Src/stm32f7xx_hal_msp.c           HAL MSP module
  - JPEG/JPEG_EncodingUsingFs_DMA/Src/system_stm32f7xx.c            STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F769xx/STM32F779xx devices.

  - This example has been tested with STMicroelectronics STM32F769I_EVAL revB
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32F769I_EVAL Set-up
  - Copy a bmp image to a uSD card (under the root)
      The image must have the following properties:
        - named as "image.bmp"
  - Connect the uSD Card to the uSD1 connector (CN17).

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
