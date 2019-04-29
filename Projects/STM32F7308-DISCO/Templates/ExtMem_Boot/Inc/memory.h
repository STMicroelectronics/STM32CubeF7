/**
  ******************************************************************************
  * @file    Templates/ExtMem_Boot/Inc/memory.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          all external memory configuration helper.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 

/** @addtogroup MEMORY
  * @{
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MEMORY_H
#define __MEMORY_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"

/** @addtogroup MEMORY
  * @{
  */    

  
/* Exported constants --------------------------------------------------------*/ 
/* Exported types ------------------------------------------------------------*/

/** @defgroup MEMORY_Exported_Types Memory Exported Types
  * @{
  */
/* Error codes */
#define MEMORY_OK          ((uint32_t)0x00)
#define MEMORY_ERROR       ((uint32_t)0x01)


/* QSPI Init Mode */
#define QSPI_MODE          ((uint32_t)0x00)    /* Init in quad-spi mode for XiP mode */
#define SPI_NOR_MODE       ((uint32_t)0x01)    /* Emulate an spi-nor using QSPI with one line for BootROM scenario */
/**
  * @}
  */

#define USE_EXTERNAL_PSRAM   3
#define USE_INTERNAL_SRAM    4
#define USE_QSPI             5

/*
  @verbatim
  ==============================================================================
                     ##### How to use this Config #####
  ==============================================================================
    [..]
      The configuration bellow allows simple selection switch between configuration 

      The configuration is composed mainly by two area:
      (#) CODE_AREA: Used to specify external memory used for code execution
         (##) XiP Mode:
             (+++) USE_QSPI : QSPI Flash is used for code execution
      
      (#) DATA_AREA: Used to specify volatile memory used for data holding
         (##) USE_EXTERNAL_PSRAM : External PSRAM is used for code execution
         (##) USE_INTERNAL_SRAM  : Internal SRAM is used for code execution

      Bellow an automatic update APPLICATION_ADDRESS to default value based on
      user configuration, which can be tailored as required
             
      Finally a set of check allows to avoid unsupported combined configuration.

  @endverbatim
*/
#define DATA_AREA            USE_INTERNAL_SRAM
#define CODE_AREA            USE_QSPI

/*************************************************/
/*                                               */
/*     Configure Application Startup Address     */
/*                                               */
/*************************************************/
#if (CODE_AREA == USE_QSPI)
  #define APPLICATION_ADDRESS            QSPI_BASE
     
#else
  #error "APPLICATION_ADDRESS not defined"

#endif

/****************************************************/
/*                                                  */
/*     Check to avoid unsupported configuration     */
/*                                                  */
/****************************************************/
#if ((DATA_AREA != USE_EXTERNAL_PSRAM) && \
     (DATA_AREA != USE_INTERNAL_SRAM))
  #error "Wrong type used for DATA_AREA"
#endif
      
#if (CODE_AREA != USE_QSPI)
  #error "Wrong type used for CODE_AREA"
#endif

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Memory_Exported_Functions Non-Volatile memory
  * @{
  */
uint32_t QSPI_Startup(uint32_t Mode);
/**
  * @}
  */ 


/** @addtogroup Memory_Exported_Functions Volatile memory
  * @{
  */
uint32_t PSRAM_Startup(void);
/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __MEMORY_H */
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
