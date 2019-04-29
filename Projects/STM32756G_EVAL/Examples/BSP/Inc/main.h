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
#include "stm32756g_eval.h"
#include "stm32756g_eval_lcd.h"
#include "stm32756g_eval_sd.h"
#include "stm32756g_eval_eeprom.h"
#include "stm32756g_eval_nor.h"
#include "stm32756g_eval_sram.h"
#include "stm32756g_eval_io.h"
#include "stm32756g_eval_ts.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50]; 
  uint32_t DemoIndex;
}BSP_DemoTypedef;

/* Exported variables --------------------------------------------------------*/
extern const unsigned char stlogo[];

/* Exported constants --------------------------------------------------------*/

/* Defines for the Audio playing process */
#define PAUSE_STATUS     ((uint32_t)0x00) /* Audio Player in Pause Status */
#define RESUME_STATUS    ((uint32_t)0x01) /* Audio Player in Resume Status */
#define IDLE_STATUS      ((uint32_t)0x02) /* Audio Player in Idle Status */


#define LED_GREEN      LED1
#define LED_ORANGE      LED2
#define LED_RED      LED3
#define LED_BLUE      LED4

/* Exported macro ------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */
void LCD_demo (void);
void Log_demo(void);
void Joystick_demo (void);
void EEPROM_demo (void);
void TSENSOR_demo (void);
void Touchscreen_demo(void);
void AudioPlay_demo(void);
void SD_demo(void);
void NOR_demo(void);
void SRAM_demo(void);
void Touchscreen_Calibration(void);
uint16_t Calibration_GetX(uint16_t x);
uint16_t Calibration_GetY(uint16_t y);
uint8_t IsCalibrationDone(void);
uint8_t CheckForUserInput(void);
void Toggle_Leds(void);
void Error_Handler(void);
void SDRAM_demo(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
