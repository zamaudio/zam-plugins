/*
 * ZamEcho mono echo effect
 * Copyright (C) 2026  Damien Zammit <damien@zamaudio.com>
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

#include "ZamEchoPlugin.hpp"
#include "ZamEchoUI.hpp"

using DGL::Point;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamEchoUI::ZamEchoUI()
    : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT)
{
    //fNanoText.loadSharedResources();
    //font = fNanoText.findFont(NANOVG_DEJAVU_SANS_TTF);

    // background
    fImgBackground = Image(ZamEchoArtwork::zamechoData, ZamEchoArtwork::zamechoWidth, ZamEchoArtwork::zamechoHeight);

    // knob
    Image knobImage(ZamEchoArtwork::knobData, ZamEchoArtwork::knobWidth, ZamEchoArtwork::knobHeight);

    // knob
    fKnobEcho = new ZamKnob(this, knobImage);
    fKnobEcho->setAbsolutePos(70, 82);
    fKnobEcho->setId(ZamEchoPlugin::paramEcho);
    fKnobEcho->setRange(0.0f, 1.0f);
    fKnobEcho->setLabel(true);
    fKnobEcho->setScrollStep(0.1f);
    fKnobEcho->setDefault(0.5f);
    fKnobEcho->setRotationAngle(240);
    fKnobEcho->setCallback(this);

    // set default values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamEchoUI::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case ZamEchoPlugin::paramEcho:
        fKnobEcho->setValue(value);
        break;
    }
}

void ZamEchoUI::programLoaded(uint32_t index)
{
	switch(index) {
	case 0:
		fKnobEcho->setValue(0.5f);
		break;
	}
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamEchoUI::imageKnobDragStarted(ZamKnob* knob)
{
    editParameter(knob->getId(), true);
}

void ZamEchoUI::imageKnobDragFinished(ZamKnob* knob)
{
    editParameter(knob->getId(), false);
}

void ZamEchoUI::imageKnobValueChanged(ZamKnob* knob, float value)
{
    setParameterValue(knob->getId(), value);
}

void ZamEchoUI::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());
    fImgBackground.draw(context);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new ZamEchoUI();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
