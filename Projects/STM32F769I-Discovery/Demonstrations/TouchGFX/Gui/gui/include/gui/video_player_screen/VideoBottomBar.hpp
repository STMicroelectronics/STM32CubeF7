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
