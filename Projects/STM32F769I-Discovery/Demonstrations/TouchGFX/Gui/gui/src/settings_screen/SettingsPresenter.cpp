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
  

#include <gui/settings_screen/SettingsPresenter.hpp>
#include <gui/settings_screen/SettingsView.hpp>

SettingsPresenter::SettingsPresenter(SettingsView& v) : 
    DemoPresenter(v),
    view(v)
{
}

void SettingsPresenter::activate()
{
    view.setSelectedClock(model->getSelectedClock());
    view.updateTime(model->getCurrentTime().hours, model->getCurrentTime().minutes, model->getCurrentTime().seconds, false);
    view.updateDate(model->getDateYear(), model->getDateMonth(), model->getDateDay());
}

void SettingsPresenter::deactivate()
{

}

void SettingsPresenter::clockSelected(int clockIndex)
{
    model->setSelectedClock(clockIndex);
}

void SettingsPresenter::updateTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    view.updateTime(hour, minute, second, true);
}

void SettingsPresenter::stateUpdated(int clockIndex, uint8_t hour, uint8_t minute, uint8_t second, uint16_t year, uint8_t month, uint8_t day) 
{
    model->setSelectedClock(clockIndex);
    model->setTime(hour, minute, second);
    model->setDate(year, month, day);
    view.updateDate(model->getDateYear(), model->getDateMonth(), model->getDateDay());
}
