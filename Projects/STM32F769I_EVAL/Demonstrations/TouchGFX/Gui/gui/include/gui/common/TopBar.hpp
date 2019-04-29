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
