/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
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
  


#include <touchgfx/hal/GPIO.hpp>

#include "stm32746g_discovery.h"
using namespace touchgfx;

static bool ledState;

// The STM32746G-Discovery board only supports a single LED.
// This GPIO utility links that LED to GPIO::RENDER_TIME.

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
