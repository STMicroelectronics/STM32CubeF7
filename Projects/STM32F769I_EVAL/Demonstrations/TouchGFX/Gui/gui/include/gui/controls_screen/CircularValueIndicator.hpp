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
  

#ifndef CIRCULAR_VALUE_INDICATOR_HPP
#define CIRCULAR_VALUE_INDICATOR_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/ListLayout.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>

#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565Bitmap.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>
#endif

using namespace touchgfx;


class CircularValueIndicator : public Container
{
public:
    CircularValueIndicator();
    virtual ~CircularValueIndicator();

    void setValue(int percentage);
    int getValue() { return value; }

private:
    static const int MIN_DEGREE = -138;
    static const int MAX_DEGREE = 138;


    Image background;
    TextureMapper arrow;

    TextAreaWithOneWildcard JunctionTempText;
    Unicode::UnicodeChar JunctionTempTextBuffer[5];
    TextArea JunctionTempPercentageText;

    Circle circle;

    int value;

#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565Bitmap myBitmapPainterCircle;
    PainterRGB565Bitmap myBitmapPainterGlow;
#elif USE_BPP==24
    PainterRGB888Bitmap myBitmapPainterCircle;
    PainterRGB888Bitmap myBitmapPainterGlow;
#endif

    void updateJunctionTempText(int value);
};

#endif /* CIRCULAR_VALUE_INDICATOR_HPP */
