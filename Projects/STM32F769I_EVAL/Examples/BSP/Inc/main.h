/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stm32f7xx_hal.h"
#include "stm32f769i_eval.h"
#include "stm32f769i_eval_lcd.h"
#include "stm32f769i_eval_eeprom.h"
#include "stm32f769i_eval_audio.h"
#include "stm32f769i_eval_qspi.h"
#include "stm32f769i_eval_nor.h"
#include "stm32f769i_eval_sram.h"
#include "stm32f769i_eval_sd.h"
#include "stm32f769i_eval_io.h"
#include "stm32f769i_eval_ts.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50]; 
  uint32_t DemoIndex;
}BSP_DemoTypedef;

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

/* Exported variables --------------------------------------------------------*/
extern const unsigned char stlogo[];
extern __IO uint32_t SdmmcTest;
extern __IO uint32_t DemoSdramIndex;
extern uint8_t     MfxExtiReceived;
/* Exported constants --------------------------------------------------------*/

/* Defines for the Audio playing process */
#define LED_GREEN      LED1
#define LED_ORANGE     LED2
#define LED_RED        LED3
#define LED_BLUE       LED4

#define RGB565_BYTE_PER_PIXEL     2
#define ARBG8888_BYTE_PER_PIXEL   4

/* Camera have a max resolution of VGA : 640x480 */
#define CAMERA_RES_MAX_X          640
#define CAMERA_RES_MAX_Y          480

/**
  * @brief  SDRAM Write read buffer start address after CAM Frame buffer
  * Assuming Camera frame buffer is of size 800x480 and format RGB565 (16 bits per pixel).
  */
#define SDRAM_WRITE_READ_ADDR        ((uint32_t)0xC0177000)

#define SDRAM_WRITE_READ_ADDR_OFFSET ((uint32_t)0x0800)

#define AUDIO_REC_START_ADDR         SDRAM_WRITE_READ_ADDR
#define AUDIO_REC_TOTAL_SIZE         ((uint32_t) 0x0000E000)
#define AUDIO_RECPDM_START_ADDR      (AUDIO_REC_START_ADDR+AUDIO_REC_TOTAL_SIZE)

/* The Audio file is flashed with ST-Link Utility @ flash address =  AUDIO_SRC_FILE_ADDRESS */
#define AUDIO_SRC_FILE_ADDRESS       0x08080000   /* Audio file address in flash */
#define AUDIO_FILE_SIZE              0x80000

#define AUDIO_PLAY_SAMPLE        0
#define AUDIO_PLAY_RECORDED      1
/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */
void     Touchscreen_demo1 (void);
void     Touchscreen_demo2 (void);
void AudioPlay_demo (void);
void AudioRecord_demo(void);
uint8_t AUDIO_Process(void);
void LCD_demo (void);
void Log_demo(void);
void Joystick_demo (void);
void Joystick_SetCursorPosition(void);
void QSPI_demo (void);
void SD_demo (void);
void EEPROM_demo (void);
void NOR_demo(void);
void SRAM_demo(void);
void SRAM_DMA_demo (void);
void SDRAM_demo(void);
void SDRAM_DMA_demo (void);
uint8_t CheckForUserInput(void);
void Error_Handler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
