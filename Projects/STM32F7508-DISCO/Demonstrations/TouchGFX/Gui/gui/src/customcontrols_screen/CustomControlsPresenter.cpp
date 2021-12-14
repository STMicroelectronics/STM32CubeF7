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
