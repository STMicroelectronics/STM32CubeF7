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
  

#ifndef PAINTERRGB888TILEDBITMAP_HPP
#define PAINTERRGB888TILEDBITMAP_HPP

#include <stdint.h>
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/transforms/DisplayTransformation.hpp>

using namespace touchgfx;

/**
 * @class PainterRGB888TiledBitmap PainterRGB888TiledBitmap.hpp touchgfx/widgets/canvas/PainterRGB888TiledBitmap.hpp
 *
 * @brief A Painter that will paint using a bitmap.
 *
 *        PainterRGB888TiledBitmap will take the color for a given point in the shape from a bitmap.
 *        Please be aware, the the bitmap is used by the CWR (not Shape), so any rotation you
 *        might specify for a Canvas Widget (e.g. Shape) is not applied to the bitmap as CWR is
 *        not aware of this rotation.
 *
 * @see AbstractPainter
 */
class PainterRGB888TiledBitmap : public PainterRGB888Bitmap
{
public:
protected:
    virtual bool renderInit();
    virtual bool renderNext(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha);
}; // class PainterRGB888TiledBitmap

#endif // PAINTERRGB888TILEDBITMAP_HPP
