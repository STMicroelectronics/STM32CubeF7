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
  #include <gui/productpresenter_screen/ProductPresenterView.hpp>
#include <gui/productpresenter_screen/ProductPresenterPresenter.hpp>

ProductPresenterPresenter::ProductPresenterPresenter(ProductPresenterView& v)
    : view(v)
{
}

void ProductPresenterPresenter::activate()
{
}

void ProductPresenterPresenter::deactivate()
{
    // Reset language to be GBR as is used in
    // the rest of the demo
    Texts::setLanguage(GBR);
}

void ProductPresenterPresenter::mcuLoadUpdated(uint8_t mcuLoadValue)
{
    view.updateProcessorLoad(mcuLoadValue);
}
