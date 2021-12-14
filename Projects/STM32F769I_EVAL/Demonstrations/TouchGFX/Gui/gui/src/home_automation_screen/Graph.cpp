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
  

#include <gui/home_automation_screen/Graph.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <math.h>

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::setup(touchgfx::AbstractPainter& painter, touchgfx::AbstractPainter& bgPainter)
{
    graphLine.setPosition(0, 0, getWidth(), getHeight());
    graphLine.setBuffer(graphBuffer, NUMBER_OF_POINTS);
    graphLine.setLineWidth(4);
    graphLine.setXFactor(2);
    graphLine.setup(painter, bgPainter);

    add(graphLine);
    add(graphLine.outer);
    add(graphLine.inner);
}
