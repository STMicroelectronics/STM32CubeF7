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
  #include <gui/customcontrols_screen/CustomControlsView.hpp>
#include <gui/customcontrols_screen/CustomControlsPresenter.hpp>

CustomControlsPresenter::CustomControlsPresenter(CustomControlsView& v)
    : view(v)
{
}

void CustomControlsPresenter::activate()
{

}

void CustomControlsPresenter::deactivate()
{

}

void CustomControlsPresenter::screenSaverMinorTick()
{
    view.screenSaverMinorTick();
}

void CustomControlsPresenter::screenSaverMajorTick()
{
    // Override default behaviour (from ModelListener.hpp)
    return;
}

void CustomControlsPresenter::mcuLoadUpdated(uint8_t mcuLoadValue)
{
    view.updateProcessorLoad(mcuLoadValue);
}
