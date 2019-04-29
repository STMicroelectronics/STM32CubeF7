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
  

#include <gui/common/TiledImage.hpp>

void TiledImage::setBitmap(const Bitmap& bmp)
{
    bitmap = bmp;
    setWidth(bitmap.getWidth());
    setHeight(bitmap.getHeight());
}

void TiledImage::draw(const Rect& invalidatedArea) const
{
    Rect meAbs;
    translateRectToAbsolute(meAbs);

    uint16_t bitmapWidth = bitmap.getWidth();
    uint16_t bitmapHeight = bitmap.getHeight();

    const int16_t minX = (invalidatedArea.x / bitmapWidth) * bitmapWidth;
    const int16_t maxX = ((invalidatedArea.right() - 1) / bitmapWidth) * bitmapWidth;
    const int16_t minY = (invalidatedArea.y / bitmapHeight) * bitmapHeight;
    const int16_t maxY = ((invalidatedArea.bottom() - 1) / bitmapHeight) * bitmapHeight;
    for (int16_t x = minX; x <= maxX; x += bitmapWidth)
    {
        for (int16_t y = minY; y <= maxY; y += bitmapHeight)
        {
            Rect dirty = Rect(x, y, bitmapWidth, bitmapHeight) & invalidatedArea;
            dirty.x -= x;
            dirty.y -= y;
            HAL::lcd().drawPartialBitmap(bitmap, meAbs.x + x, meAbs.y + y, dirty, alpha);
        }
    }
}

Rect TiledImage::getSolidRect() const
{
    if (alpha < 255)
    {
        return Rect(0, 0, 0, 0);
    }
    Rect solidRect = bitmap.getSolidRect();
    if (solidRect.width == bitmap.getWidth())
    {
        solidRect.width = getWidth();
    }
    if (solidRect.height == bitmap.getHeight())
    {
        solidRect.height = getHeight();
    }
    return rect;
}
