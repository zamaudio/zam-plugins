/*
 * Wobble Juice Plugin
 * Copyright (C) 2014 Andre Sklenar <andre.sklenar@gmail.com>, www.juicelab.cz
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

#include "ZamTubeUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamTubeUI::ZamTubeUI()
    : UI(),
      fAboutWindow(this)
{
    // background
    fImgBackground = Image(ZamTubeArtwork::backgroundData, ZamTubeArtwork::backgroundWidth, ZamTubeArtwork::backgroundHeight, GL_BGR);

    // about
    Image imageAbout(ZamTubeArtwork::aboutData, ZamTubeArtwork::aboutWidth, ZamTubeArtwork::aboutHeight, GL_BGR);
    fAboutWindow.setImage(imageAbout);

    // knobs
    Image knobImage(ZamTubeArtwork::knobData, ZamTubeArtwork::knobWidth, ZamTubeArtwork::knobHeight);

    // knob 
    fKnobAttack = new ImageKnob(this, knobImage);
    fKnobAttack->setPos(222, 74);
    fKnobAttack->setRange(0.1f, 100.0f);
    fKnobAttack->setStep(0.1f);
    fKnobAttack->setValue(10.0f);
    fKnobAttack->setRotationAngle(270);
    fKnobAttack->setCallback(this);

    // about button
    Image aboutImageNormal(ZamTubeArtwork::aboutButtonNormalData, ZamTubeArtwork::aboutButtonNormalWidth, ZamTubeArtwork::aboutButtonNormalHeight);
    Image aboutImageHover(ZamTubeArtwork::aboutButtonHoverData, ZamTubeArtwork::aboutButtonHoverWidth, ZamTubeArtwork::aboutButtonHoverHeight);
    fButtonAbout = new ImageButton(this, aboutImageNormal, aboutImageHover, aboutImageHover);
    fButtonAbout->setPos(390, 20);
    fButtonAbout->setCallback(this);
}

ZamTubeUI::~ZamTubeUI()
{
    delete fKnobAttack;
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamTubeUI::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamTubePlugin::paramTubedrive:
        fKnobAttack->setValue(value);
        break;
    }
}

void ZamTubeUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fKnobAttack->setValue(10.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamTubeUI::imageButtonClicked(ImageButton* button, int)
{
    if (button != fButtonAbout)
        return;

    fAboutWindow.exec();
}

void ZamTubeUI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZamTubePlugin::paramTubedrive, true);
}

void ZamTubeUI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZamTubePlugin::paramTubedrive, false);
}

void ZamTubeUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobAttack)
        d_setParameterValue(ZamTubePlugin::paramTubedrive, value);
}

void ZamTubeUI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamTubeUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
