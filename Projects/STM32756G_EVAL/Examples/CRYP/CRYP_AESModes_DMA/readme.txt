/**
  @page CRYP_AESModes_DMA  AES ECB in DMA mode  Example
  
  @verbatim
  ******************************************************************************
  * @file    CRYP/CRYP_AESModes_DMA/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the CRYP AES ECB encryption/decryption in DMA mode
  *          
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


How to use the CRYPTO peripheral to encrypt/decrypt data(Plaintext/Ciphertext) using AES
ECB algorithm  in DMA mode with swapping.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32F756xx Devices :
The CPU at 216MHz 


The AES ECB requires:
1. Plaintext     : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext    : which will be decrypted with ECB algorithm & 128bits key.
   Data Byte swapping (DataType 8):
   Plaintext_8   : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext_8  : which will be decrypted with ECB algorithm & 128bits key.  
   Data half-word swapping (DataType 16):
   Plaintext_16  : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext_16 : which will be decrypted with ECB algorithm & 128bits key.
   Data bit swapping (DataType 1):
   Plaintext_1   : which will be encrypted with ECB algorithm & 128bits key.    
   Ciphertext_1  : which will be decrypted with ECB algorithm & 128bits key.
   
2. Key: is the parameter which determines the Ciphertext. In this example 128 bits
   key is used

CRYP peripheral must be initialized once from the beginning, then for each 
operation of encryption/decryption, only configuration should be made if needed.
 
The AES-ECB encryption/decryption in DMA mode provide :
   Encryptedtext: which is the encryption result of Plaintext, it is compared
   to Ciphertext.
   Decryptedtext: which is the Decryption result of Ciphertext,it is compared
   to Plaintext.
1. No swapping, Encryptedtext compared to Ciphertext. then Decryptedtext compared to Plaintext.
2. Byte swapping, Encryptedtext compared to Ciphertext_8. then Decryptedtext compared to Plaintext_8.
3. half-word swapping, Encryptedtext compared to Ciphertext_16. then Decryptedtext compared to Plaintext_16.
4. Bit swapping, Encryptedtext compared to Ciphertext_1. then Decryptedtext compared to Plaintext_1.

STM32756G-EVAL LEDs are used to monitor the encryption/decryption status:
 - LED1(GREEN) is ON when encryption/decryption and TAG generation is right.
 - LED3(RED) is ON when encryption or decryption or TAG generation is wrong.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Keywords

Security, CRYP, AES, ECB, NIST FIPS publication 197, hardware CRYP, 

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
  
  - CRYP/CRYP_AESModes_DMA/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESModes_DMA/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESModes_DMA/Inc/main.h                  Header for main.c module
  - CRYP/CRYP_AESModes_DMA/Src/stm32f7xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESModes_DMA/Src/main.c                  Main program
  - CRYP/CRYP_AESModes_DMA/Src/stm32f7xx_hal_msp.c     HAL MSP module 
  - CRYP/CRYP_AESModes_DMA/Src/system_stm32f7xx.c      STM32F7xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F756xx devices.
  
  - This example has been tested with an STMicroelectronics STM32756G-EVAL board revB
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 