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
  #ifndef EASINGEQUATION_VIEW_HPP
#define EASINGEQUATION_VIEW_HPP

#include <gui_generated/easingequation_screen/EasingEquationViewBase.hpp>
#include <gui/easingequation_screen/EasingEquationPresenter.hpp>
#include <gui/easingequation_screen/EasingEquationSelector.hpp>
#include <gui/easingequation_screen/EasingEquationInOutSelectorGroup.hpp>
#include <gui/easingequation_screen/EasingEquationGraph.hpp>

class EasingEquationView : public EasingEquationViewBase
{
public:
    EasingEquationView();
    virtual ~EasingEquationView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    void updateProcessorLoad(uint8_t mcuLoadValue);

protected:
    static const int GRAPH_MULT = 6;
    static const int NUMBER_OF_STEPS = 50;
    static const int GRAPH_Y_VALUE_OFFSET = -30;
    static const int PAUSE_DURATION = 20;

    int tickCounter;
    int moveAnimationCounter;

    EasingEquationSelection easingEquationSelection;
    EasingEquationSelector selector;
    EasingEquationInOutSelectorGroup inOrOut;
    EasingEquationsInOrOut inOrOutType;
    EasingEquation easingEquation;

    EasingEquationGraph graph;
    Image graphDot;
    bool drawGraph;

    Callback<EasingEquationView, const EasingEquationSelection&> onEasingEquationSelected;
    void easingEquationSelected(const EasingEquationSelection& selection);

    Callback<EasingEquationView, EasingEquationsInOrOut> onInOrOutSelected;
    void inOrOutSelected(EasingEquationsInOrOut newInOrOutType);

    void updateEasingEquation();
};

#endif
