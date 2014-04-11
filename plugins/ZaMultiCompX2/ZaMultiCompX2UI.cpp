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

#include "ZaMultiCompX2UI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZaMultiCompX2UI::ZaMultiCompX2UI()
    : UI(),
      fAboutWindow(this)
{
    // background
    fImgBackground = Image(ZaMultiCompX2Artwork::backgroundData, ZaMultiCompX2Artwork::backgroundWidth, ZaMultiCompX2Artwork::backgroundHeight, GL_BGR);

    // about
    Image imageAbout(ZaMultiCompX2Artwork::aboutData, ZaMultiCompX2Artwork::aboutWidth, ZaMultiCompX2Artwork::aboutHeight, GL_BGR);
    fAboutWindow.setImage(imageAbout);

    // knobs
    Image knobImage(ZaMultiCompX2Artwork::knobData, ZaMultiCompX2Artwork::knobWidth, ZaMultiCompX2Artwork::knobHeight);

    // knob 
    fKnobAttack = new ImageKnob(this, knobImage);
    fKnobAttack->setPos(222, 74);
    fKnobAttack->setRange(0.1f, 100.0f);
    fKnobAttack->setStep(0.1f);
    fKnobAttack->setValue(10.0f);
    fKnobAttack->setRotationAngle(270);
    fKnobAttack->setCallback(this);

    // about button
    Image aboutImageNormal(ZaMultiCompX2Artwork::aboutButtonNormalData, ZaMultiCompX2Artwork::aboutButtonNormalWidth, ZaMultiCompX2Artwork::aboutButtonNormalHeight);
    Image aboutImageHover(ZaMultiCompX2Artwork::aboutButtonHoverData, ZaMultiCompX2Artwork::aboutButtonHoverWidth, ZaMultiCompX2Artwork::aboutButtonHoverHeight);
    fButtonAbout = new ImageButton(this, aboutImageNormal, aboutImageHover, aboutImageHover);
    fButtonAbout->setPos(390, 20);
    fButtonAbout->setCallback(this);
}

ZaMultiCompX2UI::~ZaMultiCompX2UI()
{
    delete fKnobAttack;
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZaMultiCompX2UI::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZaMultiCompX2Plugin::paramAttack:
        fKnobAttack->setValue(value);
        break;
    }
}

void ZaMultiCompX2UI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fKnobAttack->setValue(10.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZaMultiCompX2UI::imageButtonClicked(ImageButton* button, int)
{
    if (button != fButtonAbout)
        return;

    fAboutWindow.exec();
}

void ZaMultiCompX2UI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZaMultiCompX2Plugin::paramAttack, true);
}

void ZaMultiCompX2UI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZaMultiCompX2Plugin::paramAttack, false);
}

void ZaMultiCompX2UI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobAttack)
        d_setParameterValue(ZaMultiCompX2Plugin::paramAttack, value);
}

void ZaMultiCompX2UI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZaMultiCompX2UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
