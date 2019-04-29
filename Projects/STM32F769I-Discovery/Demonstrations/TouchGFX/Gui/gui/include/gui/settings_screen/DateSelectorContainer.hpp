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
