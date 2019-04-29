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
  


#include <gui/custom_controls_screen/CustomControlsPresenter.hpp>
#include <gui/custom_controls_screen/CustomControlsView.hpp>
#include <gui/common/FrontendApplication.hpp>

CustomControlsPresenter::CustomControlsPresenter(CustomControlsView& v) :
    DemoPresenter(v),
    view(v)
{
}

CustomControlsPresenter::~CustomControlsPresenter()
{
}

/**
 * Workaround for limitation in touch driver on STM32F7 EVAL board:
 * This touch controller requires very frequent sampling in order to appear responsive.
 */
#if STM32F756xx
static int8_t origTouchSampleRate;
#endif

void CustomControlsPresenter::activate()
{
#if STM32F756xx
    /* Keep track of original samplerate, so we may restore it when leaving this screen */
    origTouchSampleRate = HAL::getInstance()->getTouchSampleRate();

    /* Set default touch sample rate (for this screen only) to 2 */
    HAL::getInstance()->setTouchSampleRate(2);
#endif
}

void CustomControlsPresenter::deactivate()
{
#if STM32F756xx
    /* Restore original setting when leaving this screen. */
    HAL::getInstance()->setTouchSampleRate(origTouchSampleRate);
#endif
}

void CustomControlsPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}
