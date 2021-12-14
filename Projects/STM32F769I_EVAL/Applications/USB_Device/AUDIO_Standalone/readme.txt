/**
  @page AUDIO_Standalone USB Device AUDIO application
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/AUDIO_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Device AUDIO application.
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

Implementation of the audio streaming capability (the output is a speaker/headset).

It follows the "Universal Serial Bus Device Class Definition for Audio Devices Release 1.0 March 18, 
1998" defined by the USB Implementers Forum for reprogramming an application through USB-FS-Device. 
Following this specification, it is possible to manage only Full Speed USB mode (High Speed is not supported).
This class is natively supported by most Operating Systems: no need for specific driver setup.

This is a typical application on how to use the STM32F7xx USB OTG Device peripheral and SAI peripheral to 
stream audio data from USB Host to the audio codec implemented on the STM32F769I-EVAL board.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 216 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs: main PLL or PLL SAI.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  – PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL VCO clock must be programmed
    to output 432 MHz frequency (USBCLK = PLLVCO/PLLQ).
  – PLLSAI clock (clocked by the HSE): If the USB uses the PLLSAI as clock source, the PLLSAI VCO clock must be programmed
    to output 384 MHz frequency (USBCLK = PLLSAIVCO/PLLSAIP).

Please note, in this application the Audio clock is generated from an integrated PLL (PLLI2S).

The device supports the following audio features:
  - Pulse Coded Modulation (PCM) format
  - sampling rate: 48KHz. 
  - Bit resolution: 16
  - Number of channels: 2
  - No volume control
  - Mute/Unmute capability
  - Asynchronous Endpoints

In order to overcome the difference between USB clock domain and STM32 clock domain,
the Add-Remove mechanism is implemented at class driver level.
This is a basic solution that doesn't require external components. It is based
on adding/removing one sample at a periodic basis to speed-up or slowdown
the audio output process. This allows to resynchronize it with the input flow.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about the STM32Cube USB Device library, please refer to UM1734 
"STM32Cube USB Device library".

@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_FS" when using USB Full Speed (FS) Core
      - "USE_USB_HS" and "USE_USB_HS_IN_FS" when using USB High Speed (HS) Core in FS mode

It is possible to fine tune needed USB Device features by modifying defines values in USBD configuration
file “usbd_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:      
 - USBD_AUDIO_FREQ, specifying the sampling rate conversion from original audio file sampling rate to the
   sampling rate supported by the device.   
    
         
@par Keywords

Connectivity, USB_Device, USB, Audio, Streaming, SAI, Full speed, PCM
         
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

  - USB_Device/AUDIO_Standalone/Src/main.c                  Main program
  - USB_Device/AUDIO_Standalone/Src/system_stm32f7xx.c      STM32F7xx system clock configuration file
  - USB_Device/AUDIO_Standalone/Src/stm32f7xx_it.c          Interrupt handlers
  - USB_Device/AUDIO_Standalone/Src/usbd_audio_if.c         USBD Audio interface
  - USB_Device/AUDIO_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/AUDIO_Standalone/Src/usbd_desc.c             USB device AUDIO descriptor                                    
  - USB_Device/AUDIO_Standalone/Inc/main.h                  Main program header file
  - USB_Device/AUDIO_Standalone/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - USB_Device/AUDIO_Standalone/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - USB_Device/AUDIO_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/AUDIO_Standalone/Inc/usbd_desc.h             USB device AUDIO descriptor header file
  - USB_Device/AUDIO_Standalone/Inc/usbd_audio_if.h         USBD Audio interface header file  


@par Hardware and Software environment

  - This application runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This application has been tested with STMicroelectronics STM32F769I-EVAL
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32F769I-EVAL Set-up
    - Connect the STM32F769I-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector:
      - CN13: to use USB Full Speed (FS)
              Note that JP 14 must be removed in this case.
      - CN14: to use USB HS-IN-FS.
              Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM32F769I-EVAL Evaluation Board 
              User Manual.

    - Use CN28 connector to connect the board to external headset
    - Please ensure that jumpers JP3 and JP6 are fitted in position 1-2 (Audio)	
    - Please ensure that jumpers JP23 and JP24 are fitted in position 1-2 (Audio)
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32F769I-EVAL_USBH-FS: to configure the project for STM32F7xx devices using USB OTG FS peripheral
   - STM32F769I-EVAL_USBH-HS-IN-FS: to configure the project for STM32F7xx devices and use USB OTG HS 
                                   peripheral In FS (using embedded PHY).
 - Run the application
 - Open an audio player application (Windows Media Player) and play music on the PC host


 */
