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
