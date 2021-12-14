/**
  ******************************************************************************
  * @file    stm32f7xx_it.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F7xx_IT_H
#define __STM32F7xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void DMA2_Stream6_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void DMA2_Stream1_IRQHandler(void);
void DCMI_IRQHandler(void);
void DMA2D_IRQHandler(void);
void DMA2_Stream0_IRQHandler(void);
void AUDIO_DFSDM_DMAx_RIGHT_IRQHandler(void);
void BSP_LCD_DMA2D_IRQHandler(void);
void BSP_LCD_DSI_IRQHandler(void);
void BSP_LCD_LTDC_IRQHandler(void);
void BSP_LCD_LTDC_ER_IRQHandler(void);
void BSP_SDRAM_DMA_IRQHandler(void);
void BSP_SRAM_DMA_IRQHandler(void);
void BSP_SDMMC1_IRQHandler(void);
void BSP_SDMMC2_IRQHandler(void);
void BSP_SDMMC1_DMA_Rx_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F7xx_IT_H */

