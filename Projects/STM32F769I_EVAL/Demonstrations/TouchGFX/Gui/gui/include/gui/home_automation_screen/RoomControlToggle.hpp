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
  

#ifndef ROOM_CONTROL_TOGGLE_HPP
#define ROOM_CONTROL_TOGGLE_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <gui/home_automation_screen/RoomControl.hpp>
#include <BitmapDatabase.hpp>

using namespace touchgfx;

class RoomControlToggle : public RoomControl
{
public:
    RoomControlToggle();
    virtual ~RoomControlToggle();

    void setup(BitmapId selected, bool* state);

private:
    Image roomsSelected[NUMBER_OF_ROOMS];
    bool* roomValues;

    virtual void buttonClicked(const AbstractButton& source);
};

#endif /* ROOM_CONTROL_TOGGLE_HPP */
