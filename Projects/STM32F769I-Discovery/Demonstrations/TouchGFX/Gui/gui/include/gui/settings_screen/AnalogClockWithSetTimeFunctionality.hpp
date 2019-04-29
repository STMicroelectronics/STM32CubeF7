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
