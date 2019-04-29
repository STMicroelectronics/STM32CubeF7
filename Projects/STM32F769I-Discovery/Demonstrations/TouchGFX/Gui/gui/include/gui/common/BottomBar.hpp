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
  

#ifndef VIDEO_PLAYER_BOTTOMBAR_HPP
#define VIDEO_PLAYER_BOTTOMBAR_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>

#include <gui/common/CommonDefinitions.hpp>

#include <BitmapDatabase.hpp>

using namespace touchgfx;

class BottomBar : public Container
{
public:
    BottomBar();
    virtual ~BottomBar() {}
    void setActionCallback(GenericCallback<Action>& callback);
protected:
    Callback<BottomBar, const AbstractButton&> buttonCallback;
    virtual void buttonClicked(const AbstractButton& btn);

    //callback for buttons
    GenericCallback<Action>* actionCallback;

    Image background;
    Button playlistButton;
    Button folderButton;
};

#endif
