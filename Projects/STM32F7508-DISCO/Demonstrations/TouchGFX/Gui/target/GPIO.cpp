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
  
#include <touchgfx/hal/GPIO.hpp>

#include "stm32f7508_discovery.h"

using namespace touchgfx;

static bool ledState;

static bool isRenderTime(GPIO::GPIO_ID id)
{
    return id == GPIO::RENDER_TIME;
}

void GPIO::init()
{
    BSP_LED_Init(LED_GREEN);
    clear(GPIO::RENDER_TIME);
}

void GPIO::set(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        ledState = 1;
        BSP_LED_On(LED_GREEN);
    }
}

void GPIO::clear(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        ledState = 0;
        BSP_LED_Off(LED_GREEN);
    }
}

void GPIO::toggle(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        BSP_LED_Toggle(LED_GREEN);
    }
}

bool GPIO::get(GPIO::GPIO_ID id)
{
    if (isRenderTime(id))
    {
        return ledState;
    }
    return 0;
}
