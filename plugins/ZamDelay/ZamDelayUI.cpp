/*
 * ZamDelay mono delay effect
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

#include "ZamDelayPlugin.hpp"
#include "ZamDelayUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamDelayUI::ZamDelayUI()
    : UI()
{
    setSize(ZamDelayArtwork::zamdelayWidth, ZamDelayArtwork::zamdelayHeight);

    //fNanoText.loadSharedResources();
    //font = fNanoText.findFont(NANOVG_DEJAVU_SANS_TTF);

    // background
    fImgBackground = Image(ZamDelayArtwork::zamdelayData, ZamDelayArtwork::zamdelayWidth, ZamDelayArtwork::zamdelayHeight, GL_BGR);

    // knob
    Image knobImage(ZamDelayArtwork::knobData, ZamDelayArtwork::knobWidth, ZamDelayArtwork::knobHeight);

    Image synconImage(ZamDelayArtwork::syncbpmonData, ZamDelayArtwork::syncbpmonWidth, ZamDelayArtwork::syncbpmonHeight);

    Image syncoffImage(ZamDelayArtwork::syncbpmoffData, ZamDelayArtwork::syncbpmoffWidth, ZamDelayArtwork::syncbpmoffHeight);

    Image invertonImage(ZamDelayArtwork::invertonData, ZamDelayArtwork::invertoffWidth, ZamDelayArtwork::invertoffHeight);
    Image invertoffImage(ZamDelayArtwork::invertoffData, ZamDelayArtwork::invertoffWidth, ZamDelayArtwork::invertoffHeight);

    Image triangleImage(ZamDelayArtwork::triangleData, ZamDelayArtwork::triangleWidth, ZamDelayArtwork::triangleHeight);
    // knob
    fKnobDelaytime = new ZamKnob(this, knobImage);
    fKnobDelaytime->setAbsolutePos(190, 79);
    fKnobDelaytime->setId(ZamDelayPlugin::paramDelaytime);
    fKnobDelaytime->setRange(1.0f, 8000.0f);
    fKnobDelaytime->setLabel(true);
    fKnobDelaytime->setScrollStep(10.0f);
    fKnobDelaytime->setUsingLogScale(true);
    fKnobDelaytime->setDefault(160.0f);
    fKnobDelaytime->setRotationAngle(240);
    fKnobDelaytime->setCallback(this);

    fToggleInvert = new ImageSwitch(this, invertoffImage, invertonImage);
    fToggleInvert->setAbsolutePos(10, 15);
    fToggleInvert->setId(ZamDelayPlugin::paramInvert);
    fToggleInvert->setCallback(this);

    fToggleBPM = new ImageSwitch(this, syncoffImage, synconImage);
    fToggleBPM->setAbsolutePos(280, 80);
    fToggleBPM->setId(ZamDelayPlugin::paramSync);
    fToggleBPM->setCallback(this);

    fKnobLPF = new ZamKnob(this, knobImage);
    fKnobLPF->setAbsolutePos(67.5, 79);
    fKnobLPF->setId(ZamDelayPlugin::paramLPF);
    fKnobLPF->setRange(20.0f, 20000.0f);
    fKnobLPF->setLabel(true);
    fKnobLPF->setScrollStep(10.0f);
    fKnobLPF->setUsingLogScale(true);
    fKnobLPF->setDefault(6000.0f);
    fKnobLPF->setRotationAngle(240);
    fKnobLPF->setCallback(this);

    fKnobGain = new ZamKnob(this, knobImage);
    fKnobGain->setAbsolutePos(66, 201);
    fKnobGain->setId(ZamDelayPlugin::paramGain);
    fKnobGain->setRange(-60.0f, 0.0f);
    fKnobGain->setLabel(true);
    fKnobGain->setScrollStep(1.0f);
    fKnobGain->setRotationAngle(240);
    fKnobGain->setCallback(this);

    fKnobDrywet = new ZamKnob(this, knobImage);
    fKnobDrywet->setAbsolutePos(332, 231);
    fKnobDrywet->setId(ZamDelayPlugin::paramDrywet);
    fKnobDrywet->setRange(0.0f, 1.0f);
    fKnobDrywet->setLabel(true);
    fKnobDrywet->setScrollStep(0.1f);
    fKnobDrywet->setDefault(0.5f);
    fKnobDrywet->setRotationAngle(240);
    fKnobDrywet->setCallback(this);

    fKnobFeedback = new ZamKnob(this, knobImage);
    fKnobFeedback->setAbsolutePos(332, 138);
    fKnobFeedback->setId(ZamDelayPlugin::paramFeedback);
    fKnobFeedback->setRange(0.0f, 1.0f);
    fKnobFeedback->setLabel(true);
    fKnobFeedback->setScrollStep(0.1f);
    fKnobFeedback->setDefault(0.0f);
    fKnobFeedback->setRotationAngle(240);
    fKnobFeedback->setCallback(this);

    fSliderDiv = new ImageSlider(this, triangleImage);
    fSliderDiv->setStartPos(140, 165);
    fSliderDiv->setEndPos(260, 165);
    fSliderDiv->setId(ZamDelayPlugin::paramDivisor);
    fSliderDiv->setRange(1.0f, 5.0f);
    fSliderDiv->setDefault(3.0f);
    fSliderDiv->setStep(1.0f);
    fSliderDiv->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamDelayUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamDelayPlugin::paramInvert:
        fToggleInvert->setDown(value > 0.5);
        break;
    case ZamDelayPlugin::paramDelaytime:
        fKnobDelaytime->setValue(value);
        break;
    case ZamDelayPlugin::paramSync:
        fToggleBPM->setDown(value > 0.5);
        break;
    case ZamDelayPlugin::paramLPF:
        fKnobLPF->setValue(value);
        break;
    case ZamDelayPlugin::paramGain:
        fKnobGain->setValue(value);
        break;
    case ZamDelayPlugin::paramDrywet:
        fKnobDrywet->setValue(value);
        break;
    case ZamDelayPlugin::paramFeedback:
        fKnobFeedback->setValue(value);
        break;
    case ZamDelayPlugin::paramDivisor:
        fSliderDiv->setValue(value);
        break;
    case ZamDelayPlugin::paramDelaytimeout:
        fKnobDelaytime->setValue(value);
        break;
    }
}

void ZamDelayUI::programLoaded(uint32_t index)
{
	switch(index) {
	case 0:
		fToggleInvert->setDown(false);
		fKnobDelaytime->setValue(160.0f);
		fToggleBPM->setDown(false);
		fKnobLPF->setValue(6000.0f);
		fKnobGain->setValue(0.0f);
		fKnobDrywet->setValue(0.5f);
		fKnobFeedback->setValue(0.0f);
		fSliderDiv->setValue(3.0f);
		break;
	}
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamDelayUI::imageSwitchClicked(ImageSwitch* tog, bool down)
{
	setParameterValue(tog->getId(), down ? 1.f : 0.f);
}

void ZamDelayUI::imageSliderDragStarted(ImageSlider* s)
{
    editParameter(s->getId(), true);
}

void ZamDelayUI::imageSliderDragFinished(ImageSlider* s)
{
    editParameter(s->getId(), false);
}

void ZamDelayUI::imageSliderValueChanged(ImageSlider* s, float value)
{
    setParameterValue(s->getId(), value);
}

void ZamDelayUI::imageKnobDragStarted(ZamKnob* knob)
{
    editParameter(knob->getId(), true);
}

void ZamDelayUI::imageKnobDragFinished(ZamKnob* knob)
{
    editParameter(knob->getId(), false);
}

void ZamDelayUI::imageKnobValueChanged(ZamKnob* knob, float value)
{
    setParameterValue(knob->getId(), value);
}

void ZamDelayUI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamDelayUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
