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
  

#ifndef DATE_SELECTOR_CONTAINER
#define DATE_SELECTOR_CONTAINER

#include <touchgfx/containers/Container.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/settings_screen/DatePicker.hpp>

using namespace touchgfx;

class DateSelectorContainer : public Container
{
public:			
	DateSelectorContainer();
	virtual ~DateSelectorContainer();

    void setDate(uint16_t year, uint8_t month, uint8_t day);

    uint16_t getYear();
    uint8_t getMonth();
    uint8_t getDay();

    int getYearOffset() { return DatePicker::START_YEAR; }
private:
    DatePicker datePicker;

};

#endif /* DATE_SELECTOR_CONTAINER */
