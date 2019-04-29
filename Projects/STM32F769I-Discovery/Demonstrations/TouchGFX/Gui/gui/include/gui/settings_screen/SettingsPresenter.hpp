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
  

#ifndef SETTINGS_PRESENTER_HPP
#define SETTINGS_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class SettingsView;

class SettingsPresenter : public DemoPresenter
{
public:
    SettingsPresenter(SettingsView& v);
    virtual ~SettingsPresenter() {};

    virtual void activate();
    virtual void deactivate();

    void clockSelected(int clockIndex);

    virtual void updateTime(uint8_t hour, uint8_t minute, uint8_t second);

    void stateUpdated(int clockIndex, uint8_t hour, uint8_t minute, uint8_t second, uint16_t year, uint8_t month, uint8_t day);

private:
    SettingsPresenter();
    SettingsView& view;
};

#endif // SETTINGS_PRESENTER_HPP
