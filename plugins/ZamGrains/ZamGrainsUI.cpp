/*
 * ZamGrains Granular Delay
 * Copyright (C) 2018  Damien Zammit <damien@zamaudio.com>
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

#include "ZamGrainsPlugin.hpp"
#include "ZamGrainsUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamGrainsUI::ZamGrainsUI()
    : UI()
{
    setSize(ZamGrainsArtwork::zamgrainsWidth, ZamGrainsArtwork::zamgrainsHeight);

    // background
    fImgBackground = Image(ZamGrainsArtwork::zamgrainsData, ZamGrainsArtwork::zamgrainsWidth, ZamGrainsArtwork::zamgrainsHeight, GL_BGR);

    // knob
    Image knobImage(ZamGrainsArtwork::knobData, ZamGrainsArtwork::knobWidth, ZamGrainsArtwork::knobHeight);

    // knob
    fKnobPlayspeed = new ZamKnob(this, knobImage);
    fKnobPlayspeed->setAbsolutePos(130, 36);
    fKnobPlayspeed->setId(ZamGrainsPlugin::paramPlayspeed);
    fKnobPlayspeed->setRange(0.1f, 20.0f);
    fKnobPlayspeed->setLabel(true);
    fKnobPlayspeed->setScrollStep(0.1f);
    fKnobPlayspeed->setUsingLogScale(true);
    fKnobPlayspeed->setDefault(1.0f);
    fKnobPlayspeed->setRotationAngle(240);
    fKnobPlayspeed->setCallback(this);

    fKnobGrainspeed = new ZamKnob(this, knobImage);
    fKnobGrainspeed->setAbsolutePos(27, 36);
    fKnobGrainspeed->setId(ZamGrainsPlugin::paramGrainspeed);
    fKnobGrainspeed->setRange(0.1f, 20.0f);
    fKnobGrainspeed->setLabel(true);
    fKnobGrainspeed->setScrollStep(0.1f);
    fKnobGrainspeed->setUsingLogScale(true);
    fKnobGrainspeed->setDefault(1.0f);
    fKnobGrainspeed->setRotationAngle(240);
    fKnobGrainspeed->setCallback(this);

    fKnobGrains = new ZamKnob(this, knobImage);
    fKnobGrains->setAbsolutePos(26.5, 113);
    fKnobGrains->setId(ZamGrainsPlugin::paramGrains);
    fKnobGrains->setRange(1.0f, 100.0f);
    fKnobGrains->setLabel(true);
    fKnobGrains->setScrollStep(1.f);
    fKnobGrains->setStep(1.f);
    fKnobGrains->setDefault(1.0f);
    fKnobGrains->setRotationAngle(240);
    fKnobGrains->setCallback(this);

    fKnobLooptime = new ZamKnob(this, knobImage);
    fKnobLooptime->setAbsolutePos(130, 113);
    fKnobLooptime->setId(ZamGrainsPlugin::paramDelaytime);
    fKnobLooptime->setRange(5.f, 1000.f);
    fKnobLooptime->setLabel(true);
    fKnobLooptime->setScrollStep(1.0f);
    fKnobLooptime->setDefault(1000.0f);
    fKnobLooptime->setRotationAngle(240);
    fKnobLooptime->setCallback(this);

    fKnobMaster = new ZamKnob(this, knobImage);
    fKnobMaster->setAbsolutePos(263, 113);
    fKnobMaster->setId(ZamGrainsPlugin::paramGain);
    fKnobMaster->setRange(-60.f, 0.f);
    fKnobMaster->setLabel(true);
    fKnobMaster->setScrollStep(1.0f);
    fKnobMaster->setDefault(0.f);
    fKnobMaster->setRotationAngle(240);
    fKnobMaster->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamGrainsUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamGrainsPlugin::paramPlayspeed:
        fKnobPlayspeed->setValue(value);
        break;
    case ZamGrainsPlugin::paramGrainspeed:
        fKnobGrainspeed->setValue(value);
        break;
    case ZamGrainsPlugin::paramGrains:
        fKnobGrains->setValue(value);
        break;
    case ZamGrainsPlugin::paramGain:
        fKnobMaster->setValue(value);
        break;
    case ZamGrainsPlugin::paramDelaytime:
        fKnobLooptime->setValue(value);
        break;
    }
}

void ZamGrainsUI::programLoaded(uint32_t index)
{
	switch(index) {
	case 0:
		fKnobPlayspeed->setValue(1.0f);
		fKnobGrainspeed->setValue(1.0f);
		fKnobGrains->setValue(1.0f);
		fKnobMaster->setValue(0.0f);
		fKnobLooptime->setValue(1000.0f);
		break;
	}
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamGrainsUI::imageKnobDragStarted(ZamKnob* knob)
{
    editParameter(knob->getId(), true);
}

void ZamGrainsUI::imageKnobDragFinished(ZamKnob* knob)
{
    editParameter(knob->getId(), false);
}

void ZamGrainsUI::imageKnobValueChanged(ZamKnob* knob, float value)
{
    setParameterValue(knob->getId(), value);
}

void ZamGrainsUI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamGrainsUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
