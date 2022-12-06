/*
 * ZamVerb
 * Copyright (C) 2017 Damien Zammit <damien@zamaudio.com>
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
 */

#include "ZamVerbPlugin.hpp"
#include "ZamVerbUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamVerbUI::ZamVerbUI()
    : UI()
{
    setSize(ZamVerbArtwork::zamverbWidth, ZamVerbArtwork::zamverbHeight);

    // background
    fImgBackground = Image(ZamVerbArtwork::zamverbData, ZamVerbArtwork::zamverbWidth, ZamVerbArtwork::zamverbHeight, kImageFormatBGR);

    // knob
    Image knobImage(ZamVerbArtwork::knobData, ZamVerbArtwork::knobWidth, ZamVerbArtwork::knobHeight);

    // notch slider
    Image notchImage(ZamVerbArtwork::notchData, ZamVerbArtwork::notchWidth, ZamVerbArtwork::notchHeight);

    // knobs
    fKnobMaster = new ZamKnob(this, knobImage);
    fKnobMaster->setAbsolutePos(27, 85);
    fKnobMaster->setRange(-30.f, 30.0f);
    fKnobMaster->setLabel(true);
    fKnobMaster->setScrollStep(1.0f);
    fKnobMaster->setDefault(0.0f);
    fKnobMaster->setRotationAngle(240);
    fKnobMaster->setCallback(this);

    fKnobWetdry = new ZamKnob(this, knobImage);
    fKnobWetdry->setAbsolutePos(27, 23);
    fKnobWetdry->setRange(0.f, 100.f);
    fKnobWetdry->setLabel(true);
    fKnobWetdry->setScrollStep(1.0f);
    fKnobWetdry->setDefault(50.0f);
    fKnobWetdry->setRotationAngle(240);
    fKnobWetdry->setCallback(this);

    Point<int> notchPosStart(225,27);
    Point<int> notchPosEnd(225,27+70);

    fSliderNotch = new ImageSlider(this, notchImage);
    fSliderNotch->setStartPos(notchPosStart);
    fSliderNotch->setEndPos(notchPosEnd);
    fSliderNotch->setRange(0.f, 6.f);
    fSliderNotch->setStep(1.f);
    fSliderNotch->setDefault(0.f);
    fSliderNotch->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamVerbUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamVerbPlugin::paramRoom:
        fSliderNotch->setValue(value);
        break;
    case ZamVerbPlugin::paramMaster:
        fKnobMaster->setValue(value);
        break;
    case ZamVerbPlugin::paramWetdry:
        fKnobWetdry->setValue(value);
        break;
    }
}

void ZamVerbUI::programLoaded(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fKnobMaster->setValue(0.0f);
    fKnobWetdry->setValue(50.f);
    fSliderNotch->setValue(0.0f);
}

void ZamVerbUI::stateChanged(const char*, const char*)
{
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamVerbUI::imageKnobDragStarted(ZamKnob* knob)
{
    if (knob == fKnobMaster)
        editParameter(ZamVerbPlugin::paramMaster, true);
    else if (knob == fKnobWetdry)
        editParameter(ZamVerbPlugin::paramWetdry, true);
}

void ZamVerbUI::imageKnobDragFinished(ZamKnob* knob)
{
    if (knob == fKnobMaster)
        editParameter(ZamVerbPlugin::paramMaster, false);
    else if (knob == fKnobWetdry)
        editParameter(ZamVerbPlugin::paramWetdry, false);
}

void ZamVerbUI::imageKnobValueChanged(ZamKnob* knob, float value)
{
    if (knob == fKnobMaster)
        setParameterValue(ZamVerbPlugin::paramMaster, value);
    else if (knob == fKnobWetdry)
        setParameterValue(ZamVerbPlugin::paramWetdry, value);
}

void ZamVerbUI::imageSliderDragStarted(ImageSlider*)
{
    editParameter(ZamVerbPlugin::paramRoom, true);
}

void ZamVerbUI::imageSliderDragFinished(ImageSlider*)
{
    editParameter(ZamVerbPlugin::paramRoom, false);
}

void ZamVerbUI::imageSliderValueChanged(ImageSlider*, float value)
{
    setParameterValue(ZamVerbPlugin::paramRoom, value);
}

void ZamVerbUI::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());
    fImgBackground.draw(context);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamVerbUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
