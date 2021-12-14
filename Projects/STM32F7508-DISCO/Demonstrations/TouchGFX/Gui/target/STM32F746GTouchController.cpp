/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  
#include "STM32F746GTouchController.hpp"

#include "stm32f7508_discovery_ts.h"

using namespace touchgfx;

extern "C" uint32_t LCD_GetXSize();
extern "C" uint32_t LCD_GetYSize();

void STM32F746GTouchController::init()
{
    if (BSP_TS_Init(LCD_GetXSize(), LCD_GetYSize()) == TS_OK)
    {
        isInitialized = true;
    }
}

bool STM32F746GTouchController::sampleTouch(int32_t& x, int32_t& y)
{
    if (isInitialized)
    {
        TS_StateTypeDef state;
        BSP_TS_GetState(&state);
        if (state.touchDetected)
        {
            x = state.touchX[0];
            y = state.touchY[0];

            return true;
        }
    }
    return false;
}
