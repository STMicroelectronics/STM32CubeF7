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
  

#ifndef GUI_DEFINES_HPP
#define GUI_DEFINES_HPP

#include <touchgfx/hal/HAL.hpp>

class Defines
{
public:
    enum MainMenuType
    {
        ANIMATING_BUTTONS_MENU = 0,
        CAROUSEL_MENU,
        NO_MENU
    };

    enum DemoID
    {
        GAME2048 = 0,
        GAME2D,
        CONTROLS,
        AUDIO_PLAYER,
        HOME_AUTOMATION,
        SETTINGS,
        VIDEO_PLAYER,
        NUMBER_OF_DEMO_SCREENS,
        NO_DEMO_SCREEN
    };
};


#endif /* GUI_DEFINES_HPP */
