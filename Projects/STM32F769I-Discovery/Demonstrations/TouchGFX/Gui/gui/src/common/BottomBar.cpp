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
  

#include <gui/common/BottomBar.hpp>
#include <texts/TextKeysAndLanguages.hpp>

BottomBar::BottomBar()
    : buttonCallback(this, &BottomBar::buttonClicked),
      actionCallback(0)
{

    background.setBitmap(Bitmap(BITMAP_COMMON_PLAYER_BAR_ID));

    //size matches movie player bar bitmap
    setWidth(background.getWidth());
    setHeight(background.getHeight());

    playlistButton.setBitmaps(Bitmap(BITMAP_GRID_ID), Bitmap(BITMAP_GRID_ID));
    folderButton.setBitmaps(Bitmap(BITMAP_FOLDER_ID), Bitmap(BITMAP_FOLDER_ID));

    playlistButton.setAction(buttonCallback);
    folderButton.setAction(buttonCallback);

    folderButton.setXY(getWidth() - (20 + 64), 0);
    playlistButton.setXY(getWidth() - (20 + 64 * 2), 0);

    add(background);
    add(playlistButton);
    add(folderButton);
}

void BottomBar::buttonClicked(const AbstractButton& btn)
{
    if (&btn == &playlistButton)
    {
        if (actionCallback) //call external callback
        {
            actionCallback->execute(PLAYLIST);
        }
    }
    else if (&btn == &folderButton)
    {
        if (actionCallback) //call external callback
        {
            actionCallback->execute(FOLDER);
        }
    }
}

void BottomBar::setActionCallback(GenericCallback<Action>& callback)
{
    actionCallback = &callback;
}
