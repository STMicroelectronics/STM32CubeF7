/**
  ******************************************************************************
  * @file    k_bsp.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel bsp functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "k_bsp.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */

/* External variables --------------------------------------------------------*/
TS_StateTypeDef  TS_State = {0};

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes QSPI and CRC.
  * @param  None 
  * @retval None
  */
void k_BspInit(void)
{
  TS_IO_Init();

  HAL_Delay(500);
    
  BSP_TS_InitEx(240, 240, LCD_ORIENTATION_LANDSCAPE_ROT180);  
    
  /* Enable CRC to Unlock GUI */
  __HAL_RCC_CRC_CLK_ENABLE();  
}

/**
  * @brief  Read the coordinate of the point touched and assign their
  *         value to the variables u32_TSXCoordinate and u32_TSYCoordinate
  * @param  None
  * @retval None
  */
void k_TouchUpdate(void)
{
  static GUI_PID_STATE TS_State = {0, 0, 0, 0};
  __IO TS_StateTypeDef  ts;

  BSP_TS_GetState((TS_StateTypeDef *)&ts);
  
  ts.touchX[0] = TouchScreen_Get_Calibrated_X(ts.touchX[0]);
  ts.touchY[0] = TouchScreen_Get_Calibrated_Y(ts.touchY[0]);
  
  if((ts.touchX[0] >= LCD_GetXSize()) ||(ts.touchY[0] >= LCD_GetYSize()) ) 
  {
    ts.touchX[0] = 0;
    ts.touchY[0] = 0;
  }
  
  if((TS_State.Pressed != ts.touchDetected )||
    (TS_State.x != ts.touchX[0]) ||
      (TS_State.y != ts.touchY[0]))
{
  TS_State.Pressed = ts.touchDetected;
  if(ts.touchDetected) 
  {
    TS_State.x = ts.touchX[0];
    TS_State.y = ts.touchY[0];
    GUI_TOUCH_StoreStateEx(&TS_State);
  }
  else
  {
    GUI_TOUCH_StoreStateEx(&TS_State);
    TS_State.x = 0;
    TS_State.y = 0;      
  }
}
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
