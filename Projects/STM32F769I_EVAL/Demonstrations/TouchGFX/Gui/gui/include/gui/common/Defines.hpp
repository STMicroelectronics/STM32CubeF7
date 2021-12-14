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
