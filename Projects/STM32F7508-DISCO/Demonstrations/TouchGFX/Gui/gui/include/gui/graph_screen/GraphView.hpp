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
  #ifndef GRAPH_VIEW_HPP
#define GRAPH_VIEW_HPP

#include <gui_generated/graph_screen/GraphViewBase.hpp>
#include <gui/graph_screen/GraphPresenter.hpp>
#include <touchgfx/containers/SlideMenu.hpp>
#include <gui/graph_screen/graph_widget/Graph.hpp>
#include <gui/graph_screen/PieChart.hpp>
#include <gui/graph_screen/LegendEntry.hpp>
#include <touchgfx/mixins/MoveAnimator.hpp>

class GraphView : public GraphViewBase
{
public:
    GraphView();
    virtual ~GraphView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void handleTickEvent();

    void updateProcessorLoad(uint8_t mcuLoadValue);

protected:
    static const int NUMBER_OF_GRID_LINES = 4;
    static const int NUMBER_OF_PIE_CHART_SECTIONS = 6;

    enum State
    {
        GRAPH,
        PIE_CHART
    };

    State currentState;

    SlideMenu topMenu;

    Box graphGridLines[NUMBER_OF_GRID_LINES];
    TextAreaWithOneWildcard graphYValues[NUMBER_OF_GRID_LINES];
    Unicode::UnicodeChar graphYValuesbuf[NUMBER_OF_GRID_LINES][5];

    Graph primaryGraph;
    Graph secondaryGraph;

    MoveAnimator<PieChart> pieChart;

#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 pieChartPainter[6];
#elif USE_BPP==24
    PainterRGB888 pieChartPainter[6];
#elif USE_BPP==4
    PainterGRAY4 pieChartPainter[6];
#elif USE_BPP==2
    PainterGRAY2 pieChartPainter[6];
#else
#error Unknown USE_BPP
#endif
    MoveAnimator<LegendEntry> legendEntries[NUMBER_OF_PIE_CHART_SECTIONS];
    int pieChartX;
    int pieChartY;
    int donutX;
    int donutY;

    int pieChartAnimateOutDuration;
    int pieChartRedrawAnimateDuration;
    int pieChartRedraw2AnimateDuration;
    int pieChartDonutChangeAnimateDuration;
    int pieChartAnimateEntryDuration;
    int animateToDonutDelay;

    enum PieChartAnimationType
    {
        REDRAW_PIECHART, // Fold+new slices
        REDRAW_PIECHART2, // Unfold
        SWITCH_TO_PIECHART,
        SWITCH_TO_GRAPH,
        SWITCH_TO_FULL_CIRCLE,
        SWITCH_TO_SEMI_DONUT
    };
    int pieChartAnimationType;

    int tickCount;
    int graphX;
    int graphType;
    int leftX;
    int pointCounter;
    bool pieChartIsFull; ///< false means semi-donut

    Callback<GraphView, const MoveAnimator<LegendEntry>&> legendMoveEnded;
    void legendMoveEndedHandler(const MoveAnimator<LegendEntry>& legend);

    Callback<GraphView, const PieChart&, int> legendSelected;
    void legendSelectedHandler(const PieChart& thePieChart, int item);

    Callback<GraphView, const PieChart&> pieChartAnimationEnded;
    void pieChartAnimationEndedHandler(const PieChart& thePieChart);

    Callback<GraphView, const LegendEntry&> legendPressed;
    void legendPressedHandler(const LegendEntry& legend);

    void setCurrentState(State newState);
    void clearGraphs();
    void updateGraphSettings();
    void updateButtonSettings();
    void resetPieChart();
    void setAllButtonsTouchable(bool touchable);

    void newButtonPressed();
    void graphOrPieButtonPressed();
    void donutOrPieButtonPressed();
};

#endif // GRAPH_VIEW_HPP
