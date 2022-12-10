/*
 * ZamAutoSat Automatic Saturator
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com> 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#include "ZamAutoSatUI.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamAutoSatUI::ZamAutoSatUI()
    : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true)
{
    // background
    fImgBackground = Image(ZamAutoSatArtwork::zamautosatData, ZamAutoSatArtwork::zamautosatWidth, ZamAutoSatArtwork::zamautosatHeight, kImageFormatBGR);
}

ZamAutoSatUI::~ZamAutoSatUI()
{
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamAutoSatUI::parameterChanged(uint32_t, float)
{
}

// -----------------------------------------------------------------------

void ZamAutoSatUI::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());
    fImgBackground.draw(context);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamAutoSatUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
