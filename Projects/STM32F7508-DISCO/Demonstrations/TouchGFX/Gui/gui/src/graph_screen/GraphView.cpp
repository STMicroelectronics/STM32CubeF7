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
  #include <gui/graph_screen/GraphView.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/common/Utils.hpp>

GraphView::GraphView() :
    tickCount(0),
    graphX(0),
    graphType(0),
    leftX(0),
    pointCounter(0),
    legendMoveEnded(this, &GraphView::legendMoveEndedHandler),
    legendSelected(this, &GraphView::legendSelectedHandler),
    pieChartAnimationEnded(this, &GraphView::pieChartAnimationEndedHandler),
    legendPressed(this, &GraphView::legendPressedHandler)
{

}

void GraphView::setupScreen()
{
    mcuLoad.setTextColor(Color::getColorFrom24BitRGB(0x0, 0x0, 0x0));

    remove(topMenuContainer);
    topMenu.setXY(0, 0);
    topMenu.setup(SlideMenu::SOUTH, Bitmap(BITMAP_GRAPH_TOP_MENU_LINE_ID), Bitmap(BITMAP_GRAPH_TOP_MENU_ACTIVATE_BUTTON_ID), Bitmap(BITMAP_GRAPH_TOP_MENU_ACTIVATE_BUTTON_ID), 0, 75, 203, 78);
    topMenu.setExpandedStateTimeout(300);
    topMenuContainer.setXY(0, 0);
    topMenu.add(topMenuContainer);
    topMenu.setVisiblePixelsWhenCollapsed(Bitmap(BITMAP_GRAPH_TOP_MENU_ACTIVATE_BUTTON_ID).getHeight());
    add(topMenu);

    int graphXOffset = 44;
    int graphYOffset = 10;
    int graphWidth = 415;
    int graphHeight = graphBackground.getHeight() - graphYOffset;

    for (int i = 0; i < NUMBER_OF_GRID_LINES; i++)
    {
        graphGridLines[i].setColor(Color::getColorFrom24BitRGB(0xE1, 0xE4, 0xE6));
        graphGridLines[i].setPosition(graphXOffset, 38 + 40 * i, graphWidth, 1);
        graphArea.add(graphGridLines[i]);

        Unicode::snprintf(graphYValuesbuf[i], 5, "%d", 80 - i * 20);
        graphYValues[i].setTypedText(TypedText(T_GRAPH_Y_VALUE));
        graphYValues[i].setWildcard(graphYValuesbuf[i]);
        graphYValues[i].setColor(graphGridLines[i].getColor());
        graphYValues[i].resizeToCurrentText();
        graphYValues[i].setXY(graphGridLines[i].getX() - graphYValues[i].getWidth() - 6, graphGridLines[i].getY() - (graphYValues[i].getTypedText().getFont()->getFontHeight() / 2) - 2);
        graphArea.add(graphYValues[i]);
    }

    primaryGraph.setXY(graphXOffset, graphYOffset);
    primaryGraph.setup(graphWidth, graphHeight, Color::getColorFrom24BitRGB(0x24, 0x73, 0xAC), graphBackground.getColor());
    primaryGraph.setDotShape(0, 30, 5);
    primaryGraph.setDotBackgroundShape(0, 30, 7);
    graphArea.add(primaryGraph);

    secondaryGraph.setXY(primaryGraph.getX(), primaryGraph.getY());
    secondaryGraph.setup(graphWidth, graphHeight, Color::getColorFrom24BitRGB(0x27, 0x92, 0x34), graphBackground.getColor());
    secondaryGraph.setDotShape(0, 90, 6);
    secondaryGraph.setDotBackgroundShape(0, 90, 8);
    graphArea.add(secondaryGraph);

    primaryGraphLineSetting.forceState(true);
    primaryGraphDotSetting.forceState(true);
    primaryGraphAreaSetting.forceState(true);
    secondaryGraphLineSetting.forceState(true);
    secondaryGraphDotSetting.forceState(false);
    secondaryGraphAreaSetting.forceState(true);


    pieChartX = 0;
    pieChartY = -16;
    donutX = pieChartX;
    donutY = 34;
    pieChart.setPosition(pieChartX, pieChartY, 240, 240);
    pieChart.setCenter(120, 120);
    pieChart.setPieChart(0, 0, 0, 0);
    pieChart.recalculatePieChart();


    pieChartAnimateOutDuration = 16;
    pieChartRedrawAnimateDuration = 20;
    pieChartRedraw2AnimateDuration = 20;
    pieChartDonutChangeAnimateDuration = 40;
    pieChartAnimateEntryDuration = 35;
    animateToDonutDelay = 5;
    pieChart.setSelectAnimationSteps(4);

    colortype colors[NUMBER_OF_PIE_CHART_SECTIONS] =
    {
        Color::getColorFrom24BitRGB(0x13, 0x89, 0xCC),   // Blue
        Color::getColorFrom24BitRGB(0x18, 0xB6, 0x84),   // Green
        Color::getColorFrom24BitRGB(0xFF, 0x78, 0x00),   // Orange
        Color::getColorFrom24BitRGB(0xD9, 0x0C, 0x33),   // Red
        Color::getColorFrom24BitRGB(0xFF, 0xBA, 0x00),   // Yellow
        Color::getColorFrom24BitRGB(0xA8, 0x00, 0xFF)    // Purple
    };

    for (int i = 0; i < NUMBER_OF_PIE_CHART_SECTIONS; i++)
    {
        pieChartPainter[i].setColor(colors[i]);
        pieChart.setPainter(i, pieChartPainter[i]);
    }
    pieChart.setPieChartSelectionCallback(legendSelected);
    pieChart.setPieChartAnimationEndedCallback(pieChartAnimationEnded);
    pieChart.activatePieChartAnimationEndedCallback(false);

    graphArea.add(pieChart);

    legendEntries[0].setBitmaps(BITMAP_PIE_CHART_LEGEND_BLUE_ID, BITMAP_PIE_CHART_LEGEND_BLUE_ACTIVE_ID);
    legendEntries[1].setBitmaps(BITMAP_PIE_CHART_LEGEND_GREEN_ID, BITMAP_PIE_CHART_LEGEND_GREEN_ACTIVE_ID);
    legendEntries[2].setBitmaps(BITMAP_PIE_CHART_LEGEND_ORANGE_ID, BITMAP_PIE_CHART_LEGEND_ORANGE_ACTIVE_ID);
    legendEntries[3].setBitmaps(BITMAP_PIE_CHART_LEGEND_RED_ID, BITMAP_PIE_CHART_LEGEND_RED_ACTIVE_ID);
    legendEntries[4].setBitmaps(BITMAP_PIE_CHART_LEGEND_YELLOW_ID, BITMAP_PIE_CHART_LEGEND_YELLOW_ACTIVE_ID);
    legendEntries[5].setBitmaps(BITMAP_PIE_CHART_LEGEND_PURPLE_ID, BITMAP_PIE_CHART_LEGEND_PURPLE_ACTIVE_ID);

    for (int i = 0; i < NUMBER_OF_PIE_CHART_SECTIONS; i++)
    {
        legendEntries[i].setText((TEXTS)(T_GRAPH_LEGEND_TEXT_00 + i));
        legendEntries[i].setXY(HAL::DISPLAY_WIDTH, 50 + (i % 3) * 42);
        legendEntries[i].setSelectionCallback(legendPressed);
        graphArea.add(legendEntries[i]);
    }
    legendEntries[0].setMoveAnimationEndedAction(legendMoveEnded);

    setCurrentState(GRAPH);

    updateButtonSettings();
    updateGraphSettings();
    resetPieChart();
}

void GraphView::tearDownScreen()
{

}

void GraphView::handleTickEvent()
{
    tickCount++;

    if (currentState == GRAPH)
    {
        if (pointCounter < primaryGraph.NUMBER_OF_POINTS)
        {
            switch (graphType)
            {
            case 0:
            {
                primaryGraph.addValue(graphX, (int)((CWRUtil::toQ5(150) * CWRUtil::sine(graphX + Utils::randomNumberBetween(-25, 25))).to<int>()) + 200 + Utils::randomNumberBetween(-50, 50));
                secondaryGraph.addValue(graphX, (int)((CWRUtil::toQ5(150) * CWRUtil::sine(graphX + 230)).to<int>()) + 200 + Utils::randomNumberBetween(-50, 50));
                graphX += 15;

                pointCounter++;
                break;
            }
            case 1:
            case 2:
            {
                static int value1;
                static int value2;
                int graph1min = graphType == 1 ? 100 : 10;
                int graph1max = graphType == 1 ? 390 : 390;
                int graph2min = graphType == 1 ? 10 : 10;
                int graph2max = graphType == 1 ? 300 : 390;
                int randomMax = graphType == 1 ? 50 : 100;
                if (graphX == 0)
                {
                    value1 = Utils::randomNumberBetween(graph1min, graph1max);
                    value2 = Utils::randomNumberBetween(graph2min, graph2max);
                    if (graphType == 1 && value1 < value2)
                    {
                        int value0 = value1;
                        value1 = value2;
                        value2 = value0;
                    }
                }

                value1 += Utils::randomNumberBetween(-randomMax, randomMax);
                value2 += Utils::randomNumberBetween(-randomMax, randomMax);
                if (value1 < graph1min)
                {
                    value1 += Utils::randomNumberBetween(randomMax, 2 * randomMax);
                }
                if (value1 > graph1max)
                {
                    value1 -= Utils::randomNumberBetween(randomMax, 2 * randomMax);
                }
                if (value2 < graph2min)
                {
                    value2 += Utils::randomNumberBetween(randomMax, 2 * randomMax);
                }
                if (value2 > graph2max)
                {
                    value2 -= Utils::randomNumberBetween(randomMax, 2 * randomMax);
                }
                if (graphType == 1)
                {
                    if (value1 < value2)
                    {
                        int value0 = value1;
                        value1 = value2;
                        value2 = value0;
                    }
                    if (value1 - value2 < 20)
                    {
                        value1 += 10;
                        value2 -= 10;
                    }
                }
                primaryGraph.addValue(graphX, value1);
                secondaryGraph.addValue(graphX, value2);

                graphX += 15;

                pointCounter++;
                break;
            }
            }
        }
    }

}

void GraphView::setCurrentState(State newState)
{
    currentState = newState;

    clearGraphs();
    resetPieChart();

    pieChart.setVisible(currentState == PIE_CHART);
    primaryGraph.setVisible(currentState == GRAPH);
    secondaryGraph.setVisible(currentState == GRAPH);
    pieChart.invalidate();
    primaryGraph.invalidate();
    secondaryGraph.invalidate();

    primaryGraphLineSetting.setVisible(currentState == GRAPH);
    primaryGraphDotSetting.setVisible(currentState == GRAPH);
    primaryGraphAreaSetting.setVisible(currentState == GRAPH);
    secondaryGraphLineSetting.setVisible(currentState == GRAPH);
    secondaryGraphDotSetting.setVisible(currentState == GRAPH);
    secondaryGraphAreaSetting.setVisible(currentState == GRAPH);
    donutOrPieButton.setVisible(currentState == PIE_CHART);

    primaryGraphLineSetting.invalidate();
    primaryGraphDotSetting.invalidate();
    primaryGraphAreaSetting.invalidate();
    secondaryGraphLineSetting.invalidate();
    secondaryGraphDotSetting.invalidate();
    secondaryGraphAreaSetting.invalidate();
    donutOrPieButton.invalidate();

    if (currentState == GRAPH)
    {
        newButton.setBitmaps(Bitmap(BITMAP_NEW_GRAPH_ID), Bitmap(BITMAP_NEW_GRAPH_PRESSED_ID));
    }
    else
    {
        newButton.setBitmaps(Bitmap(BITMAP_NEW_PIE_CHART_ID), Bitmap(BITMAP_NEW_PIE_CHART_PRESSED_ID));
    }
    newButton.invalidate();

    for (int i = 0; i < NUMBER_OF_GRID_LINES; i++)
    {
        graphGridLines[i].setVisible(currentState == GRAPH);
        graphYValues[i].setVisible(currentState == GRAPH);
        graphGridLines[i].invalidate();
        graphYValues[i].invalidate();
    }

    updateButtonSettings();
}

void GraphView::clearGraphs()
{
    primaryGraph.clear();
    secondaryGraph.clear();
    primaryGraph.invalidate();

    pointCounter = 0;
    graphX = 0;
    leftX = 0;
    graphType++;
    graphType %= 3;
}

void GraphView::updateGraphSettings()
{
    topMenu.resetExpandedStateTimer();

    primaryGraph.setLineVisible(primaryGraphLineSetting.getState());
    primaryGraph.invalidate();

    primaryGraph.setDotsVisible(primaryGraphDotSetting.getState());
    primaryGraph.setDotsBackgroundVisible(primaryGraphDotSetting.getState());
    primaryGraph.invalidate();

    primaryGraph.setAreaVisible(primaryGraphAreaSetting.getState());
    primaryGraph.invalidate();

    secondaryGraph.setLineVisible(secondaryGraphLineSetting.getState());
    secondaryGraph.invalidate();

    secondaryGraph.setDotsVisible(secondaryGraphDotSetting.getState());
    secondaryGraph.setDotsBackgroundVisible(secondaryGraphDotSetting.getState());
    secondaryGraph.invalidate();

    secondaryGraph.setAreaVisible(secondaryGraphAreaSetting.getState());
    secondaryGraph.invalidate();
}

void GraphView::updateButtonSettings()
{
    if (currentState == GRAPH)
    {
        graphOrPieButton.setBitmaps(Bitmap(BITMAP_PIE_TOGGLE_BUTTON_ACTIVE_ID), Bitmap(BITMAP_PIE_TOGGLE_BUTTON_INACTIVE_ID));
    }
    else if (currentState == PIE_CHART)
    {
        graphOrPieButton.setBitmaps(Bitmap(BITMAP_GRAPH_TOGGLE_BUTTON_ACTIVE_ID), Bitmap(BITMAP_GRAPH_TOGGLE_BUTTON_INACTIVE_ID));
    }
    graphOrPieButton.invalidate();

    if (pieChartIsFull)
    {
        donutOrPieButton.setBitmaps(Bitmap(BITMAP_HALF_CIRCLE_TOGGLE_BUTTON_ACTIVE_ID), Bitmap(BITMAP_HALF_CIRCLE_TOGGLE_BUTTON_INACTIVE_ID));
    }
    else
    {
        donutOrPieButton.setBitmaps(Bitmap(BITMAP_PIE_TOGGLE_BUTTON_ACTIVE_ID), Bitmap(BITMAP_PIE_TOGGLE_BUTTON_INACTIVE_ID));
    }
    donutOrPieButton.invalidate();
}

void GraphView::resetPieChart()
{
    int shares[NUMBER_OF_PIE_CHART_SECTIONS];

    int remaining = 100;
    int sum = 0;

    for (int i = 0; i < NUMBER_OF_PIE_CHART_SECTIONS; i++)
    {
        shares[i] = Utils::randomNumberBetween(10, 100);
        sum += shares[i];
    }

    for (int i = 0; i < NUMBER_OF_PIE_CHART_SECTIONS - 1; i++)
    {
        shares[i] = (shares[i] * 100) / sum;
        remaining -= shares[i];
    }
    shares[NUMBER_OF_PIE_CHART_SECTIONS - 1] = remaining;
    pieChart.setSlices(shares);

    pieChart.recalculatePieChart();
    pieChart.invalidate();

    for (int i = 0; i < NUMBER_OF_PIE_CHART_SECTIONS; i++)
    {
        legendEntries[i].setValueText(shares[i]);
    }
}

void GraphView::setAllButtonsTouchable(bool touchable)
{
    newButton.setTouchable(touchable);

    graphOrPieButton.setTouchable(touchable);

    primaryGraphLineSetting.setTouchable(touchable);
    primaryGraphDotSetting.setTouchable(touchable);
    primaryGraphAreaSetting.setTouchable(touchable);

    secondaryGraphLineSetting.setTouchable(touchable);
    secondaryGraphDotSetting.setTouchable(touchable);
    secondaryGraphAreaSetting.setTouchable(touchable);

    donutOrPieButton.setTouchable(touchable);
}

void GraphView::legendMoveEndedHandler(const MoveAnimator<LegendEntry>& legend)
{
    if (&legend == &legendEntries[0])
    {
        if (legendEntries[0].getX() > HAL::DISPLAY_WIDTH - 1)
        {
            // Changing to Graph
            setCurrentState(GRAPH);
            setAllButtonsTouchable(true);
        }
    }
}

void GraphView::legendSelectedHandler(const PieChart& thePieChart, int item)
{
    if (item >= 0 && item < NUMBER_OF_PIE_CHART_SECTIONS)
    {
        legendEntries[item].setSelected(thePieChart.isSelected(item));
    }
}

void GraphView::pieChartAnimationEndedHandler(const PieChart& thePieChart)
{
    if (&thePieChart == &this->pieChart)
    {
        switch (pieChartAnimationType)
        {
        case REDRAW_PIECHART:
            resetPieChart();
            if (pieChartIsFull)
            {
                this->pieChart.startPieChartAnimation(0, 360, 0, 60, pieChartRedraw2AnimateDuration, EasingEquations::quadEaseOut, EasingEquations::quadEaseOut);
            }
            else
            {
                this->pieChart.startPieChartAnimation(-90, 90, 30, 90, pieChartRedraw2AnimateDuration, EasingEquations::quadEaseOut, EasingEquations::quadEaseOut);
            }
            pieChartAnimationType = REDRAW_PIECHART2;
            break;

        case REDRAW_PIECHART2:
        case SWITCH_TO_PIECHART:
        case SWITCH_TO_GRAPH:
        case SWITCH_TO_FULL_CIRCLE:
        case SWITCH_TO_SEMI_DONUT:
            this->pieChart.activatePieChartAnimationEndedCallback(false);
            setAllButtonsTouchable(true);
            break;
        }
    }
}

void GraphView::legendPressedHandler(const LegendEntry& legend)
{
    for (int i = 0; i < NUMBER_OF_PIE_CHART_SECTIONS; i++)
    {
        // pieChart will call back and (de)select legend
        if (&legend == &legendEntries[i])
        {
            pieChart.setSelected(i, !pieChart.isSelected(i));
        }
        else
        {
            pieChart.setSelected(i, false);
        }
    }
}

void GraphView::newButtonPressed()
{
    if (currentState == GRAPH)
    {
        clearGraphs();
    }
    else if (currentState == PIE_CHART)
    {
        pieChart.activatePieChartAnimationEndedCallback(true);
        pieChartAnimationType = REDRAW_PIECHART;
        if (pieChartIsFull)
        {
            this->pieChart.startPieChartAnimation(180, 180, 0, 60, pieChartRedrawAnimateDuration, EasingEquations::quadEaseIn, EasingEquations::quadEaseIn);
        }
        else
        {
            this->pieChart.startPieChartAnimation(0, 0, 0, 108, pieChartRedrawAnimateDuration, EasingEquations::quadEaseIn, EasingEquations::quadEaseIn);
        }
        setAllButtonsTouchable(false);
    }
}

void GraphView::graphOrPieButtonPressed()
{
    if (currentState == PIE_CHART)
    {
        // Go to Graph view
        for (int i = 0; i < NUMBER_OF_PIE_CHART_SECTIONS; i++)
        {
            legendEntries[i].setMoveAnimationDelay((NUMBER_OF_PIE_CHART_SECTIONS - 1 - i) * 2);
            legendEntries[i].startMoveAnimation(HAL::DISPLAY_WIDTH, legendEntries[i].getY(), 16, EasingEquations::cubicEaseInOut);
        }
        int startAngle, endAngle;
        pieChart.getAngles(startAngle, endAngle);
        pieChart.startPieChartAnimation(startAngle, endAngle, 0, 0, pieChartAnimateOutDuration, EasingEquations::quintEaseIn, EasingEquations::quadEaseIn);
        pieChart.setMoveAnimationDelay(0);
        pieChart.startMoveAnimation(pieChartX, pieChartY, pieChartAnimateOutDuration, EasingEquations::linearEaseIn);
        pieChart.activatePieChartAnimationEndedCallback(true);
        pieChartAnimationType = SWITCH_TO_GRAPH;
        setAllButtonsTouchable(false);

        topMenu.setVisible(true);
        topMenu.invalidate();
    }
    else if (currentState == GRAPH)
    {
        // Go to Pie chart view
        pieChart.unselectAll();
        for (int i = 0; i < NUMBER_OF_PIE_CHART_SECTIONS; i++)
        {
            legendEntries[i].setSelected(false);
            legendEntries[i].setMoveAnimationDelay(i * 2);
            legendEntries[i].startMoveAnimation((i < 3) ? 236 : 356, legendEntries[i].getY(), 16, EasingEquations::cubicEaseInOut);
        }
        pieChart.setPieChart(-270, -270, 0, 0);
        pieChart.startPieChartAnimation(0, 360, 0, 60, pieChartAnimateEntryDuration, EasingEquations::quadEaseIn, EasingEquations::linearEaseOut);
        pieChart.setMoveAnimationDelay(0);
        pieChart.startMoveAnimation(pieChartX, pieChartY, 35, EasingEquations::linearEaseOut);
        pieChart.activatePieChartAnimationEndedCallback(true);
        pieChartAnimationType = SWITCH_TO_PIECHART;
        setCurrentState(PIE_CHART);
        setAllButtonsTouchable(false);
        pieChartIsFull = true;
        updateButtonSettings();

        topMenu.animateToState(SlideMenu::COLLAPSED);
        topMenu.setVisible(false);
        topMenu.invalidate();
    }
}

void GraphView::donutOrPieButtonPressed()
{
    if (pieChartIsFull)
    {
        // To semi donut
        pieChart.startPieChartAnimation(-90, 90, 30, 90, pieChartDonutChangeAnimateDuration, EasingEquations::quintEaseOut, EasingEquations::quintEaseIn);
        pieChart.setMoveAnimationDelay(animateToDonutDelay);
        pieChart.startMoveAnimation(donutX, donutY, pieChartDonutChangeAnimateDuration - animateToDonutDelay, EasingEquations::quintEaseInOut);
        pieChartAnimationType = SWITCH_TO_SEMI_DONUT;
    }
    else
    {
        // To full circle
        pieChart.startPieChartAnimation(0, 360, 0, 60, pieChartDonutChangeAnimateDuration, EasingEquations::quintEaseIn, EasingEquations::quadEaseOut);
        pieChart.setMoveAnimationDelay(0);
        pieChart.startMoveAnimation(pieChartX, pieChartY, pieChartDonutChangeAnimateDuration, EasingEquations::linearEaseInOut);
        pieChartAnimationType = SWITCH_TO_FULL_CIRCLE;
    }
    pieChartIsFull = !pieChartIsFull;
    updateButtonSettings();
    setAllButtonsTouchable(false);
    pieChart.activatePieChartAnimationEndedCallback(true);
}

void GraphView::updateProcessorLoad(uint8_t mcuLoadValue)
{
    mcuLoad.updateProcessorLoad(mcuLoadValue);
}
