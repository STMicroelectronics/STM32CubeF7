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
  #include <gui/easingequation_screen/EasingEquationView.hpp>

EasingEquationView::EasingEquationView() :
    tickCounter(0),
    moveAnimationCounter(0),
    onEasingEquationSelected(this, &EasingEquationView::easingEquationSelected),
    onInOrOutSelected(this, &EasingEquationView::inOrOutSelected)
{

}

void EasingEquationView::setupScreen()
{
    mcuLoad.setTextColor(Color::getColorFrom24BitRGB(0x41, 0x5D, 0x62));


    selector.setXY(0, 0);
    selector.setEasingEquationSelectedCallback(onEasingEquationSelected);
    add(selector);

    graph.setXY(185, 164 - 100 + GRAPH_Y_VALUE_OFFSET);
    graph.setup((NUMBER_OF_STEPS + 1) * 4, 100 - (2 * GRAPH_Y_VALUE_OFFSET), Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF), 2);
    graph.setRange(0, NUMBER_OF_STEPS + 1, (100 - GRAPH_Y_VALUE_OFFSET) * GRAPH_MULT, GRAPH_Y_VALUE_OFFSET * GRAPH_MULT);
    add(graph);

    inOrOut.setXY(selector.getRect().right(), HAL::DISPLAY_HEIGHT - inOrOut.getHeight());
    inOrOut.setInOrOutSelectedCallback(onInOrOutSelected);
    add(inOrOut);

    graphDot.setBitmap(Bitmap(BITMAP_GRAPHDOT_ID));
    graphDot.setVisible(false);
    add(graphDot);

    selector.select(1);
    inOrOut.selectFirst();
}

void EasingEquationView::tearDownScreen()
{

}

void EasingEquationView::updateProcessorLoad(uint8_t mcuLoadValue)
{
    mcuLoad.updateProcessorLoad(mcuLoadValue);
}

void EasingEquationView::easingEquationSelected(const EasingEquationSelection& selection)
{
    this->easingEquationSelection = selection;
    easingEquationName.setTypedText(TypedText(selection.EasingEquationName));
    easingEquationName.invalidate();

    inOrOutSelected(this->inOrOutType);
}

void EasingEquationView::inOrOutSelected(EasingEquationsInOrOut newInOrOutType)
{
    inOrOutType = newInOrOutType;
    if (newInOrOutType == EASING_EQUATION_IN)
    {
        easingEquation = easingEquationSelection.in;
    }
    else if (newInOrOutType == EASING_EQUATION_OUT)
    {
        easingEquation = easingEquationSelection.out;
    }
    else
    {
        easingEquation = easingEquationSelection.inOut;
    }
    updateEasingEquation();
}

void EasingEquationView::updateEasingEquation()
{
    dot.moveTo(150, 197);
    moveAnimationCounter = 0;
    graph.clear();
    graph.invalidate();
    drawGraph = true;
}

void EasingEquationView::handleTickEvent()
{
    tickCounter++;

    if (tickCounter % 2 == 0)
    {
        int moveAnimationDuration = NUMBER_OF_STEPS;

        if (moveAnimationCounter <= moveAnimationDuration)
        {
            graphDot.setVisible(true);

            int deltaDot = easingEquation(moveAnimationCounter, 0, 258, moveAnimationDuration);
            dot.moveTo(150 + deltaDot, dot.getY());

            int deltaGraphDot = easingEquation(moveAnimationCounter, 0, 100 * GRAPH_MULT, moveAnimationDuration);
            graphDot.moveTo(graph.getX() + 4 * moveAnimationCounter - (graphDot.getWidth() / 2) - 1, graph.getRect().bottom() - (deltaGraphDot / GRAPH_MULT) + GRAPH_Y_VALUE_OFFSET - (graphDot.getHeight() / 2) - 1);

            if (drawGraph)
            {
                graph.addValue(moveAnimationCounter, deltaGraphDot);
            }

            moveAnimationCounter++;
        }
        else if (moveAnimationCounter <= moveAnimationDuration + PAUSE_DURATION)
        {
            moveAnimationCounter++;
        }
        else
        {
            // End of animation
            moveAnimationCounter = 0;
            drawGraph = false;
        }
    }
}
