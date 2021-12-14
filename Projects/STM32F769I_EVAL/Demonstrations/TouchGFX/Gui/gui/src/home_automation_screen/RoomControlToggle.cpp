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
  

#include <gui/home_automation_screen/RoomControlToggle.hpp>


RoomControlToggle::RoomControlToggle() :
    RoomControl()
{
    for (int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
        roomsSelected[i].setXY(rooms[i].getX(), rooms[i].getY());
        roomsSelected[i].setVisible(false);

        add(roomsSelected[i]);
    }
}

RoomControlToggle::~RoomControlToggle()
{
}

void RoomControlToggle::setup(BitmapId selected, bool* state)
{
    roomValues = state;

    for (int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
        roomsSelected[i].setBitmap(Bitmap(selected));
        roomsSelected[i].setVisible(roomValues[i]);
    }
}

void RoomControlToggle::buttonClicked(const AbstractButton& source)
{
    for (int i = 0; i < NUMBER_OF_ROOMS; i++)
    {
        if (&source == &(rooms[i]))
        {
            roomValues[i] = !roomValues[i];
            roomsSelected[i].setVisible(roomValues[i]);
            roomsSelected[i].invalidate();
        }
    }
}
