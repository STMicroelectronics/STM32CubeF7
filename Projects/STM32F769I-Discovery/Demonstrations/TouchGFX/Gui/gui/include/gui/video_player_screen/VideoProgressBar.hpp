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
  

#ifndef VIDEO_PROGRESS_BAR_HPP
#define VIDEO_PROGRESS_BAR_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

using namespace touchgfx;

class VideoProgressBar : public Container
{
public:
    VideoProgressBar();
    void setMovieData(uint32_t numberOfFrames, uint32_t msPerFrame);
    void setProgress(uint32_t procent);

    void setCallback(GenericCallback< const uint32_t >& callback);

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
private:
    GenericCallback< const uint32_t >* dragCallback;
    Image left;
    Image right;
    uint32_t numberOfFrames;
    uint32_t msPerFrame;

    TextAreaWithOneWildcard progressText;
    Unicode::UnicodeChar progressWildcardBuf[8]; //2:59:59

};

#endif
