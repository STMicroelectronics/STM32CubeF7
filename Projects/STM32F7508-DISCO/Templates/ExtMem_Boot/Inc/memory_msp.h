/**
  ******************************************************************************
  * @file    memory_msp.h
  * @author  MCD Application Team
  * @brief   This file contains Header for msp function needed for external
             memory initialization.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MEMORY_PRIVATE_H
#define __MEMORY_PRIVATE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/** @addtogroup MEMORY_DEFS
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void SRAM_MspInit(void);
void SDRAM_MspInit(void);
void QSPI_MspInit(void);
void QSPI_MspDeInit(void);
void NOR_MspInit(void);
void SDCARD_MspInit(void);
void SDCARD_MspDeInit(void);

/* Exported macro ------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __MEMORY_PRIVATE_H */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
