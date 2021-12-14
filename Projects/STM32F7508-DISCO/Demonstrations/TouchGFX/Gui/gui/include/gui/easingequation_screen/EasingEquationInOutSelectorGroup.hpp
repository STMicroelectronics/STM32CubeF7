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
  #ifndef EASINGEQUATIONINOUTSELECTORGROUP_HPP
#define EASINGEQUATIONINOUTSELECTORGROUP_HPP

#include <touchgfx/widgets/RadioButtonGroup.hpp>
#include <gui/easingequation_screen/EasingEquationButton.hpp>
#include <touchgfx/containers/Container.hpp>
#include <gui/easingequation_screen/EasingEquationsEnum.hpp>
#include <touchgfx/widgets/AbstractButton.hpp>

using namespace touchgfx;

/* Class that represents a group of radiobuttons that lets the
   user select between IN, OUT or IN/OUT easing equations
 */
class EasingEquationInOutSelectorGroup : public Container
{
public:
    EasingEquationInOutSelectorGroup();
    void setInOrOutSelectedCallback(GenericCallback<EasingEquationsInOrOut>& callback);
    void selectFirst();
private:
    EasingEquationButton in;
    EasingEquationButton out;
    EasingEquationButton inOut;
    RadioButtonGroup<3> buttons;

    Callback<EasingEquationInOutSelectorGroup, const AbstractButton&> onSelected;
    void selected(const AbstractButton& btn);
    void setButtonColors();
    touchgfx::GenericCallback<EasingEquationsInOrOut>* inOrOutSelectedCallback;
};

#endif /* EASINGEQUATIONINOROUTORINOUT_HPP */
