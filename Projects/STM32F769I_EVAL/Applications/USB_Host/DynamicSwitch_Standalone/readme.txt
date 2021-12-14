/**
  @page DynamicSwitch_Standalone USB Host Dynamic Switch application

  @verbatim
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the USB Host Dynamic Switch application.
  ******************************************************************************
  *
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

This application is a part of the USB Host Library package using STM32Cube firmware. It describes how to use
dynamically switch, on the same port, between available USB host applications on the STM32F7xx devices.

The USBH_RegisterClass() API is provided by the USB Host Library to load the class driver. In this
application MSC, HID and AUDIO classes are loaded and the user can start his application depending on the
connected device.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 200 Mhz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs: main PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

When the application is started, the connected USB device is detected in the appropriate mode
(MSC/HID/AUDIO) and gets initialized. The STM32 MCU behaves as a (MSC/HID/AUDIO) Host, it enumerates the
device and extracts VID, PID, manufacturer name, Serial number and product name information and displays it
on the LCD screen.

A menu is displayed depending on the connected device and the user can select any operation from the
menu using the Joystick buttons.

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

For more details about the STM32Cube USB Host library, please refer to UM1720
"STM32Cube USB Host library".


@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_HS" when using USB High Speed (HS) Core
      - "USE_USB_FS" when using USB Full Speed (FS) Core
      - "USE_USB_HS" and "USE_USB_HS_IN_FS" when using USB High Speed (HS) Core in FS mode

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file “usbh_conf.h” available under the project includes directory, in a way to fit the application
requirements, such as:
  - Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages on the LCD screen, lcd_log.c driver need to be added to the application sources.

  - Number of supported classes: USBH_MAX_NUM_SUPPORTED_CLASS


@par Keywords

Connectivity, USB_Host, USB, MSC, HID, AUDIO, Full-Speed, High-Speed, LCD, SD Card, Mass Storage, FatFS

@Note If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
      then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
      The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
      It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
      In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
      Even though the user must manage the cache coherence for read accesses.
      Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
      Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

@Note In case of using an AZERTY keyboard, user should add "AZERTY_KEYBOARD" define to ensure correct displaying taped characters.

@par Directory contents

  - USB_Host/DynamicSwitch_Standalone/Src/main.c                  Main program
  - USB_Host/DynamicSwitch_Standalone/Src/usbh_diskio_dma.c       FatFS usbh diskio driver implementation
  - USB_Host/DynamicSwitch_Standalone/Src/system_stm32f7xx.c      STM32F7xx system clock configuration file
  - USB_Host/DynamicSwitch_Standalone/Src/stm32f7xx_it.c          Interrupt handlers
  - USB_Host/DynamicSwitch_Standalone/Src/menu.c                  Main Menu State Machine
  - USB_Host/DynamicSwitch_Standalone/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/DynamicSwitch_Standalone/Src/msc_explorer.c          Explore the USB flash disk content
  - USB_Host/DynamicSwitch_Standalone/Src/file_operations.c       Write/read file on the disk
  - USB_Host/DynamicSwitch_Standalone/Src/msc_menu.c              MSC State Machine
  - USB_Host/DynamicSwitch_Standalone/Src/hid_menu.c              HID State Machine
  - USB_Host/DynamicSwitch_Standalone/Src/audio_menu.c            AUDIO State Machine
  - USB_Host/DynamicSwitch_Standalone/Src/audio_explorer.c        Explore the uSD content
  - USB_Host/DynamicSwitch_Standalone/Src/audio.c                 Audio Out (playback) interface API
  - USB_Host/DynamicSwitch_Standalone/Src/mouse.c                 HID mouse functions
  - USB_Host/DynamicSwitch_Standalone/Src/keyboard.c              HID keyboard functions
  - USB_Host/DynamicSwitch_Standalone/Inc/main.h                  Main program header file
  - USB_Host/DynamicSwitch_Standalone/Inc/usbh_diskio_dma.h       FatFS usbh diskio driver header file
  - USB_Host/DynamicSwitch_Standalone/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - USB_Host/DynamicSwitch_Standalone/Inc/lcd_log_conf.h          LCD log configuration file
  - USB_Host/DynamicSwitch_Standalone/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - USB_Host/DynamicSwitch_Standalone/Inc/usbh_conf.h             USB Host driver Configuration file
  - USB_Host/DynamicSwitch_Standalone/Inc/ffconf.h                FatFs Module Configuration file


@par Hardware and Software environment

  - This application runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.

  - This application has been tested with STMicroelectronics STM32F769I-EVAL
    evaluation boards and can be easily tailored to any other supported device
    and development board.

  - STM32F769I-EVAL Set-up
    - Insert a microSD card containing .Wav audio file into the STM32F769I-EVAL uSD slot (CN17)
    - Plug the USB device into the STM32F769I-EVAL board through 'USB micro A-Male
      to A-Female' cable to the connector:
      - CN8 : to use USB High Speed (HS)
      - CN13: to use USB Full Speed (FS)
      - CN14: to use USB HS-IN-FS.
              Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM32F769I-EVAL Evaluation Board
              User Manual.
        @note Make sure that :
         - jumper JP14 must be removed when using USB OTG FS

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32F769I-EVAL_USBH-HS: to configure the project for STM32F7xx devices using USB OTG HS peripheral
   - STM32F769I-EVAL_USBH-FS: to configure the project for STM32F7xx devices using USB OTG FS peripheral
   - STM32F769I-EVAL_USBH-HS-IN-FS: to configure the project for STM32F7xx devices and use USB OTG HS
                                   peripheral In FS (using embedded PHY).
 - Run the application


 */
