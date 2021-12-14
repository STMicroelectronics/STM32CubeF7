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
  

#ifndef ANALOG_CLOCK_WITH_SET_TIME_FUNCTIONALITY
#define ANALOG_CLOCK_WITH_SET_TIME_FUNCTIONALITY

#include <touchgfx/containers/Container.hpp>
#include <BitmapDatabase.hpp>
#include <gui/settings_screen/AnalogClock.hpp>

using namespace touchgfx;

/**
 * @class AnalogClockWithSetTimeFunctionality
 *
 * @brief An analog clock.
 *
 * @sa AnalogClock
 */
class AnalogClockWithSetTimeFunctionality : public AnalogClock
{
public:
    enum UpdateMode
    {
        HOUR_UPDATE,
        MINUTE_UPDATE,
        SECOND_UPDATE
    };

    AnalogClockWithSetTimeFunctionality();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    void setUpdateMode(UpdateMode mode);
	
	void setVolumeChangedCallback(GenericCallback<AnalogClockWithSetTimeFunctionality::UpdateMode, uint8_t>& callback) { timeSettingChangedCallback = &callback; }
	
private:
    UpdateMode updateMode;

    void updateHand(int newX, int newY);

    GenericCallback<AnalogClockWithSetTimeFunctionality::UpdateMode, uint8_t >* timeSettingChangedCallback;
};

#endif /* ANALOG_CLOCK_WITH_SET_TIME_FUNCTIONALITY */
