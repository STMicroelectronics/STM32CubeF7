/**
  @page FatFs_RAMDisk_RTOS   FatFs with RAM disk drive in RTOS mode application
 
  @verbatim
  ******************************************************************************
  * @file    FatFs/FatFs_RAMDisk_RTOS/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the FatFs with RAM disk drive in RTOS mode application
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

@par Application Description

How to use STM32Cube firmware with FatFs middleware component as a generic FAT
file system module. This example develops an application exploiting FatFs
features, with a RAM disk (SRAM) drive in RTOS mode configuration. 

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock
(SYSCLK) to run at 216 MHz.
 
The application is based on writing and reading back a text file from a drive,
it creates a normal priority thread and it's performed using FatFs APIs to 
access the FAT volume as described in the following steps: 

 - Link the SDRAM disk I/O driver;
 - Register the file system object (mount) to the FatFs module for the SDRAM drive;
 - Create a FAT file system (format) on the SDRAM drive;
 - Create and Open new text file object with write access;
 - Write data to the text file;
 - Close the open text file;
 - Open text file object with read access;
 - Read back data from the text file;
 - Close the open text file;
 - Check on read data from text file;
 - Unlink the SDRAM disk I/O driver.

It is worth noting that the application manages any error occurred during the 
access to FAT volume, when using FatFs APIs.

It is possible to fine tune needed FatFs features by modifying defines values 
in FatFs configuration file “ffconf.h” available under the project includes 
directory, in a way to fit the application requirements. 
 
STM32 Eval board's LEDs can be used to monitor the application status:
  - LED1 is ON when the application runs successfully.
  - LED3 is ON when any error occurs. 

@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set accordingly to the 
      OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in HAL time base ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the HAL time base interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the HAL time base interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the HAL time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about FatFs implementation on STM32Cube, please refer to UM1721 "Developing Applications 
on STM32Cube with FatFs".


@par Keywords

FatFS, RAMDisk, SRAM, FAT, File system, Mount, Format, FreeRTOS, RTOS

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
 
  - FatFs/FatFs_RAMDisk_RTOS/Inc/stm32f7xx_hal_conf.h              HAL configuration file
  - FatFs/FatFs_RAMDisk_RTOS/Inc/stm32f7xx_it.h                    Interrupt handlers header file
  - FatFs/FatFs_RAMDisk_RTOS/Inc/main.h                            Main program header file
  - FatFs/FatFs_RAMDisk_RTOS/Inc/ffconf.h                          FAT file system module configuration file   
  - FatFs/FatFs_RAMDisk_RTOS/Inc/sram_diskio.h                     SRAM diskio header file
  - FatFs/FatFs_RAMDisk_RTOS/Inc/FreeRTOSConfig.h                  FreeRTOS distribution file
  - FatFs/FatFs_RAMDisk_RTOS/Src/stm32f7xx_hal_timebase_tim.c      HAL time base functions
  - FatFs/FatFs_RAMDisk_RTOS/Src/stm32f7xx_it.c                    Interrupt handlers
  - FatFs/FatFs_RAMDisk_RTOS/Src/main.c                            Main program
  - FatFs/FatFs_RAMDisk_RTOS/Src/system_stm32f7xx.c                STM32F7xx system clock configuration file
  - FatFs/FatFs_RAMDisk_RTOS/Src/sram_diskio.c                     SRAM diskio source file
         
 
@par Hardware and Software environment

  - This application runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This application has been tested with STMicroelectronics STM32769I_EVAL 
    evaluation boards and can be easily tailored to any other supported device 
    and development board. 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application


 */