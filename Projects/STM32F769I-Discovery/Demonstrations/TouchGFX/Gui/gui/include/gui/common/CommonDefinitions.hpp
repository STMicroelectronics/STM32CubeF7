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
  

#ifndef COMMON_DEFINITIONS_HPP
#define COMMON_DEFINITIONS_HPP

enum Action
{
    HW_ON,
    HW_OFF,
    REPEAT_ON,
    REPEAT_OFF,
    PLAY,
    PAUSE,
    REPEAT,
    PLAYLIST,
    PLAYLIST_BACK,
    FOLDER,
    EQUALIZER
};

enum PlayerState
{
    STOPPED,
    PAUSED,
    PLAYING
};

enum
{
    PLAYLIST_LENGTH   = 20,
    DIRLIST_LENGTH    = 20,
    FILENAME_LENGTH   = 20,
    FOLDERNAME_LENGTH = 50
};

#define RGB24BYTES(w, h) ((w)*(h)*3)

#endif
