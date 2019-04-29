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
