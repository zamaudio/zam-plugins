/*
 * ZamCompX2 Stereo compressor
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

#include "ZamCompX2Plugin.hpp"
#include "ZamCompX2UI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamCompX2UI::ZamCompX2UI()
    : UI()
{
    setSize(ZamCompX2Artwork::zamcompx2Width, ZamCompX2Artwork::zamcompx2Height);

    // background
    fImgBackground = Image(ZamCompX2Artwork::zamcompx2Data, ZamCompX2Artwork::zamcompx2Width, ZamCompX2Artwork::zamcompx2Height, GL_BGR);

    // led images
    fLedRedImg = Image(ZamCompX2Artwork::ledredData, ZamCompX2Artwork::ledredWidth, ZamCompX2Artwork::ledredHeight);
    fLedYellowImg = Image(ZamCompX2Artwork::ledyellowData, ZamCompX2Artwork::ledyellowWidth, ZamCompX2Artwork::ledyellowHeight);

    // toggle images
    fToggleOnImg = Image(ZamCompX2Artwork::toggleonData, ZamCompX2Artwork::toggleonWidth, ZamCompX2Artwork::toggleonHeight);
    fToggleOffImg = Image(ZamCompX2Artwork::toggleoffData, ZamCompX2Artwork::toggleoffWidth, ZamCompX2Artwork::toggleoffHeight);

    // led values
    fLedRedValue = 0.0f;
    fLedYellowValue = 0.0f;

    // knob
    Image knobImage(ZamCompX2Artwork::knobData, ZamCompX2Artwork::knobWidth, ZamCompX2Artwork::knobHeight);

    // knob
    fKnobAttack = new ImageKnob(this, knobImage);
    fKnobAttack->setAbsolutePos(24, 45);
    fKnobAttack->setId(ZamCompX2Plugin::paramAttack);
    fKnobAttack->setRange(0.1f, 200.0f);
    fKnobAttack->setUsingLogScale(true);
    fKnobAttack->setDefault(10.0f);
    fKnobAttack->setRotationAngle(240);
    fKnobAttack->setCallback(this);

    fKnobRelease = new ImageKnob(this, knobImage);
    fKnobRelease->setAbsolutePos(108, 45);
    fKnobRelease->setId(ZamCompX2Plugin::paramRelease);
    fKnobRelease->setRange(1.0f, 500.0f);
    fKnobRelease->setDefault(80.0f);
    fKnobRelease->setRotationAngle(240);
    fKnobRelease->setCallback(this);

    fKnobThresh = new ImageKnob(this, knobImage);
    fKnobThresh->setAbsolutePos(191.5, 45);
    fKnobThresh->setId(ZamCompX2Plugin::paramThresh);
    fKnobThresh->setRange(-60.0f, 0.0f);
    fKnobThresh->setDefault(0.0f);
    fKnobThresh->setRotationAngle(240);
    fKnobThresh->setCallback(this);

    fKnobRatio = new ImageKnob(this, knobImage);
    fKnobRatio->setAbsolutePos(270, 45);
    fKnobRatio->setId(ZamCompX2Plugin::paramRatio);
    fKnobRatio->setRange(1.0f, 20.0f);
    fKnobRatio->setUsingLogScale(true);
    fKnobRatio->setDefault(4.0f);
    fKnobRatio->setRotationAngle(240);
    fKnobRatio->setCallback(this);

    fKnobKnee = new ImageKnob(this, knobImage);
    fKnobKnee->setAbsolutePos(348.5, 45);
    fKnobKnee->setId(ZamCompX2Plugin::paramKnee);
    fKnobKnee->setRange(0.0f, 8.0f);
    fKnobKnee->setDefault(0.0f);
    fKnobKnee->setRotationAngle(240);
    fKnobKnee->setCallback(this);

    fKnobMakeup = new ImageKnob(this, knobImage);
    fKnobMakeup->setAbsolutePos(638, 62);
    fKnobMakeup->setId(ZamCompX2Plugin::paramMakeup);
    fKnobMakeup->setRange(-30.0f, 30.0f);
    fKnobMakeup->setDefault(0.0f);
    fKnobMakeup->setRotationAngle(240);
    fKnobMakeup->setCallback(this);

    fKnobSlew = new ImageKnob(this, knobImage);
    fKnobSlew->setAbsolutePos(427.3, 45);
    fKnobSlew->setId(ZamCompX2Plugin::paramSlew);
    fKnobSlew->setRange(1.0f, 150.0f);
    fKnobSlew->setDefault(1.0f);
    fKnobSlew->setRotationAngle(240);
    fKnobSlew->setCallback(this);

    fToggleSidechain = new ImageSwitch(this, fToggleOffImg, fToggleOnImg);
    fToggleSidechain->setAbsolutePos(500, 61);
    fToggleSidechain->setId(ZamCompX2Plugin::paramSidechain);
    fToggleSidechain->setCallback(this);

    fToggleStereo = new ImageSwitch(this, fToggleOffImg, fToggleOnImg);
    fToggleStereo->setAbsolutePos(740, 61);
    fToggleStereo->setId(ZamCompX2Plugin::paramStereo);
    fToggleStereo->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamCompX2UI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamCompX2Plugin::paramAttack:
        fKnobAttack->setValue(value);
        break;
    case ZamCompX2Plugin::paramRelease:
        fKnobRelease->setValue(value);
        break;
    case ZamCompX2Plugin::paramThresh:
        fKnobThresh->setValue(value);
        break;
    case ZamCompX2Plugin::paramRatio:
        fKnobRatio->setValue(value);
        break;
    case ZamCompX2Plugin::paramKnee:
        fKnobKnee->setValue(value);
        break;
    case ZamCompX2Plugin::paramMakeup:
        fKnobMakeup->setValue(value);
        break;
    case ZamCompX2Plugin::paramSlew:
        fKnobSlew->setValue(value);
        break;
    case ZamCompX2Plugin::paramSidechain:
        fToggleSidechain->setDown(value > 0.5);
        break;
    case ZamCompX2Plugin::paramStereo:
        fToggleStereo->setDown(value > 0.5);
        break;
    case ZamCompX2Plugin::paramGainRed:
        if (fLedRedValue != value)
        {
            fLedRedValue = value;
            repaint();
        }
        break;
    case ZamCompX2Plugin::paramOutputLevel:
        if (fLedYellowValue != value)
        {
            fLedYellowValue = value;
            repaint();
        }
        break;
    }
}

void ZamCompX2UI::programLoaded(uint32_t index)
{
	switch(index) {
	case 0:
		fKnobAttack->setValue(10.0f);
		fKnobRelease->setValue(80.0f);
		fKnobKnee->setValue(0.0f);
		fKnobRatio->setValue(4.0f);
		fKnobThresh->setValue(0.0f);
		fKnobMakeup->setValue(0.0f);
		fKnobSlew->setValue(1.0f);
		fToggleStereo->setDown(false);
		fToggleSidechain->setDown(false);
		break;
	case 1:
		fKnobAttack->setValue(10.0f);
		fKnobRelease->setValue(80.0f);
		fKnobKnee->setValue(1.0f);
		fKnobRatio->setValue(5.0f);
		fKnobThresh->setValue(-18.0f);
		fKnobMakeup->setValue(6.0f);
		fKnobSlew->setValue(20.0f);
		fToggleStereo->setDown(true);
		fToggleSidechain->setDown(false);
		break;
	case 2:
		fKnobAttack->setValue(50.0f);
		fKnobRelease->setValue(400.0f);
		fKnobKnee->setValue(8.0f);
		fKnobRatio->setValue(5.0f);
		fKnobThresh->setValue(-16.0f);
		fKnobMakeup->setValue(9.0f);
		fKnobSlew->setValue(1.0f);
		fToggleStereo->setDown(true);
		fToggleSidechain->setDown(false);
		break;
	}
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamCompX2UI::imageKnobDragStarted(ImageKnob* knob)
{
    editParameter(knob->getId(), true);
}

void ZamCompX2UI::imageKnobDragFinished(ImageKnob* knob)
{
    editParameter(knob->getId(), false);
}

void ZamCompX2UI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    setParameterValue(knob->getId(), value);
}

void ZamCompX2UI::imageSwitchClicked(ImageSwitch* tog, bool down)
{
    setParameterValue(tog->getId(), down ? 1.f : 0.f);
}

void ZamCompX2UI::onDisplay()
{
    fImgBackground.draw();

    // draw leds
    static const float sLedSpacing  = 15.5f;
    static const int   sLedInitialX = 498;

    static const int sYellowLedStaticY = 16;
    static const int sRedLedStaticY    = 45;

    int numRedLeds;
    int numYellowLeds;

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
        fLedRedImg.drawAt(sLedInitialX + (12 - i)*sLedSpacing, sRedLedStaticY);

	if (fLedYellowValue >= 20.f)
		numYellowLeds = 19;
	else if (fLedYellowValue >= 10.f)
		numYellowLeds = 18;
	else if (fLedYellowValue >= 8.f)
		numYellowLeds = 17;
	else if (fLedYellowValue >= 4.f)
		numYellowLeds = 16;
	else if (fLedYellowValue >= 2.f)
		numYellowLeds = 15;
	else if (fLedYellowValue >= 1.f)
		numYellowLeds = 14;
	else if (fLedYellowValue >= 0.f)
		numYellowLeds = 13;
	else if (fLedYellowValue >= -1.f)
		numYellowLeds = 12;
	else if (fLedYellowValue >= -2.f)
		numYellowLeds = 11;
	else if (fLedYellowValue >= -3.f)
		numYellowLeds = 10;
	else if (fLedYellowValue >= -4.f)
		numYellowLeds = 9;
	else if (fLedYellowValue >= -5.f)
		numYellowLeds = 8;
	else if (fLedYellowValue >= -6.f)
		numYellowLeds = 7;
	else if (fLedYellowValue >= -8.f)
		numYellowLeds = 6;
	else if (fLedYellowValue >= -10.f)
		numYellowLeds = 5;
	else if (fLedYellowValue >= -15.f)
		numYellowLeds = 4;
	else if (fLedYellowValue >= -20.f)
		numYellowLeds = 3;
	else if (fLedYellowValue >= -30.f)
		numYellowLeds = 2;
	else if (fLedYellowValue >= -40.f)
		numYellowLeds = 1;
	else numYellowLeds = 0;

	if (numYellowLeds > 12) {
		for (int i=12; i<numYellowLeds; ++i)
			fLedRedImg.drawAt(sLedInitialX + i*sLedSpacing, sYellowLedStaticY);
		for (int i=0; i<12; ++i)
			fLedYellowImg.drawAt(sLedInitialX + i*sLedSpacing, sYellowLedStaticY);
	} else {
		for (int i=0; i<numYellowLeds; ++i)
			fLedYellowImg.drawAt(sLedInitialX + i*sLedSpacing, sYellowLedStaticY);
	}
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamCompX2UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
