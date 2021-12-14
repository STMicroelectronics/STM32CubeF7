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

