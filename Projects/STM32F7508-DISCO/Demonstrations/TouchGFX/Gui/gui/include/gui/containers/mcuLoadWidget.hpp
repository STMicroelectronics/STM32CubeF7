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
  #ifndef MCULOADWIDGET_HPP
#define MCULOADWIDGET_HPP

#include <gui_generated/containers/McuLoadWidgetBase.hpp>
#include <touchgfx/Color.hpp>

class McuLoadWidget : public McuLoadWidgetBase
{
public:
    McuLoadWidget();
    virtual ~McuLoadWidget() {}

    void updateProcessorLoad(uint8_t mcuLoad);

    void setTextColor(colortype color);
protected:
};

#endif // MCULOADWIDGET_HPP
