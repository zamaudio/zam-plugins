/*
 * ZamPiano polyphonic synthesiser
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

#include "ZamPianoUI.hpp"
#include "ZamPianoPlugin.hpp"

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

    int i = 26;
    int j = 34;
    int di = 73;
    int dj = 82;
    fKnobBright = new ImageKnob(this, knobImage);
    fKnobBright->setAbsolutePos(i, j);
    fKnobBright->setRange(0.f, 1.0f);
    fKnobBright->setDefault(0.5f);
    fKnobBright->setRotationAngle(240);
    fKnobBright->setCallback(this);

    fKnobDetune = new ImageKnob(this, knobImage);
    fKnobDetune->setAbsolutePos(i + di, j);
    fKnobDetune->setRange(0.f, 1.0f);
    fKnobDetune->setDefault(0.1f);
    fKnobDetune->setRotationAngle(240);
    fKnobDetune->setCallback(this);

    fKnobHammer = new ImageKnob(this, knobImage);
    fKnobHammer->setAbsolutePos(i+2*di, j);
    fKnobHammer->setRange(-30.f, 30.0f);
    fKnobHammer->setDefault(0.0f);
    fKnobHammer->setRotationAngle(240);
    fKnobHammer->setCallback(this);

    fKnobStiff = new ImageKnob(this, knobImage);
    fKnobStiff->setAbsolutePos(i + 3*di, j);
    fKnobStiff->setRange(0.f, 1.0f);
    fKnobStiff->setDefault(0.28f);
    fKnobStiff->setRotationAngle(240);
    fKnobStiff->setCallback(this);

    fKnobReverbGain = new ImageKnob(this, knobImage);
    fKnobReverbGain->setAbsolutePos(i, j + dj);
    fKnobReverbGain->setRange(0.f, 1.0f);
    fKnobReverbGain->setDefault(0.137f);
    fKnobReverbGain->setRotationAngle(240);
    fKnobReverbGain->setCallback(this);

    fKnobReverbRoom = new ImageKnob(this, knobImage);
    fKnobReverbRoom->setAbsolutePos(i + di, j + dj);
    fKnobReverbRoom->setRange(0.f, 2.0f);
    fKnobReverbRoom->setDefault(0.72f);
    fKnobReverbRoom->setRotationAngle(240);
    fKnobReverbRoom->setCallback(this);

    fKnobPan = new ImageKnob(this, knobImage);
    fKnobPan->setAbsolutePos(i + 2*di, j + dj);
    fKnobPan->setRange(0.f, 1.0f);
    fKnobPan->setDefault(0.6f);
    fKnobPan->setRotationAngle(240);
    fKnobPan->setCallback(this);

    fKnobWidth = new ImageKnob(this, knobImage);
    fKnobWidth->setAbsolutePos(i + 3*di, j + dj);
    fKnobWidth->setRange(0.f, 1.0f);
    fKnobWidth->setDefault(0.5f);
    fKnobWidth->setRotationAngle(240);
    fKnobWidth->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamPianoUI::parameterChanged(uint32_t index, float value)
{
	switch (index)
	{
	case ZamPianoPlugin::paramDetuning:
		fKnobDetune->setValue(value);
		break;
	case ZamPianoPlugin::paramBright:
		fKnobBright->setValue(value);
		break;
	case ZamPianoPlugin::paramHammerHard:
		fKnobHammer->setValue(value);
		break;
	case ZamPianoPlugin::paramStiffness:
		fKnobStiff->setValue(value);
		break;
	case ZamPianoPlugin::paramReverbGain:
		fKnobReverbGain->setValue(value);
		break;
	case ZamPianoPlugin::paramReverbRoom:
		fKnobReverbRoom->setValue(value);
		break;
	case ZamPianoPlugin::paramPanAngle:
		fKnobPan->setValue(value);
		break;
	case ZamPianoPlugin::paramSpatialWidth:
		fKnobWidth->setValue(value);
		break;
	}
}

void ZamPianoUI::programLoaded(uint32_t index)
{
    if (index != 0)
        return;

    fKnobBright->setValue(0.5f);
    fKnobDetune->setValue(0.1f);
    fKnobHammer->setValue(0.5f);
    fKnobStiff->setValue(0.28f);
    fKnobReverbGain->setValue(0.137f);
    fKnobReverbRoom->setValue(0.72f);
    fKnobPan->setValue(0.6f);
    fKnobWidth->setValue(0.5f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamPianoUI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobBright)
        editParameter(ZamPianoPlugin::paramBright, true);
    else if (knob == fKnobDetune)
        editParameter(ZamPianoPlugin::paramDetuning, true);
    else if (knob == fKnobHammer)
        editParameter(ZamPianoPlugin::paramHammerHard, true);
    else if (knob == fKnobStiff)
        editParameter(ZamPianoPlugin::paramStiffness, true);
    else if (knob == fKnobReverbGain)
        editParameter(ZamPianoPlugin::paramReverbGain, true);
    else if (knob == fKnobReverbRoom)
        editParameter(ZamPianoPlugin::paramReverbRoom, true);
    else if (knob == fKnobPan)
        editParameter(ZamPianoPlugin::paramPanAngle, true);
    else if (knob == fKnobWidth)
        editParameter(ZamPianoPlugin::paramSpatialWidth, true);
}

void ZamPianoUI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobBright)
        editParameter(ZamPianoPlugin::paramBright, false);
    else if (knob == fKnobDetune)
        editParameter(ZamPianoPlugin::paramDetuning, false);
    else if (knob == fKnobHammer)
        editParameter(ZamPianoPlugin::paramHammerHard, false);
    else if (knob == fKnobStiff)
        editParameter(ZamPianoPlugin::paramStiffness, false);
    else if (knob == fKnobReverbGain)
        editParameter(ZamPianoPlugin::paramReverbGain, false);
    else if (knob == fKnobReverbRoom)
        editParameter(ZamPianoPlugin::paramReverbRoom, false);
    else if (knob == fKnobPan)
        editParameter(ZamPianoPlugin::paramPanAngle, false);
    else if (knob == fKnobWidth)
        editParameter(ZamPianoPlugin::paramSpatialWidth, false);
}

void ZamPianoUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobBright)
        setParameterValue(ZamPianoPlugin::paramBright, value);
    else if (knob == fKnobDetune)
        setParameterValue(ZamPianoPlugin::paramDetuning, value);
    else if (knob == fKnobHammer)
        setParameterValue(ZamPianoPlugin::paramHammerHard, value);
    else if (knob == fKnobStiff)
        setParameterValue(ZamPianoPlugin::paramStiffness, value);
    else if (knob == fKnobReverbGain)
        setParameterValue(ZamPianoPlugin::paramReverbGain, value);
    else if (knob == fKnobReverbRoom)
        setParameterValue(ZamPianoPlugin::paramReverbRoom, value);
    else if (knob == fKnobPan)
        setParameterValue(ZamPianoPlugin::paramPanAngle, value);
    else if (knob == fKnobWidth)
        setParameterValue(ZamPianoPlugin::paramSpatialWidth, value);
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
