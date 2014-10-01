/*
 * ZamNoise  Noise detection and removal plugin
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com> 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
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
    : UI()
{
    setSize(ZamNoiseArtwork::zamnoiseWidth,  ZamNoiseArtwork::zamnoiseHeight);

    // background
    fImgBackground = Image(ZamNoiseArtwork::zamnoiseData, ZamNoiseArtwork::zamnoiseWidth, ZamNoiseArtwork::zamnoiseHeight, GL_BGR);

    // toggle img
    Image toggleonImage(ZamNoiseArtwork::toggleonData, ZamNoiseArtwork::toggleonWidth, ZamNoiseArtwork::toggleonHeight);
    Image toggleoffImage(ZamNoiseArtwork::toggleoffData, ZamNoiseArtwork::toggleoffWidth, ZamNoiseArtwork::toggleoffHeight);

    // toggle
    fToggleNoise = new ImageToggle(this, toggleonImage, toggleoffImage);
    fToggleNoise->setAbsolutePos(30, 30);
    fToggleNoise->setCallback(this);
    fToggleNoise->setValue(0.f);

    d_programChanged(0);
}

ZamNoiseUI::~ZamNoiseUI()
{
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamNoiseUI::d_parameterChanged(uint32_t index, float value)
{
        switch (index)
        {
        case ZamNoisePlugin::paramNoiseToggle:
                fToggleNoise->setValue(value);
                break;
        }
}


void ZamNoiseUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    fToggleNoise->setValue(0.0f);
}

void ZamNoiseUI::imageToggleClicked(ImageToggle*, int)
{
        float toggle = fToggleNoise->getValue();
        fToggleNoise->setValue(toggle);
        d_setParameterValue(ZamNoisePlugin::paramNoiseToggle, toggle);
}


// -----------------------------------------------------------------------

void ZamNoiseUI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamNoiseUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
