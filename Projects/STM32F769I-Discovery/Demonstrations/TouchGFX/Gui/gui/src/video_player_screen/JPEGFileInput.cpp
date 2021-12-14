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

