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
