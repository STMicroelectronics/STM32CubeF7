/**
  @page Binary Description of the binary file

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    USB_Host/FWupgrade_Standalone/Binary/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the binary file.
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

This directory contains a binary to be loaded into Flash memory using Device
Firmware Upgrade application.

This binary is a simple LED chaser. The 4 LEDs lights one by one for a period of 100 ms and the cycle
repeats giving the running light appearance.
The system Timer (Systick) is used for to generate the delay.
The offset address of this binary is 0x08008000 which matches the definition in DFU application
"USBD_DFU_APP_DEFAULT_ADD".

@Note�If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
 �����then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
������The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

@Note It is recommended to enable the cache and maintain its coherence, but depending on the use case
����� It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
������In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
������Even though the user must manage the cache coherence for read accesses.
������Please refer to the AN4838 �Managing memory protection unit (MPU) in STM32 MCUs�
������Please refer to the AN4839 �Level 1 cache on STM32F7 Series�

@par Hardware and Software environment

  - This application runs on STM32F756xx/STM32F746xx devices.

  - This application has been tested with STMicroelectronics STM327x6G_EVAL RevB
    evaluation boards and can be easily tailored to any other supported device
    and development board.


 */
