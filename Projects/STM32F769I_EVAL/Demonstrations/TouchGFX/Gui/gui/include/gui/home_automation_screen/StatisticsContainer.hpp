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
  

#ifndef STATISTICS_CONTAINER_HPP
#define STATISTICS_CONTAINER_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/home_automation_screen/Graph.hpp>
#include <gui/home_automation_screen/PainterRGB888TiledBitmap.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <gui/common/TiledImage.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

using namespace touchgfx;

class StatisticsContainer : public Container
{
public:
    StatisticsContainer();
    virtual ~StatisticsContainer();

    virtual void setup();

    virtual void handleTickEvent();
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    void update(int x);
private:
    enum
    {
        STAT_STATE_NONE,
        STAT_STATE_INIT,
        STAT_STATE_SHOW_OFF,
        STAT_STATE_READY
    };
    Image upper;
    TiledImage lower;
    Graph graph[3];
    PainterRGB888TiledBitmap painter[3];
    PainterRGB888 bgPainter;
    Box verticalBar;
    Circle circle[3];
    PainterRGB888Bitmap circlePainter[3];
    Image icon[3];
    TextAreaWithOneWildcard pctValue[3];
    Unicode::UnicodeChar buffer[3][4];
    TextArea pctSign[3];
    static int yScale;
    int numValues;
    int statState;
    float randFreq[3][5];
    float randAdd[3][5];
    int tick;
};

#endif /* STATISTICS_CONTAINER_HPP */
