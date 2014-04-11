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

#include "ZaMultiCompUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZaMultiCompUI::ZaMultiCompUI()
    : UI(),
      fAboutWindow(this)
{
    // background
    fImgBackground = Image(ZaMultiCompArtwork::backgroundData, ZaMultiCompArtwork::backgroundWidth, ZaMultiCompArtwork::backgroundHeight, GL_BGR);

    // about
    Image imageAbout(ZaMultiCompArtwork::aboutData, ZaMultiCompArtwork::aboutWidth, ZaMultiCompArtwork::aboutHeight, GL_BGR);
    fAboutWindow.setImage(imageAbout);

    // knobs
    Image knobImage(ZaMultiCompArtwork::knobData, ZaMultiCompArtwork::knobWidth, ZaMultiCompArtwork::knobHeight);

    // knob 
    fKnobAttack = new ImageKnob(this, knobImage);
    fKnobAttack->setPos(222, 74);
    fKnobAttack->setRange(0.1f, 100.0f);
    fKnobAttack->setStep(0.1f);
    fKnobAttack->setValue(10.0f);
    fKnobAttack->setRotationAngle(270);
    fKnobAttack->setCallback(this);

    // about button
    Image aboutImageNormal(ZaMultiCompArtwork::aboutButtonNormalData, ZaMultiCompArtwork::aboutButtonNormalWidth, ZaMultiCompArtwork::aboutButtonNormalHeight);
    Image aboutImageHover(ZaMultiCompArtwork::aboutButtonHoverData, ZaMultiCompArtwork::aboutButtonHoverWidth, ZaMultiCompArtwork::aboutButtonHoverHeight);
    fButtonAbout = new ImageButton(this, aboutImageNormal, aboutImageHover, aboutImageHover);
    fButtonAbout->setPos(390, 20);
    fButtonAbout->setCallback(this);
}

ZaMultiCompUI::~ZaMultiCompUI()
{
    delete fKnobAttack;
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZaMultiCompUI::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZaMultiCompPlugin::paramAttack:
        fKnobAttack->setValue(value);
        break;
    }
}

void ZaMultiCompUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fKnobAttack->setValue(10.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZaMultiCompUI::imageButtonClicked(ImageButton* button, int)
{
    if (button != fButtonAbout)
        return;

    fAboutWindow.exec();
}

void ZaMultiCompUI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZaMultiCompPlugin::paramAttack, true);
}

void ZaMultiCompUI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZaMultiCompPlugin::paramAttack, false);
}

void ZaMultiCompUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobAttack)
        d_setParameterValue(ZaMultiCompPlugin::paramAttack, value);
}

void ZaMultiCompUI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZaMultiCompUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
