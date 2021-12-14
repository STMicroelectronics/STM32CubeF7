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
  


#include <gui/audio_player_screen/BMPFileLoader.hpp>
#include <touchgfx/Color.hpp>

#ifdef SIMULATOR
#include <stdio.h>
#define PRAGMA_CCRAM_LOCATION
#define ATTRIBUTE_CCRAM_LOCATION
#else
#include <string.h>
#include <ff.h>

#ifdef __GNUC__
#define PRAGMA_CCRAM_LOCATION
#define ATTRIBUTE_CCRAM_LOCATION __attribute__ ((section (".ccmram"))) __attribute__ ((aligned(4)))
#elif defined __ICCARM__
#define PRAGMA_CCRAM_LOCATION _Pragma("location=\"CCMRAM\"")
#define ATTRIBUTE_CCRAM_LOCATION
#elif defined(__ARMCC_VERSION)
#define PRAGMA_CCRAM_LOCATION
#define ATTRIBUTE_CCRAM_LOCATION __attribute__ ((section ("CCMRAM"))) __attribute__ ((aligned(4)))
#endif

#endif

int BMPFileLoader::readFile(FileHdl hdl, uint8_t* const buffer, uint32_t length)
{
  if (!hdl)
  {
    return 1;
  }
#ifdef SIMULATOR
    uint32_t r = fread(buffer, 1, length, (FILE*)hdl);
#else
    uint32_t r = 0;
    f_read((FIL*)hdl, buffer, length, (unsigned int*)&r);

#endif
    if (r != length)
    {
        return 1;
    }
    return 0;
}

void BMPFileLoader::seekFile(FileHdl hdl, uint32_t offset)
{
  if (!hdl)
  {
    return;
  }
#ifdef SIMULATOR
    fseek((FILE*)hdl, offset, SEEK_SET);
#else
    f_lseek((FIL*)hdl, offset);
#endif
}

void BMPFileLoader::getBMP24Dimensions(FileHdl fileHandle, uint16_t& width, uint16_t& height)
{
    if (!fileHandle)
    {
      width = 0;
      height = 0;
      return;
    }
    uint8_t data[50];
    seekFile(fileHandle, 0);
    readFile(fileHandle, data, 26); //read first part of header.

    width = data[18] | (data[19] << 8) | (data[20] << 16) | (data[21] << 24);
    height = data[22] | (data[23] << 8) | (data[24] << 16) | (data[25] << 24);
}

PRAGMA_CCRAM_LOCATION static uint8_t data[256 * 3] ATTRIBUTE_CCRAM_LOCATION;

void BMPFileLoader::readBMP24File(Bitmap bitmap, FileHdl fileHandle)
{
    if (!fileHandle)
    {
      return;
    }
    seekFile(fileHandle, 0);
    readFile(fileHandle, data, 26); //read first part of header.

    const uint32_t offset = data[10] | (data[11] << 8) | (data[12] << 16) | (data[12] << 24);
    const uint32_t width = data[18] | (data[19] << 8) | (data[20] << 16) | (data[21] << 24);
    const uint32_t height = data[22] | (data[23] << 8) | (data[24] << 16) | (data[25] << 24);

    //could check offset here
    readFile(fileHandle, data, offset - 26); //read rest of header.

    const uint32_t buffer_width = bitmap.getWidth();
    const uint32_t rowpadding = (4 - ((width * 3) % 4)) % 4;
    uint8_t* const  buffer8 = Bitmap::dynamicBitmapGetAddress(bitmap.getId());

    for (uint32_t y = 0; y < height; y++)
    {
        const uint32_t inx = 3 * (0 + (height - y - 1) * buffer_width);
        readFile(fileHandle, data, (width - 0) * 3 + rowpadding); //rest of line
        memcpy(&buffer8[inx], data, 768);
    }
}
