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
