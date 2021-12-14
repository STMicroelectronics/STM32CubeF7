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
