/*
 * ZamChild670 stereo tube compressor
 * Copyright (C) 2015  Damien Zammit <damien@zamaudio.com>
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

#include "ZamChild670Plugin.hpp"
#include "ZamChild670UI.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamChild670UI::ZamChild670UI()
    : UI()
{
    setSize(ZamChild670Artwork::zamchild670Width, ZamChild670Artwork::zamchild670Height);

    // background
    fImgBackground = Image(ZamChild670Artwork::zamchild670Data, ZamChild670Artwork::zamchild670Width, ZamChild670Artwork::zamchild670Height, GL_BGR);

    // knob
    Image knobImage(ZamChild670Artwork::knobData, ZamChild670Artwork::knobWidth, ZamChild670Artwork::knobHeight);

    // knob
    fKnobIngain = new ImageKnob(this, knobImage);
    fKnobIngain->setAbsolutePos(24, 45);
    fKnobIngain->setId(ZamChild670Plugin::paramInlevel);
    fKnobIngain->setRange(-20.f, 0.0f);
    fKnobIngain->setDefault(-12.0f);
    fKnobIngain->setRotationAngle(240);
    fKnobIngain->setCallback(this);

    fKnobAC = new ImageKnob(this, knobImage);
    fKnobAC->setAbsolutePos(103, 45);
    fKnobAC->setId(ZamChild670Plugin::paramAC);
    fKnobAC->setRange(0.0f, 10.0f);
    fKnobAC->setDefault(0.2f);
    fKnobAC->setRotationAngle(240);
    fKnobAC->setCallback(this);

    fKnobDC = new ImageKnob(this, knobImage);
    fKnobDC->setAbsolutePos(181, 45);
    fKnobDC->setId(ZamChild670Plugin::paramDC);
    fKnobDC->setRange(0.0f, 1.0f);
    fKnobDC->setDefault(0.0f);
    fKnobDC->setRotationAngle(240);
    fKnobDC->setCallback(this);

    fKnobOutgain = new ImageKnob(this, knobImage);
    fKnobOutgain->setAbsolutePos(262, 45);
    fKnobOutgain->setId(ZamChild670Plugin::paramOutlevel);
    fKnobOutgain->setRange(-20.0f, 20.0f);
    fKnobOutgain->setDefault(0.0f);
    fKnobOutgain->setRotationAngle(240);
    fKnobOutgain->setCallback(this);

    // set default values
    d_programChanged(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamChild670UI::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamChild670Plugin::paramInlevel:
        fKnobIngain->setValue(value);
        break;
    case ZamChild670Plugin::paramAC:
        fKnobAC->setValue(value);
        break;
    case ZamChild670Plugin::paramDC:
        fKnobDC->setValue(value);
        break;
    case ZamChild670Plugin::paramOutlevel:
        fKnobOutgain->setValue(value);
        break;
    }
}

void ZamChild670UI::d_programChanged(uint32_t)
{
    // Default values
    fKnobIngain->setValue(-12.0f);
    fKnobAC->setValue(0.2f);
    fKnobDC->setValue(0.0f);
    fKnobOutgain->setValue(0.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamChild670UI::imageKnobDragStarted(ImageKnob* knob)
{
    d_editParameter(knob->getId(), true);
}

void ZamChild670UI::imageKnobDragFinished(ImageKnob* knob)
{
    d_editParameter(knob->getId(), false);
}

void ZamChild670UI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    d_setParameterValue(knob->getId(), value);
}

void ZamChild670UI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamChild670UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
