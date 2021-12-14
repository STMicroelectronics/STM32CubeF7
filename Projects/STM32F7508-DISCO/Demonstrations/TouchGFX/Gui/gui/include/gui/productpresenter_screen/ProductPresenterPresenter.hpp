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
  #ifndef PRODUCTPRESENTER_PRESENTER_HPP
#define PRODUCTPRESENTER_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ProductPresenterView;

class ProductPresenterPresenter : public Presenter, public ModelListener
{
public:
    ProductPresenterPresenter(ProductPresenterView& v);
    virtual ~ProductPresenterPresenter() {};

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    void mcuLoadUpdated(uint8_t mcuLoadValue);

private:
    ProductPresenterPresenter();

    ProductPresenterView& view;
};


#endif // PRODUCTPRESENTER_PRESENTER_HPP
