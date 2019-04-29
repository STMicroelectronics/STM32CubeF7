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
  

#include <touchgfx/Color.hpp>

#include <gui/common/TopBar.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>

TopBar::TopBar()
{
    titleWildcardBuf[0] = 0;

    background.setBitmap(Bitmap(BITMAP_COMMON_PLAYER_BAR_ID));

    //size matches movie player bar bitmap
    setWidth(background.getWidth());
    setHeight(background.getHeight());

    titleText.setTypedText(TypedText(T_TOPBAR_TITLE));
    titleText.setWildcard(titleWildcardBuf);
    titleText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    add(background);
    add(titleText);
}

void TopBar::setText(const char* text)
{
    if (text)
    {
        Unicode::strncpy(titleWildcardBuf, text, TEXT_LENGTH);
        titleWildcardBuf[TEXT_LENGTH - 1] = 0;
    }
    else
    {
        titleWildcardBuf[0] = 0;
    }

    titleText.invalidate();
    titleText.resizeToCurrentText();
    titleText.moveTo((getWidth() - titleText.getWidth()) / 2, (getHeight() - titleText.getHeight()) / 2);
    titleText.invalidate();
    titleText.setAlpha(0);
    titleText.startFadeAnimation(255, 10, EasingEquations::cubicEaseIn);
}

void TopBar::setBackgroundVisible(bool visible)
{
    background.setVisible(visible);
    background.invalidate();
}

