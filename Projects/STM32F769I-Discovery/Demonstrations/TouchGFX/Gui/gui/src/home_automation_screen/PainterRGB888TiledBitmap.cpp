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
  

#include <gui/home_automation_screen/PainterRGB888TiledBitmap.hpp>

bool PainterRGB888TiledBitmap::renderInit()
{
    bitmapARGB8888Pointer = 0;
    bitmapRGB888Pointer = 0;

    int bitmapX = (currentX % bitmapRectToFrameBuffer.width);
    int bitmapY = (currentY % bitmapRectToFrameBuffer.height);

    if (bitmap.getFormat() == Bitmap::ARGB8888)
    {
        bitmapARGB8888Pointer = (const uint32_t*)bitmap.getData();
        if (!bitmapARGB8888Pointer)
        {
            return false;
        }
        bitmapARGB8888Pointer += bitmapX + bitmapY * bitmapRectToFrameBuffer.width;
    }
    else
    {
        bitmapRGB888Pointer = bitmap.getData();
        if (!bitmapRGB888Pointer)
        {
            return false;
        }
        bitmapRGB888Pointer += (bitmapX + bitmapY * bitmapRectToFrameBuffer.width) * 3;
    }

    return true;
}

bool PainterRGB888TiledBitmap::renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha)
{
    if (bitmap.getFormat() == Bitmap::ARGB8888)
    {
        uint32_t argb8888 = *bitmapARGB8888Pointer++;
        alpha = (argb8888 >> 24) & 0xFF;
        red = (argb8888 >> 16) & 0xFF;
        green = (argb8888 >> 8) & 0xFF;
        blue = (argb8888) & 0xFF;
        if ((currentX + 1) % bitmapRectToFrameBuffer.width == 0)
        {
            bitmapARGB8888Pointer -= bitmapRectToFrameBuffer.width;
        }
    }
    else
    {
        blue = *bitmapRGB888Pointer++;
        green = *bitmapRGB888Pointer++;
        red = *bitmapRGB888Pointer++;
        alpha = 255; // No alpha per pixel in the image, it is solid
        if ((currentX + 1) % bitmapRectToFrameBuffer.width == 0)
        {
            bitmapRGB888Pointer -= bitmapRectToFrameBuffer.width * 3;
        }
    }
    if (painterAlpha < 255)
    {
        // Apply given alpha from setAlpha()
        alpha = (((uint16_t)alpha) * ((uint16_t)painterAlpha)) / 255;
    }
    return true;
}
