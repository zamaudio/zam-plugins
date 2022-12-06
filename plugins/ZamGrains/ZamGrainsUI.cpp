/*
 * ZamGrains Granular Delay
 * Copyright (C) 2018  Damien Zammit <damien@zamaudio.com>
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
    fImgBackground = Image(ZamGrainsArtwork::zamgrainsData, ZamGrainsArtwork::zamgrainsWidth, ZamGrainsArtwork::zamgrainsHeight, kImageFormatBGR);

    // knob
    Image knobImage(ZamGrainsArtwork::knobData, ZamGrainsArtwork::knobWidth, ZamGrainsArtwork::knobHeight);

    // toggleon
    Image toggleonImage(ZamGrainsArtwork::toggleonData, ZamGrainsArtwork::toggleonWidth, ZamGrainsArtwork::toggleonHeight);
    // toggleoff
    Image toggleoffImage(ZamGrainsArtwork::toggleoffData, ZamGrainsArtwork::toggleoffWidth, ZamGrainsArtwork::toggleoffHeight);

    // knob
    fKnobPlayspeed = new ZamKnob(this, knobImage);
    fKnobPlayspeed->setAbsolutePos(130, 36);
    fKnobPlayspeed->setId(ZamGrainsPlugin::paramPlayspeed);
    fKnobPlayspeed->setRange(0.1f, 20.0f);
    fKnobPlayspeed->setLabel(true);
    fKnobPlayspeed->setScrollStep(0.01f);
    fKnobPlayspeed->setUsingLogScale(true);
    fKnobPlayspeed->setDefault(1.0f);
    fKnobPlayspeed->setRotationAngle(240);
    fKnobPlayspeed->setCallback(this);

    fKnobGrainspeed = new ZamKnob(this, knobImage);
    fKnobGrainspeed->setAbsolutePos(27, 36);
    fKnobGrainspeed->setId(ZamGrainsPlugin::paramGrainspeed);
    fKnobGrainspeed->setRange(0.1f, 20.0f);
    fKnobGrainspeed->setLabel(true);
    fKnobGrainspeed->setScrollStep(0.01f);
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

    Point<int> togglePosStart(212,120);

    fToggleFreeze = new ImageSwitch(this, toggleoffImage, toggleonImage);
    fToggleFreeze->setAbsolutePos(togglePosStart);
    fToggleFreeze->setId(ZamGrainsPlugin::paramFreeze);
    fToggleFreeze->setCallback(this);

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
    case ZamGrainsPlugin::paramFreeze:
        fToggleFreeze->setDown(value > 0.5f);
        break;
    case ZamGrainsPlugin::paramGrainpos:
        grainpos = value;
        break;
    case ZamGrainsPlugin::paramPlaypos:
        playpos = value;
        break;
    case ZamGrainsPlugin::paramFinalpos:
        finalpos = value;
        repaint();
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
		fToggleFreeze->setDown(false);
		grainpos = 0.f;
		playpos = 0.f;
		finalpos = 0.f;
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

void ZamGrainsUI::imageSwitchClicked(ImageSwitch* toggle, bool down)
{
    float val = down ? 1.f : 0.f;
    setParameterValue(toggle->getId(), val);
}

void ZamGrainsUI::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());

    int i;
    int grainx = 0;
    int grains = (int)fKnobGrains->getValue();
    float playspeed = fKnobPlayspeed->getValue();
    float grainspeed = fKnobGrainspeed->getValue();
    int width = 345;
    int height = 140;
    int offsetx = 5;
    int offsety = 175;

    fImgBackground.draw(context);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(8.0);
    glEnable(GL_POINT_SPRITE);

    glLineWidth(1);
    glColor4f(1.f, 1.f, 0.235f, 1.0f);
    for (i = 0; i < grains + 1; ++i) {
        grainx = i * width / grains;
	glBegin(GL_LINES);
        glVertex2i(offsetx + grainx, offsety + 5);
        glVertex2i(offsetx + grainx, offsety + height - 5);
        glEnd();
    }
    // reset color

    glBegin(GL_POINTS);
    glColor4f(0.235f, 0.235f, 1.0f, 1.0f);
    glVertex2i(offsetx + width * playpos, offsety + 5);
    glColor4f(1.0f, 0.235f, 0.235f, 1.0f);
    glVertex2i(offsetx + ((int)(width * playpos * playspeed) % width), offsety + height / 3);
    glColor4f(0.235f, 1.0f, 0.235f, 1.0f);
    glVertex2i(offsetx + ((int)(width * grainpos * grainspeed) % width), offsety + 2 * height / 3);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex2i(offsetx + (int)width * finalpos, offsety + height - 5);
    glEnd();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamGrainsUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
