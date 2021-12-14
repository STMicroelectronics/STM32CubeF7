/**
  @page HID_LPM_Standalone USB Device Humain Interface (HID) application with LPM
  
  @verbatim
  ******************************************************************************
  * @file    USB_Device/HID_LPM_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB HID_LPM application.
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

The STM32F7xx devices support the USB Link Power Management Protocol (LPM-L1) and complies with the USB 2.0
LPM-L1 ECN. The hpcd.Init.lpm_enable in the usbd_conf.c should be set to 1 to enable the support for LPM-L1
protocol in the USB stack.

LPM-L1 allows a USB host to configure the USB device into inactive state much faster than the normal 
USB suspend mode (L2). It also provides much faster wake-up times in the order of micro-seconds compared 
to the generic resume by host or upstream resume by device.

When LPM packet is received from the host, STM32 USB will Acknowledge the LPM packet
and it will enter in L1 suspend mode. During USB L1 suspend mode ,system will be
in STOP low power mode.

On Host L1 resume, STM32 will wakeup from STOP and USB resumes operations.

You can test L1 suspend/resume, by running the USBCV3.0 chapter9 for USB2.0 devices 
and select (in debug mode) test "TD9.21: LPM L1 Suspend Resume Test".
Please note that for running USBCV3.0, you'll need a PC with a USB3.0 (xHCI)
host controller (please refer to USBCV3.0 documentation for more informations).

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to 
implement the USB Link Power Management (LPM) feature and how it can be used with the selective suspend 
mechanism to reduce system power consumption, in this application the STM32 is enumerated as a HID device 
supporting (LPM) feature. This application is based on HID_LPM_Standalone application available in the same 
firmware package.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 216 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs: main PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  – PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL VCO clock must be programmed
    to output 432 MHz frequency (USBCLK = PLLVCO/PLLQ).
  – PLLSAI clock (clocked by the HSE): If the USB uses the PLLSAI as clock source, the PLLSAI VCO clock must be programmed
    to output 384 MHz frequency (USBCLK = PLLSAIVCO/PLLSAIP).

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
      - "USE_USB_HS" when using USB High Speed (HS) Core
      - "USE_USB_FS" when using USB Full Speed (FS) Core 
      - "USE_USB_HS" and "USE_USB_HS_IN_FS" when using USB High Speed (HS) Core in FS mode

@par Keywords

Connectivity, USB_Device, USB, HID, Full Speed, High Speed, Joystick, Mouse, Power Management, LPM, LPM-L1,
ECN, Suspend mode, Stop mode, 

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

  - USB_Device/HID_LPM_Standalone/Src/main.c                  Main program
  - USB_Device/HID_LPM_Standalone/Src/system_stm32f7xx.c      STM32F7xx system clock configuration file
  - USB_Device/HID_LPM_Standalone/Src/stm32f7xx_it.c          Interrupt handlers
  - USB_Device/HID_LPM_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/HID_LPM_Standalone/Src/usbd_desc.c             USB device HID descriptor
  - USB_Device/HID_LPM_Standalone/Inc/main.h                  Main program header file
  - USB_Device/HID_LPM_Standalone/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - USB_Device/HID_LPM_Standalone/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - USB_Device/HID_LPM_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/HID_LPM_Standalone/Inc/usbd_desc.h             USB device HID descriptor header file  

	
@par Hardware and Software environment

  - This application runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
    
  - This application has been tested with STMicroelectronics STM32769I-EVAL
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32769I-EVAL Set-up
    - Connect the STM32769I-EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector:
      - CN8 : to use USB High Speed (HS) 
      - CN13: to use USB Full Speed (FS)
      - CN14: to use USB HS-IN-FS.
              Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM32769I-EVAL Evaluation Board 
              User Manual.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32769I-EVAL_USBH-HS: to configure the project for STM32F7xx devices using USB OTG HS peripheral
   - STM32769I-EVAL_USBH-FS: to configure the project for STM32F7xx devices using USB OTG FS peripheral
   - STM32769I-EVAL_USBH-HS-IN-FS: to configure the project for STM32F7xx devices and use USB OTG HS 
                                   peripheral In FS (using embedded PHY).
 - Run the application

 @note Known Limitations
 The Low Power management (suspend/resume and Remote wakeup) In HS mode is not yet supported. 
 Only FS and HS-IN-FS mode are supported
 

 */
  