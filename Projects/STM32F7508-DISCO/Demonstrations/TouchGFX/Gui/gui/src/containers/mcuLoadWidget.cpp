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
  #include <gui/containers/mcuLoadWidget.hpp>

McuLoadWidget::McuLoadWidget()
{
    Unicode::snprintf(mcuLoadValueTxtBuffer, 5, "%d", 0);
}

void McuLoadWidget::updateProcessorLoad(uint8_t mcuLoad)
{
    Unicode::snprintf(mcuLoadValueTxtBuffer, MCULOADVALUETXT_SIZE, "%d", mcuLoad);
    mcuLoadValueTxt.invalidate();
}

void McuLoadWidget::setTextColor(colortype color)
{
    mcuLoadTxt.setColor(color);
    mcuLoadValueTxt.setColor(color);
    invalidate();
}
