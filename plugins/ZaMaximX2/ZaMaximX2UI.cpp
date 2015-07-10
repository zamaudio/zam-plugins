/*
 * ZaMaximX2 stereo maximiser
 * Copyright (C) 2015  Damien Zammit <damien@zamaudio.com>
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

#include "ZaMaximX2Plugin.hpp"
#include "ZaMaximX2UI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZaMaximX2UI::ZaMaximX2UI()
    : UI()
{
    setSize(ZaMaximX2Artwork::zamaximx2Width, ZaMaximX2Artwork::zamaximx2Height);

    // background
    fImgBackground = Image(ZaMaximX2Artwork::zamaximx2Data, ZaMaximX2Artwork::zamaximx2Width, ZaMaximX2Artwork::zamaximx2Height, GL_BGR);

    // led images
    fLedRedImg = Image(ZaMaximX2Artwork::ledredData, ZaMaximX2Artwork::ledredWidth, ZaMaximX2Artwork::ledredHeight);
    fLedYellowImg = Image(ZaMaximX2Artwork::ledyellowData, ZaMaximX2Artwork::ledyellowWidth, ZaMaximX2Artwork::ledyellowHeight);

    // led values
    fLedRedValue = 0.0f;
    fLedYellowValue = 0.0f;

    // knob
    Image knobImage(ZaMaximX2Artwork::knobData, ZaMaximX2Artwork::knobWidth, ZaMaximX2Artwork::knobHeight);

    // knob
    fKnobRelease = new ImageKnob(this, knobImage);
    fKnobRelease->setAbsolutePos(27, 46);
    fKnobRelease->setId(ZaMaximX2Plugin::paramRelease);
    fKnobRelease->setRange(0.1f, 100.0f);
    fKnobRelease->setStep(0.1f);
    fKnobRelease->setDefault(1.0f);
    fKnobRelease->setRotationAngle(240);
    fKnobRelease->setCallback(this);

    fKnobThresh = new ImageKnob(this, knobImage);
    fKnobThresh->setAbsolutePos(110, 46);
    fKnobThresh->setId(ZaMaximX2Plugin::paramThresh);
    fKnobThresh->setRange(-60.0f, 0.0f);
    fKnobThresh->setStep(1.0f);
    fKnobThresh->setDefault(0.0f);
    fKnobThresh->setRotationAngle(240);
    fKnobThresh->setCallback(this);

    fKnobCeiling = new ImageKnob(this, knobImage);
    fKnobCeiling->setAbsolutePos(192, 46);
    fKnobCeiling->setId(ZaMaximX2Plugin::paramCeiling);
    fKnobCeiling->setRange(-40.0f, 0.0f);
    fKnobCeiling->setStep(1.0f);
    fKnobCeiling->setDefault(-6.0f);
    fKnobCeiling->setRotationAngle(240);
    fKnobCeiling->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZaMaximX2UI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZaMaximX2Plugin::paramRelease:
        fKnobRelease->setValue(value);
        break;
    case ZaMaximX2Plugin::paramThresh:
        fKnobThresh->setValue(value);
        break;
    case ZaMaximX2Plugin::paramCeiling:
        fKnobCeiling->setValue(value);
        break;
    case ZaMaximX2Plugin::paramGainRed:
        if (fLedRedValue != value)
        {
            fLedRedValue = value;
            repaint();
        }
        break;
    case ZaMaximX2Plugin::paramOutputLevel:
        if (fLedYellowValue != value)
        {
            fLedYellowValue = value;
            repaint();
        }
        break;
    }
}

void ZaMaximX2UI::programLoaded(uint32_t index)
{
	switch(index) {
	case 0:
		fKnobRelease->setValue(1.0f);
		fKnobThresh->setValue(0.0f);
		fKnobCeiling->setValue(-6.0f);
		break;
	}
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZaMaximX2UI::imageKnobDragStarted(ImageKnob* knob)
{
    editParameter(knob->getId(), true);
}

void ZaMaximX2UI::imageKnobDragFinished(ImageKnob* knob)
{
    editParameter(knob->getId(), false);
}

void ZaMaximX2UI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    setParameterValue(knob->getId(), value);
}

void ZaMaximX2UI::onDisplay()
{
    fImgBackground.draw();

    // draw leds
    static const float sLedSpacing  = 15.5f;
    static const int   sLedInitialX = 256;

    static const int sYellowLedStaticY = 18;
    static const int sRedLedStaticY    = 47;

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
    return new ZaMaximX2UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
