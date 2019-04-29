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
