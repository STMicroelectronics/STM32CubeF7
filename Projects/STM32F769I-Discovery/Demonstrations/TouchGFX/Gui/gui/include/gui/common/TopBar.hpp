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
  

#ifndef VIDEO_PLAYER_TOPBAR_HPP
#define VIDEO_PLAYER_TOPBAR_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

class TopBar : public Container
{
public:
    TopBar();
    void setText(const char* text);
    void setBackgroundVisible(bool visible);
private:
    static const int TEXT_LENGTH = 40;
    Image background;
    FadeAnimator<TextAreaWithOneWildcard> titleText;
    Unicode::UnicodeChar titleWildcardBuf[TEXT_LENGTH];
};

#endif
