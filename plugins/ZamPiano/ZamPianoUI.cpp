/*
 * ZamPiano polyphonic synthesiser
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

#include "ZamPianoPlugin.hpp"
#include "ZamPianoUI.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamPianoUI::ZamPianoUI()
    : UI()
{
    setSize(ZamPianoArtwork::zampianoWidth, ZamPianoArtwork::zampianoHeight);

    // background
    fImgBackground = Image(ZamPianoArtwork::zampianoData, ZamPianoArtwork::zampianoWidth, ZamPianoArtwork::zampianoHeight, GL_BGR);

    // knob
    Image knobImage(ZamPianoArtwork::knobData, ZamPianoArtwork::knobWidth, ZamPianoArtwork::knobHeight);

    // knob

    fKnobGain = new ImageKnob(this, knobImage);
    fKnobGain->setAbsolutePos(60, 80);
    fKnobGain->setRange(-30.f, 30.0f);
    fKnobGain->setDefault(0.0f);
    fKnobGain->setRotationAngle(240);
    fKnobGain->setCallback(this);

    // set default values
    d_programChanged(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamPianoUI::d_parameterChanged(uint32_t index, float value)
{
	switch (index)
	{
	case ZamPianoPlugin::paramGain:
		fKnobGain->setValue(value);
		break;
	}
}

void ZamPianoUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    fKnobGain->setValue(0.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamPianoUI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobGain)
        d_editParameter(ZamPianoPlugin::paramGain, true);
}

void ZamPianoUI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobGain)
        d_editParameter(ZamPianoPlugin::paramGain, false);
}

void ZamPianoUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobGain)
        d_setParameterValue(ZamPianoPlugin::paramGain, value);
}

void ZamPianoUI::onDisplay()
{
	fImgBackground.draw();
}
// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamPianoUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
