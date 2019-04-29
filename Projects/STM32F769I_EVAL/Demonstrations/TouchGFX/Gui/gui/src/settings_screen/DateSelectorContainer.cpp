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
  

#include <gui/settings_screen/DateSelectorContainer.hpp>


DateSelectorContainer::DateSelectorContainer()
{
    datePicker.setXY(100, 56);
    add(datePicker);
}

DateSelectorContainer::~DateSelectorContainer()
{
}

void DateSelectorContainer::setDate(uint16_t year, uint8_t month, uint8_t day)
{
    datePicker.setDay(day, 16, EasingEquations::backEaseOut);
    datePicker.setMonth(month, 16, EasingEquations::backEaseOut);
    datePicker.setYear(year, 16, EasingEquations::backEaseOut);
}

uint16_t DateSelectorContainer::getYear()
{
    return datePicker.getYear();
}

uint8_t DateSelectorContainer::getMonth()
{
    return datePicker.getMonth();
}

uint8_t DateSelectorContainer::getDay()
{
    return datePicker.getDay();
}
