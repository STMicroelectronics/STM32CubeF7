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
    FOLDERNAME_LENGTH = 40
};

#define RGB24BYTES(w, h) ((w)*(h)*3)

#endif
