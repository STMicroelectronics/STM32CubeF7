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
