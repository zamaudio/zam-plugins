/*
 * ZamNoise  Noise detection and removal plugin
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

#include "ZamNoisePlugin.hpp"
#include "ZamNoiseUI.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamNoiseUI::ZamNoiseUI()
    : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true)
{
    // background
    fImgBackground = Image(ZamNoiseArtwork::zamnoiseData, ZamNoiseArtwork::zamnoiseWidth, ZamNoiseArtwork::zamnoiseHeight, kImageFormatBGR);

    // toggle img
    Image toggleonImage(ZamNoiseArtwork::toggleonData, ZamNoiseArtwork::toggleonWidth, ZamNoiseArtwork::toggleonHeight);
    Image toggleoffImage(ZamNoiseArtwork::toggleoffData, ZamNoiseArtwork::toggleoffWidth, ZamNoiseArtwork::toggleoffHeight);

    // toggle
    fToggleNoise = new ImageButton(this, toggleonImage, toggleoffImage);
    fToggleNoise->setAbsolutePos(30, 30);
    fToggleNoise->setCallback(this);
    fToggleNoise->setCheckable(true);
    fToggleNoise->setChecked(false, false);

    programLoaded(0);
}

ZamNoiseUI::~ZamNoiseUI()
{
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamNoiseUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamNoisePlugin::paramNoiseToggle:
        fToggleNoise->setChecked(value > 0.5f, false);
        break;
    }
}


void ZamNoiseUI::programLoaded(uint32_t index)
{
    if (index != 0)
        return;

    fToggleNoise->setChecked(false, false);
}

void ZamNoiseUI::imageButtonClicked(ImageButton*, int)
{
    setParameterValue(ZamNoisePlugin::paramNoiseToggle, fToggleNoise->isChecked() ? 1.f : 0.f);
}


// -----------------------------------------------------------------------

void ZamNoiseUI::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());
    fImgBackground.draw(context);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamNoiseUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
