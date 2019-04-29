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
  

#ifndef ROOM_CONTROL_ADJUSTABLE_HPP
#define ROOM_CONTROL_ADJUSTABLE_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TouchArea.hpp>
#include <gui/home_automation_screen/RoomControl.hpp>
#include <gui/home_automation_screen/RoomControlWheel.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>

using namespace touchgfx;

class RoomControlAdjustable : public RoomControl
{
public:
    RoomControlAdjustable();
    virtual ~RoomControlAdjustable();

private:
    int selectedItemIndex;

    uint8_t roomValues[NUMBER_OF_ROOMS];

    RoomControlWheel centerCircleControl;
    Circle circle[NUMBER_OF_ROOMS];
    PainterRGB888 circleColorPainter;

    TouchArea cancelSelectionTouchArea;

    virtual void buttonClicked(const AbstractButton& source);

    bool isItemSelected();
    void updateRoomCircle(int index);
    void updateCenterCircle();
};

#endif /* ROOM_CONTROL_ADJUSTABLE_HPP */
