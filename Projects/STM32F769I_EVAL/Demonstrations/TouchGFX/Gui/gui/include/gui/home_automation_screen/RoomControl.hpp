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
