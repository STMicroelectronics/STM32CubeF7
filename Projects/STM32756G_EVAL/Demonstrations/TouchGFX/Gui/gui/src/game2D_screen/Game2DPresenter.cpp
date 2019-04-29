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
  


#include <gui/game2D_screen/Game2DPresenter.hpp>
#include <gui/game2D_screen/Game2DView.hpp>
#include <gui/common/FrontendApplication.hpp>

Game2DPresenter::Game2DPresenter(Game2DView& v) :
    DemoPresenter(v),
    view(v)
{
}

Game2DPresenter::~Game2DPresenter()
{
}

/**
 * Workaround for limitation in exc7200 touch driver on STM32F4 EVAL board:
 * This touch controller requires very frequent sampling in order to appear responsive.
 * On this particular screen, rendering times are quite long, so we must sample touch
 * every frame when running on this board.
 */
#if STM32F429xx || STM32F756xx
static int8_t origTouchSampleRate;
#endif

void Game2DPresenter::activate()
{

#if STM32F429xx || STM32F756xx
    /* Keep track of original samplerate, so we may restore it when leaving this screen */
    origTouchSampleRate = HAL::getInstance()->getTouchSampleRate();
#endif

#if STM32F429xx
    /* Set default touch sample rate (for this screen only) to 1 */
    origTouchSampleRate = HAL::getInstance()->getTouchSampleRate();
    HAL::getInstance()->setTouchSampleRate(1);
#endif

#if STM32F756xx
    /* Set default touch sample rate (for this screen only) to 2 */
    HAL::getInstance()->setTouchSampleRate(2);
#endif
    // ensure the timeliness of the animations
    HAL::getInstance()->setFrameRateCompensation(true);
}

void Game2DPresenter::deactivate()
{
#if STM32F429xx || STM32F756xx
    /* Restore original setting when leaving this screen. */
    HAL::getInstance()->setTouchSampleRate(origTouchSampleRate);
#endif
    HAL::getInstance()->setFrameRateCompensation(false);
}
