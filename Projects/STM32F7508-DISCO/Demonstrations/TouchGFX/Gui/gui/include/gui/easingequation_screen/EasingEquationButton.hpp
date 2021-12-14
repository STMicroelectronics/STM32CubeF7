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
  #ifndef EASING_EQUATION_BUTTON
#define EASING_EQUATION_BUTTON

#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/widgets/RadioButton.hpp>
#include <gui/easingequation_screen/EasingEquationsEnum.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/TextArea.hpp>

using namespace touchgfx;

/*
 * Class that represents a (radio) button with a text label and an optional text shadow.
 * The button has an associated easing equation that can be returned on request.
 */
class EasingEquationButton : public Container
{
public:
    EasingEquationButton();

    void setBitmaps();
    void setTextColor(colortype textColor, colortype shadowColor);
    void setTextShadowVisible(bool textShadowVisible);

    void setup(EasingEquationSelection newEasingEquation, const Bitmap& bmpUnselected, const Bitmap& bmpUnselectedPressed, const Bitmap& bmpSelected, const Bitmap& bmpSelectedPressed);
    const EasingEquationSelection& getEasingEquation() const;

    RadioButton& getRadioButton();

private:
    RadioButton radioButton;

    EasingEquationSelection easingEquation;

    TextArea buttonText;
    TextArea buttonTextShadow;
};

#endif
