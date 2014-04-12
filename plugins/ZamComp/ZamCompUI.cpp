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

#include "ZamCompUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamCompUI::ZamCompUI()
    : UI()
{
    // background
    fImgBackground = Image(ZamCompArtwork::zamcompData, ZamCompArtwork::zamcompWidth, ZamCompArtwork::zamcompHeight, GL_BGR);

    // led images
    fLedRedImg = Image(ZamCompArtwork::ledredData, ZamCompArtwork::ledredWidth, ZamCompArtwork::ledredHeight);

    // led values
    fLedRedValue = 0.0f; // FIXME: set as 0.0f, this is just for testing

    // knob
    Image knobImage(ZamCompArtwork::knobData, ZamCompArtwork::knobWidth, ZamCompArtwork::knobHeight);

    // knob 
    fKnobAttack = new ImageKnob(this, knobImage);
    fKnobAttack->setPos(24, 45);
    fKnobAttack->setRange(0.1f, 200.0f);
    fKnobAttack->setStep(0.1f);
    fKnobAttack->setValue(10.0f);
    fKnobAttack->setRotationAngle(240);
    fKnobAttack->setCallback(this);

    fKnobRelease = new ImageKnob(this, knobImage);
    fKnobRelease->setPos(108, 45);
    fKnobRelease->setRange(50.0f, 500.0f);
    fKnobRelease->setStep(1.0f);
    fKnobRelease->setValue(80.0f);
    fKnobRelease->setRotationAngle(240);
    fKnobRelease->setCallback(this);

    fKnobThresh = new ImageKnob(this, knobImage);
    fKnobThresh->setPos(191.5, 45);
    fKnobThresh->setRange(-60.0f, 0.0f);
    fKnobThresh->setStep(1.0f);
    fKnobThresh->setValue(0.0f);
    fKnobThresh->setRotationAngle(240);
    fKnobThresh->setCallback(this);

    fKnobRatio = new ImageKnob(this, knobImage);
    fKnobRatio->setPos(270, 45);
    fKnobRatio->setRange(1.0f, 20.0f);
    fKnobRatio->setStep(0.1f);
    fKnobRatio->setValue(4.0f);
    fKnobRatio->setRotationAngle(240);
    fKnobRatio->setCallback(this);

    fKnobKnee = new ImageKnob(this, knobImage);
    fKnobKnee->setPos(348.5, 45);
    fKnobKnee->setRange(0.0f, 8.0f);
    fKnobKnee->setStep(0.1f);
    fKnobKnee->setValue(0.0f);
    fKnobKnee->setRotationAngle(240);
    fKnobKnee->setCallback(this);

    fKnobMakeup = new ImageKnob(this, knobImage);
    fKnobMakeup->setPos(427.3, 45);
    fKnobMakeup->setRange(-30.0f, 30.0f);
    fKnobMakeup->setStep(1.0f);
    fKnobMakeup->setValue(0.0f);
    fKnobMakeup->setRotationAngle(240);
    fKnobMakeup->setCallback(this);
}

ZamCompUI::~ZamCompUI()
{
    delete fKnobAttack;
    delete fKnobRelease;
    delete fKnobThresh;
    delete fKnobRatio;
    delete fKnobKnee;
    delete fKnobMakeup;
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamCompUI::d_parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamCompPlugin::paramAttack:
        fKnobAttack->setValue(value);
        break;
    case ZamCompPlugin::paramRelease:
        fKnobRelease->setValue(value);
        break;
    case ZamCompPlugin::paramThresh:
        fKnobThresh->setValue(value);
        break;
    case ZamCompPlugin::paramRatio:
        fKnobRatio->setValue(value);
        break;
    case ZamCompPlugin::paramKnee:
        fKnobKnee->setValue(value);
        break;
    case ZamCompPlugin::paramMakeup:
        fKnobMakeup->setValue(value);
        break;
    case ZamCompPlugin::paramGainR:
        if (fLedRedValue != value)
        {
            fLedRedValue = value;
            repaint();
        }
        break;
    }
}

void ZamCompUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    // Default values
    fKnobAttack->setValue(10.0f);
    fKnobRelease->setValue(80.0f);
    fKnobThresh->setValue(0.0f);
    fKnobRatio->setValue(4.0f);
    fKnobKnee->setValue(0.0f);
    fKnobMakeup->setValue(0.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamCompUI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZamCompPlugin::paramAttack, true);
    else if (knob == fKnobRelease)
        d_editParameter(ZamCompPlugin::paramRelease, true);
    else if (knob == fKnobThresh)
        d_editParameter(ZamCompPlugin::paramThresh, true);
    else if (knob == fKnobRatio)
        d_editParameter(ZamCompPlugin::paramRatio, true);
    else if (knob == fKnobKnee)
        d_editParameter(ZamCompPlugin::paramKnee, true);
    else if (knob == fKnobMakeup)
        d_editParameter(ZamCompPlugin::paramMakeup, true);
}

void ZamCompUI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobAttack)
        d_editParameter(ZamCompPlugin::paramAttack, false);
    else if (knob == fKnobRelease)
        d_editParameter(ZamCompPlugin::paramRelease, false);
    else if (knob == fKnobThresh)
        d_editParameter(ZamCompPlugin::paramThresh, false);
    else if (knob == fKnobRatio)
        d_editParameter(ZamCompPlugin::paramRatio, false);
    else if (knob == fKnobKnee)
        d_editParameter(ZamCompPlugin::paramKnee, false);
    else if (knob == fKnobMakeup)
        d_editParameter(ZamCompPlugin::paramMakeup, false);
}

void ZamCompUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobAttack)
        d_setParameterValue(ZamCompPlugin::paramAttack, value);
    else if (knob == fKnobRelease)
        d_setParameterValue(ZamCompPlugin::paramRelease, value);
    else if (knob == fKnobThresh)
        d_setParameterValue(ZamCompPlugin::paramThresh, value);
    else if (knob == fKnobRatio)
        d_setParameterValue(ZamCompPlugin::paramRatio, value);
    else if (knob == fKnobKnee)
        d_setParameterValue(ZamCompPlugin::paramKnee, value);
    else if (knob == fKnobMakeup)
        d_setParameterValue(ZamCompPlugin::paramMakeup, value);
}

void ZamCompUI::onDisplay()
{
    fImgBackground.draw();

    // draw leds
    static const float sLedSpacing  = 15.5f;
    static const int   sLedInitialX = 498;

    //static const int sYellowLedStaticY = 16;
    static const int sRedLedStaticY    = 45;

    int numRedLeds;

	if (fLedRedValue >= 40.f)
		numRedLeds = 12;
	else if (fLedRedValue >= 30.f)
		numRedLeds = 11;
	else if (fLedRedValue >= 20.f)
		numRedLeds = 10;
	else if (fLedRedValue >= 15.f)
		numRedLeds = 9;
	else if (fLedRedValue >= 10.f)
		numRedLeds = 8;
	else if (fLedRedValue >= 8.f)
		numRedLeds = 7;
	else if (fLedRedValue >= 6.f)
		numRedLeds = 6;
	else if (fLedRedValue >= 5.f)
		numRedLeds = 5;
	else if (fLedRedValue >= 4.f)
		numRedLeds = 4;
	else if (fLedRedValue >= 3.f)
		numRedLeds = 3;
	else if (fLedRedValue >= 2.f)
		numRedLeds = 2;
	else if (fLedRedValue >= 1.f)
		numRedLeds = 1;
	else numRedLeds = 0;

    for (int i=numRedLeds; i>0; --i)
        fLedRedImg.draw(sLedInitialX + (12 - i)*sLedSpacing, sRedLedStaticY);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamCompUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
