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
  

#ifndef ROOM_CONTROL_HPP
#define ROOM_CONTROL_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/containers/Container.hpp>

using namespace touchgfx;

class RoomControl : public Container
{
public:
    static const int NUMBER_OF_ROOMS = 10;

    RoomControl();
    virtual ~RoomControl();

protected:
    Button rooms[NUMBER_OF_ROOMS];

    Callback<RoomControl, const AbstractButton&> buttonClickedCallback;
    virtual void buttonClicked(const AbstractButton& source) = 0;
};

#endif /* ROOM_CONTROL_HPP */
