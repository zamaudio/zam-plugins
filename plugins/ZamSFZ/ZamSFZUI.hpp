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

#ifndef ZAMSFZUI_HPP_INCLUDED
#define ZAMSFZUI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "ZamSFZArtwork.hpp"
#include "../../widgets/ZamWidgets.hpp"

using DGL::Image;
using DGL::ZamKnob;
using DGL::ImageButton;
using DGL::ImageSwitch;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamSFZUI : public UI,
                   public ZamKnob::Callback,
		   public ImageButton::Callback,
		   public ImageSwitch::Callback
{
public:
    ZamSFZUI();

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(uint32_t index, float value) override;
    void programLoaded(uint32_t index) override;
    void stateChanged(const char* key, const char* value) override;

    void uiFileBrowserSelected(const char* filename) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageKnobDragStarted(ZamKnob* knob) override;
    void imageKnobDragFinished(ZamKnob* knob) override;
    void imageKnobValueChanged(ZamKnob* knob, float value) override;
    void imageButtonClicked(ImageButton*, int) override;
    void imageSwitchClicked(ImageSwitch*, bool) override;

    void onDisplay() override;

private:
    Image fImgBackground;
    ScopedPointer<ZamKnob> fKnobGain;
    ScopedPointer<ImageButton> fButtonLoad;
    ScopedPointer<ImageSwitch> fLoading;
    char* filepath;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMSFZUI_HPP_INCLUDED
