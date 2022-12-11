/*
 * ZamGateX2
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
 */

#include "ZamGateX2Plugin.hpp"
#include "ZamGateX2UI.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamGateX2UI::ZamGateX2UI()
    : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true)
{
	// background
	fImgBackground = Image(ZamGateX2Artwork::zamgatex2Data, ZamGateX2Artwork::zamgatex2Width, ZamGateX2Artwork::zamgatex2Height, kImageFormatBGR);

	// led images
	fLedRedImg = Image(ZamGateX2Artwork::ledredData, ZamGateX2Artwork::ledredWidth, ZamGateX2Artwork::ledredHeight);
	fLedYellowImg = Image(ZamGateX2Artwork::ledyellowData, ZamGateX2Artwork::ledyellowWidth, ZamGateX2Artwork::ledyellowHeight);

	// toggle images
	fTogOn = Image(ZamGateX2Artwork::toggleonData, ZamGateX2Artwork::toggleonWidth, ZamGateX2Artwork::toggleonHeight);
	fTogOff = Image(ZamGateX2Artwork::toggleoffData, ZamGateX2Artwork::toggleoffWidth, ZamGateX2Artwork::toggleoffHeight);

	// led values
	fLedRedValue = 0.0f;
	fLedYellowValue = -45.0f;

	// knob
	Image knobImage(ZamGateX2Artwork::knobData, ZamGateX2Artwork::knobWidth, ZamGateX2Artwork::knobHeight);

	// knob
	fKnobAttack = new ZamKnob(this, knobImage);
	fKnobAttack->setAbsolutePos(24, 45);
	fKnobAttack->setId(ZamGateX2Plugin::paramAttack);
	fKnobAttack->setRange(0.1f, 500.0f);
	fKnobAttack->setLabel(true);
	fKnobAttack->setScrollStep(1.0f);
	fKnobAttack->setDefault(50.0f);
	fKnobAttack->setRotationAngle(240);
	fKnobAttack->setCallback(this);

	fKnobRelease = new ZamKnob(this, knobImage);
	fKnobRelease->setAbsolutePos(108, 45);
	fKnobRelease->setId(ZamGateX2Plugin::paramRelease);
	fKnobRelease->setRange(0.1f, 500.0f);
	fKnobRelease->setLabel(true);
	fKnobRelease->setScrollStep(1.0f);
	fKnobRelease->setDefault(100.0f);
	fKnobRelease->setRotationAngle(240);
	fKnobRelease->setCallback(this);

	fKnobThresh = new ZamKnob(this, knobImage);
	fKnobThresh->setAbsolutePos(191.7, 45);
	fKnobThresh->setId(ZamGateX2Plugin::paramThresh);
	fKnobThresh->setRange(-60.0f, 0.0f);
	fKnobThresh->setLabel(true);
	fKnobThresh->setScrollStep(1.0f);
	fKnobThresh->setDefault(-60.0f);
	fKnobThresh->setRotationAngle(240);
	fKnobThresh->setCallback(this);

	fKnobMakeup = new ZamKnob(this, knobImage);
	fKnobMakeup->setAbsolutePos(273, 45);
	fKnobMakeup->setId(ZamGateX2Plugin::paramMakeup);
	fKnobMakeup->setRange(-30.0f, 30.0f);
	fKnobMakeup->setLabel(true);
	fKnobMakeup->setScrollStep(1.0f);
	fKnobMakeup->setDefault(0.0f);
	fKnobMakeup->setRotationAngle(240);
	fKnobMakeup->setCallback(this);

	fKnobGateclose = new ZamKnob(this, knobImage);
	fKnobGateclose->setAbsolutePos(480, 61);
	fKnobGateclose->setId(ZamGateX2Plugin::paramGateclose);
	fKnobGateclose->setRange(-50.0f, 0.0f);
	fKnobGateclose->setLabel(true);
	fKnobGateclose->setScrollStep(1.0f);
	fKnobGateclose->setDefault(-50.0f);
	fKnobGateclose->setRotationAngle(240);
	fKnobGateclose->setCallback(this);

	fToggleSidechain = new ImageSwitch(this, fTogOff, fTogOn);
	fToggleSidechain->setAbsolutePos(350, 61);
	fToggleSidechain->setId(ZamGateX2Plugin::paramSidechain);
	fToggleSidechain->setCallback(this);

	fToggleMode = new ImageSwitch(this, fTogOff, fTogOn);
	fToggleMode->setAbsolutePos(580, 61);
	fToggleMode->setId(ZamGateX2Plugin::paramOpenshut);
	fToggleMode->setCallback(this);

	// set default values
	programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamGateX2UI::parameterChanged(uint32_t index, float value)
{
	switch (index)
	{
	case ZamGateX2Plugin::paramAttack:
		fKnobAttack->setValue(value);
		break;
	case ZamGateX2Plugin::paramRelease:
		fKnobRelease->setValue(value);
		break;
	case ZamGateX2Plugin::paramThresh:
		fKnobThresh->setValue(value);
		break;
	case ZamGateX2Plugin::paramMakeup:
		fKnobMakeup->setValue(value);
		break;
	case ZamGateX2Plugin::paramGateclose:
		fKnobGateclose->setValue(value);
		break;
	case ZamGateX2Plugin::paramSidechain:
		fToggleSidechain->setDown(value > 0.5);
		break;
	case ZamGateX2Plugin::paramGainR:
		if (fLedRedValue != value)
		{
			fLedRedValue = value;
			repaint();
		}
		break;
	case ZamGateX2Plugin::paramOutputLevel:
		if (fLedYellowValue != value)
		{
			fLedYellowValue = value;
			repaint();
		}
		break;
	}
}

void ZamGateX2UI::programLoaded(uint32_t)
{
	// Default values
	fKnobAttack->setValue(50.0f);
	fKnobRelease->setValue(100.0f);
	fKnobThresh->setValue(-60.0f);
	fKnobMakeup->setValue(0.0f);
	fKnobGateclose->setValue(-50.0f);
	fToggleSidechain->setDown(false);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamGateX2UI::imageKnobDragStarted(ZamKnob* knob)
{
	editParameter(knob->getId(), true);
}

void ZamGateX2UI::imageKnobDragFinished(ZamKnob* knob)
{
	editParameter(knob->getId(), false);
}

void ZamGateX2UI::imageKnobValueChanged(ZamKnob* knob, float value)
{
	setParameterValue(knob->getId(), value);
}

void ZamGateX2UI::imageSwitchClicked(ImageSwitch* tog, bool down)
{
	setParameterValue(tog->getId(), down ? 1.f : 0.f);
}

void ZamGateX2UI::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());
	fImgBackground.draw(context);

	// draw leds
	static const float sLedSpacing  = 15.5f;
	static const int   sLedInitialX = 344.7;

	static const int sYellowLedStaticY = 16;
	static const int sRedLedStaticY	= 45;

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
		fLedRedImg.drawAt(context, sLedInitialX + (12 - i)*sLedSpacing, sRedLedStaticY);

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
			fLedRedImg.drawAt(context, sLedInitialX + i*sLedSpacing, sYellowLedStaticY);
		for (int i=0; i<12; ++i)
			fLedYellowImg.drawAt(context, sLedInitialX + i*sLedSpacing, sYellowLedStaticY);
	} else {
		for (int i=0; i<numYellowLeds; ++i)
			fLedYellowImg.drawAt(context, sLedInitialX + i*sLedSpacing, sYellowLedStaticY);
	}
}

// -----------------------------------------------------------------------

UI* createUI()
{
	return new ZamGateX2UI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
