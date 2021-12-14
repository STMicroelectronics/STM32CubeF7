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
  #include <gui/easingequation_screen/EasingEquationView.hpp>
#include <gui/easingequation_screen/EasingEquationPresenter.hpp>

EasingEquationPresenter::EasingEquationPresenter(EasingEquationView& v)
    : view(v)
{
}

void EasingEquationPresenter::activate()
{

}

void EasingEquationPresenter::deactivate()
{

}

void EasingEquationPresenter::mcuLoadUpdated(uint8_t mcuLoadValue)
{
    view.updateProcessorLoad(mcuLoadValue);
}
