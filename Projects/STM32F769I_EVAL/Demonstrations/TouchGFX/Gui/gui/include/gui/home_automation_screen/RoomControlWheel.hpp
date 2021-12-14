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
  

#ifndef ROOM_CONTROL_WHEEL_HPP
#define ROOM_CONTROL_WHEEL_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/TextArea.hpp>

using namespace touchgfx;

class RoomControlWheel : public Container
{
public:
    RoomControlWheel();
    virtual ~RoomControlWheel();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    void updateArcEnd(int newDegreeValue);
    void showKnob(int newDegreeValue);

    int getValue();
protected:
    Circle circle;
    PainterRGB888 circleColorPainter;
    Circle knob;
    PainterRGB888 knobColorPainter;

    TextAreaWithOneWildcard percent;
    Unicode::UnicodeChar percentBuffer[4];
    TextArea percentSign;

    void setSelectedPosition(int newX, int newY);
};

#endif /* ROOM_CONTROL_WHEEL_HPP */
