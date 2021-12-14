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
