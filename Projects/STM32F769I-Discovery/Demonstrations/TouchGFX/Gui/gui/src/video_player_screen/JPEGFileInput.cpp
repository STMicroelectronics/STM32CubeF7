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
  

#include <gui/video_player_screen/FileInput.hpp>
#include "jinclude.h"

#include <stdint.h>

#ifdef SIMULATOR
#include <stdio.h>
#define FIL FILE
#endif

//link LibJPEG file read operation to touchgfx::fileinput function.
extern "C"
{
    size_t jpeg_read_file (FIL *file, uint8_t *buf, uint32_t sizeofbuf)
    {
        return touchgfx::fileinput::readFile(file, buf, sizeofbuf);
    }
}

