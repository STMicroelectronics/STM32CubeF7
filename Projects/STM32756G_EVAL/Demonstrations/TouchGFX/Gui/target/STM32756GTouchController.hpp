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
  


#ifndef STM32756GTOUCHCONTROLLER_HPP
#define STM32756GTOUCHCONTROLLER_HPP

#include <platform/driver/touch/TouchController.hpp>

namespace touchgfx
{
class STM32756GTouchController: public TouchController
{
public:
    STM32756GTouchController() : isInitialized(false) {}
    virtual void init();
    virtual bool sampleTouch(int32_t& x, int32_t& y);
protected:
    bool isInitialized;
};

}

#endif // STM32756GTOUCHCONTROLLER_HPP
