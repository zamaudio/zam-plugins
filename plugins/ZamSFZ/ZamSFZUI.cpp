/*
 * ZamSFZ polyphonic SFZ player
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

#include "ZamSFZUI.hpp"
#include "ZamSFZPlugin.hpp"

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

    // knob

    fKnobGain = new ImageKnob(this, knobImage);
    fKnobGain->setAbsolutePos(189, 24);
    fKnobGain->setRange(-30.f, 30.0f);
    fKnobGain->setDefault(0.0f);
    fKnobGain->setRotationAngle(240);
    fKnobGain->setCallback(this);

    fButtonLoad = new ImageButton(this, loadImage, loadImage, loadImagePress);
    fButtonLoad->setAbsolutePos(35,25);
    fButtonLoad->setCallback(this);

    // set default values
    d_programChanged(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void ZamSFZUI::d_parameterChanged(uint32_t index, float value)
{
	switch (index)
	{
	case ZamSFZPlugin::paramGain:
		fKnobGain->setValue(value);
		break;
	}
}

void ZamSFZUI::d_programChanged(uint32_t index)
{
    if (index != 0)
        return;

    fKnobGain->setValue(0.0f);
}

void ZamSFZUI::d_stateChanged(const char* key, const char* value)
{
	if (strcmp(key, "filepath") == 0) {
		printf("do something?\n");
	}
}

// -----------------------------------------------------------------------
// Widget Callbacks

void ZamSFZUI::imageKnobDragStarted(ImageKnob* knob)
{
    if (knob == fKnobGain)
        d_editParameter(ZamSFZPlugin::paramGain, true);
}

void ZamSFZUI::imageKnobDragFinished(ImageKnob* knob)
{
    if (knob == fKnobGain)
        d_editParameter(ZamSFZPlugin::paramGain, false);
}

void ZamSFZUI::imageKnobValueChanged(ImageKnob* knob, float value)
{
    if (knob == fKnobGain)
        d_setParameterValue(ZamSFZPlugin::paramGain, value);
}

void ZamSFZUI::imageButtonClicked(ImageButton*, int)
{
/*
	GtkWidget* dialog = gtk_file_chooser_dialog_new(
		"Load SFZ",
		NULL,
		GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);
	
	if (filepath) {
		gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filepath);
	}

	if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_ACCEPT) {
		gtk_widget_destroy(dialog);
		return;
	}
	
	if (filepath) {
		g_free(filepath);
	}
	filepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	
	gtk_widget_destroy(dialog);
*/	
	d_setState("filepath", "/home/damien/Music/rhodes-nord/rhodes-nord-wav.sfz");
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
