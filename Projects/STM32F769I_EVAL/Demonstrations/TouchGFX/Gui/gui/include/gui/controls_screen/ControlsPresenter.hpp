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
  

#ifndef CONTROLS_PRESENTER_HPP
#define CONTROLS_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class ControlsView;

class ControlsPresenter : public DemoPresenter
{
public:
    ControlsPresenter(ControlsView& v);

    virtual void activate();
    virtual void deactivate();

    virtual ~ControlsPresenter() {};

#ifdef USE_STM32769I_DISCO
    virtual void newJunctionTempValue(int value);
#else
    virtual void newPotentiometerValue(int value);
#endif
    void newBrightnessValue(int value);

private:
    ControlsPresenter();
    ControlsView& view;
    uint8_t origBrightnessValue;
};

#endif // CONTROLS_PRESENTER_HPP
