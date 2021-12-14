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
  

#include <gui/controls_screen/ControlsPresenter.hpp>
#include <gui/controls_screen/ControlsView.hpp>

ControlsPresenter::ControlsPresenter(ControlsView& v) : 
    DemoPresenter(v),
    view(v)
{
}

void ControlsPresenter::activate()
{
#ifndef SIMULATOR
#ifdef USE_STM32769I_DISCO
    model->setScanJunctionTemp(true);
    view.setJunctionTempValue(model->getTempValue());
#else
    model->setScanPotentiometer(true);
    view.setPotentiometerValue(model->getPotentiometerValue());
#endif
#endif
    origBrightnessValue = model->getBrightnessValue();
    view.setBrightnessValue(model->getBrightnessValue());
}

void ControlsPresenter::deactivate()
{
#ifndef SIMULATOR
#ifdef USE_STM32769I_DISCO
    model->setScanJunctionTemp(false);
#else
    model->setScanPotentiometer(false);
#endif
#endif
    model->setBrightnessValue(origBrightnessValue);
}

#ifdef USE_STM32769I_DISCO
void ControlsPresenter::newJunctionTempValue(int value)
{
    view.setJunctionTempValue(value);
}
#else
void ControlsPresenter::newPotentiometerValue(int value)
{
    view.setPotentiometerValue(value);
}
#endif

void ControlsPresenter::newBrightnessValue(int value)
{
    model->setBrightnessValue(value);
}
