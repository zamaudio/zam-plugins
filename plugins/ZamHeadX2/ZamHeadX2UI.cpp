/*
 * ZamHeadX2 stereo HRTF tool
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
 */

#include "ZamHeadX2Plugin.hpp"
#include "ZamHeadX2UI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamHeadX2UI::ZamHeadX2UI()
    : UI()
{
    setSize(ZamHeadX2Artwork::zamheadx2Width, ZamHeadX2Artwork::zamheadx2Height);

    // background
    fImgBackground = Image(ZamHeadX2Artwork::zamheadx2Data, ZamHeadX2Artwork::zamheadx2Width, ZamHeadX2Artwork::zamheadx2Height, GL_BGR);

    // knob
    Image knobImage(ZamHeadX2Artwork::knobData, ZamHeadX2Artwork::knobWidth, ZamHeadX2Artwork::knobHeight);

    // knob
    fKnobAzimuth = new ZamKnob(this, knobImage);
    fKnobAzimuth->setAbsolutePos(229, 33);
    fKnobAzimuth->setId(ZamHeadX2Plugin::paramAzimuth);
    fKnobAzimuth->setRange(-120.f, 120.0f);
    fKnobAzimuth->setLabel(true);
    fKnobAzimuth->setScrollStep(10.0f);
    fKnobAzimuth->setDefault(0.0f);
    fKnobAzimuth->setRotationAngle(240);
    fKnobAzimuth->setCallback(this);

    fKnobElevation = new ZamKnob(this, knobImage);
    fKnobElevation->setAbsolutePos(62, 78);
    fKnobElevation->setId(ZamHeadX2Plugin::paramElevation);
    fKnobElevation->setRange(-30.0f, 90.0f);
    fKnobElevation->setLabel(true);
    fKnobElevation->setScrollStep(10.0f);
    fKnobElevation->setDefault(0.0f);
    fKnobElevation->setRotationAngle(120);
    fKnobElevation->setCallback(this);

    fKnobWidth = new ZamKnob(this, knobImage);
    fKnobWidth->setAbsolutePos(45, 170);
    fKnobWidth->setId(ZamHeadX2Plugin::paramWidth);
    fKnobWidth->setRange(0.0f, 2.5f);
    fKnobWidth->setLabel(true);
    fKnobWidth->setScrollStep(0.1f);
    fKnobWidth->setDefault(1.0f);
    fKnobWidth->setRotationAngle(300);
    fKnobWidth->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamHeadX2UI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamHeadX2Plugin::paramAzimuth:
        fKnobAzimuth->setValue(value);
        break;
    case ZamHeadX2Plugin::paramElevation:
        fKnobElevation->setValue(value);
        break;
    case ZamHeadX2Plugin::paramWidth:
        fKnobWidth->setValue(value);
        break;
    }
}

void ZamHeadX2UI::programLoaded(uint32_t index)
{
	switch(index) {
	case 0:
		fKnobAzimuth->setValue(0.0f);
		fKnobElevation->setValue(0.0f);
		fKnobWidth->setValue(1.0f);
		break;
	}
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamHeadX2UI::imageKnobDragStarted(ZamKnob* knob)
{
    editParameter(knob->getId(), true);
}

void ZamHeadX2UI::imageKnobDragFinished(ZamKnob* knob)
{
    editParameter(knob->getId(), false);
}

void ZamHeadX2UI::imageKnobValueChanged(ZamKnob* knob, float value)
{
    setParameterValue(knob->getId(), value);
}

void ZamHeadX2UI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamHeadX2UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
