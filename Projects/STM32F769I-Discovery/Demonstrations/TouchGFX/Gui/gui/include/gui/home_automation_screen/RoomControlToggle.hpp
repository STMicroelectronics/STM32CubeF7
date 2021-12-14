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
