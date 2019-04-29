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
