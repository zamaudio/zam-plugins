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

#include "ZamCompX2UI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamCompX2UI::ZamCompX2UI()
    : UI(),
      fAboutWindow(this)
{
    // background
    fImgBackground = Image(ZamCompX2Artwork::backgroundData, ZamCompX2Artwork::backgroundWidth, ZamCompX2Artwork::backgroundHeight, GL_BGR);

    // about
    Image imageAbout(ZamCompX2Artwork::aboutData, ZamCompX2Artwork::aboutWidth, ZamCompX2Artwork::aboutHeight, GL_BGR);
    fAboutWindow.setImage(imageAbout);

    // knobs
    Image knobImage(ZamCompX2Artwork::knobData, ZamCompX2Artwork::knobWidth, ZamCompX2Artwork::knobHeight);

    // knob 
    fKnobAttack = new ImageKnob(this, knobImage);
    fKnobAttack->setPos(222, 74);
    fKnobAttack->setRange(0.1f, 100.0f);
    fKnobAttack->setStep(0.1f);
    fKnobAttack->setValue(10.0f);
    fKnobAttack->setRotationAngle(270);
    fKnobAttack->setCallback(this);

    // about button
    Image aboutImageNormal(ZamCompX2Artwork::aboutButtonNormalData, ZamCompX2Artwork::aboutButtonNormalWidth, ZamCompX2Artwork::aboutButtonNormalHeight);
    Image aboutImageHover(ZamCompX2Artwork::aboutButtonHoverData, ZamCompX2Artwork::aboutButtonHoverWidth, ZamCompX2Artwork::aboutButtonHoverHeight);
    fButtonAbout = new ImageButton(this, aboutImageNormal, aboutImageHover, aboutImageHover);
    fButtonAbout->setPos(390, 20);
    fButtonAbout->setCallback(this);
}

ZamCompX2UI::~ZamCompX2UI()
{
    delete fKnobAttack;
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamCompX2UI::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamCompX2Plugin::paramAttack:
        fKnobAttack->setValue(value);
        break;
    }
}

void ZamCompX2UI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fKnobAttack->setValue(10.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamCompX2UI::imageButtonClicked(ImageButton* button, int)
{
    if (button != fButtonAbout)
        return;

    fAboutWindow.exec();
}

void ZamCompX2UI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZamCompX2Plugin::paramAttack, true);
}

void ZamCompX2UI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZamCompX2Plugin::paramAttack, false);
}

void ZamCompX2UI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobAttack)
        d_setParameterValue(ZamCompX2Plugin::paramAttack, value);
}

void ZamCompX2UI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamCompX2UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
