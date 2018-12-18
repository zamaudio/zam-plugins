/*
 * ZamSFZ polyphonic SFZ player
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
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#include "ZamSFZUI.hpp"
#include "ZamSFZPlugin.hpp"

#include "Window.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamSFZUI::ZamSFZUI()
    : UI()
{
    setSize(ZamSFZArtwork::zamsfzWidth, ZamSFZArtwork::zamsfzHeight);

    // background
    fImgBackground = Image(ZamSFZArtwork::zamsfzData, ZamSFZArtwork::zamsfzWidth, ZamSFZArtwork::zamsfzHeight, GL_BGR);

    // knob
    Image knobImage(ZamSFZArtwork::knobData, ZamSFZArtwork::knobWidth, ZamSFZArtwork::knobHeight);

    // button
    Image loadImage(ZamSFZArtwork::loadData, ZamSFZArtwork::loadWidth, ZamSFZArtwork::loadHeight);
    Image loadImagePress(ZamSFZArtwork::loadprData, ZamSFZArtwork::loadprWidth, ZamSFZArtwork::loadprHeight);
    Image loadingImage(ZamSFZArtwork::loadingData, ZamSFZArtwork::loadingWidth, ZamSFZArtwork::loadingHeight);
    Image emptyImage(ZamSFZArtwork::emptyData, ZamSFZArtwork::emptyWidth, ZamSFZArtwork::emptyHeight);

    // knob

    fKnobGain = new ZamKnob(this, knobImage);
    fKnobGain->setAbsolutePos(189, 24);
    fKnobGain->setRange(-30.f, 30.0f);
    fKnobGain->setDefault(0.0f);
    fKnobGain->setLabel(true);
    fKnobGain->setRotationAngle(240);
    fKnobGain->setCallback(this);

    fButtonLoad = new ImageButton(this, loadImage, loadImage, loadImagePress);
    fButtonLoad->setAbsolutePos(35,25);
    fButtonLoad->setCallback(this);

    fLoading = new ImageSwitch(this, emptyImage, loadingImage);
    fLoading->setAbsolutePos(35, 55);
    fLoading->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamSFZUI::parameterChanged(uint32_t index, float value)
{
	switch (index)
	{
	case ZamSFZPlugin::paramLoading:
		fLoading->setDown(value > 0.5);
		break;
	case ZamSFZPlugin::paramGain:
		fKnobGain->setValue(value);
		break;
	}
}

void ZamSFZUI::programLoaded(uint32_t index)
{
    if (index != 0)
        return;

    fKnobGain->setValue(0.0f);
    fLoading->setDown(false);
}

void ZamSFZUI::stateChanged(const char* key, const char*)
{
	if (strcmp(key, "filepath") == 0) {
		printf("state changed... do something?\n");
	}
}

void ZamSFZUI::uiFileBrowserSelected(const char* filename)
{
	// if a file was selected, tell DSP
	if (filename != nullptr)
		setState("filepath", filename);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamSFZUI::imageKnobDragStarted(ZamKnob* knob)
{
    if (knob == fKnobGain)
        editParameter(ZamSFZPlugin::paramGain, true);
}

void ZamSFZUI::imageKnobDragFinished(ZamKnob* knob)
{
    if (knob == fKnobGain)
        editParameter(ZamSFZPlugin::paramGain, false);
}

void ZamSFZUI::imageKnobValueChanged(ZamKnob* knob, float value)
{
    if (knob == fKnobGain)
        setParameterValue(ZamSFZPlugin::paramGain, value);
}

void ZamSFZUI::imageButtonClicked(ImageButton*, int)
{
    DGL::Window::FileBrowserOptions opts;
    opts.title = "Load SFZ";
    //opts.filters = "sfz;";

    getParentWindow().openFileBrowser(opts);
}

void ZamSFZUI::imageSwitchClicked(ImageSwitch*, bool state)
{
	// Hack: unset the toggle
	fLoading->setDown(!state);
}

void ZamSFZUI::onDisplay()
{
	fImgBackground.draw();
}
// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamSFZUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
