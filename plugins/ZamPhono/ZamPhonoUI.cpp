/*
 * ZamPhono
 * Copyright (C) 2016  Damien Zammit <damien@zamaudio.com>
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
 */

#include "ZamPhonoUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamPhonoUI::ZamPhonoUI()
    : UI()
{
    setSize(ZamPhonoArtwork::zamphonoWidth, ZamPhonoArtwork::zamphonoHeight);

    // background
    fImgBackground = Image(ZamPhonoArtwork::zamphonoData, ZamPhonoArtwork::zamphonoWidth, ZamPhonoArtwork::zamphonoHeight, GL_BGR);

    // notch slider
    Image sliderImage(ZamPhonoArtwork::sliderData, ZamPhonoArtwork::sliderWidth, ZamPhonoArtwork::sliderHeight);

    // toggle play/cut
    Image playImage(ZamPhonoArtwork::playData, ZamPhonoArtwork::playWidth, ZamPhonoArtwork::playHeight);
    Image cutImage(ZamPhonoArtwork::cutData, ZamPhonoArtwork::cutWidth, ZamPhonoArtwork::cutHeight);

    Point<int> notchPosStart(255, 73);
    Point<int> notchPosEnd(255, 73+52);

    fSliderNotch = new ImageSlider(this, sliderImage);
    fSliderNotch->setStartPos(notchPosStart);
    fSliderNotch->setEndPos(notchPosEnd);
    fSliderNotch->setRange(0.f, 4.f);
    fSliderNotch->setStep(1.f);
    fSliderNotch->setValue(3.f);
    fSliderNotch->setCallback(this);

    Point<int> playcutStart(10, 168);

    fTogglePlaycut = new ImageSwitch(this, playImage, cutImage);
    fTogglePlaycut->setAbsolutePos(playcutStart);
    fTogglePlaycut->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamPhonoUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamPhonoPlugin::paramType:
	fSliderNotch->setValue(value);
	break;
    case ZamPhonoPlugin::paramToggle:
	fTogglePlaycut->setDown(value > 0.5);
	break;
    }
}

void ZamPhonoUI::programLoaded(uint32_t index)
{
	switch(index) {
	default:
	case 0:
		fSliderNotch->setValue(3.f);
		fTogglePlaycut->setDown(false);
		break;
	}
}

void ZamPhonoUI::imageSliderDragStarted(ImageSlider*)
{
    editParameter(ZamPhonoPlugin::paramType, true);
}

void ZamPhonoUI::imageSliderDragFinished(ImageSlider*)
{
    editParameter(ZamPhonoPlugin::paramType, false);
}

void ZamPhonoUI::imageSliderValueChanged(ImageSlider*, float value)
{
    setParameterValue(ZamPhonoPlugin::paramType, value);
}

void ZamPhonoUI::imageSwitchClicked(ImageSwitch* toggle, bool down)
{
    float v = down ? 1.f : 0.f;
    if (toggle == fTogglePlaycut) {
        setParameterValue(ZamPhonoPlugin::paramToggle, v);
    }
}

void ZamPhonoUI::onDisplay()
{
    fImgBackground.draw();
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamPhonoUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
