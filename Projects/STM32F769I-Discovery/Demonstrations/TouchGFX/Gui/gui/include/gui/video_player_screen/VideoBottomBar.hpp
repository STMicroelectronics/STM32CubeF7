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
  

#ifndef VIDEO_BOTTOM_BAR_HPP
#define VIDEO_BOTTOM_BAR_HPP

#include <gui/common/BottomBar.hpp>

class VideoBottomBar : public BottomBar
{
public:
    VideoBottomBar();
    virtual ~VideoBottomBar() {}
private:
    virtual void buttonClicked(const AbstractButton& btn);
    static const int ON_OFF_LENGTH = 4;
    ToggleButton repeatButton;
    ToggleButton hwJPEGButton;
    TextAreaWithOneWildcard hwJPEGText;
    Unicode::UnicodeChar onOffBuf[ON_OFF_LENGTH];
};

#endif /* VIDEO_BOTTOM_BAR_HPP */
