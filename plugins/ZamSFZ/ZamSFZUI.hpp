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

#ifndef ZAMSFZUI_HPP_INCLUDED
#define ZAMSFZUI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageKnob.hpp"
#include "ImageButton.hpp"
#include "ZamSFZArtwork.hpp"

using DGL::Image;
using DGL::ImageKnob;
using DGL::ImageButton;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamSFZUI : public UI,
                   public ImageKnob::Callback,
		   public ImageButton::Callback
{
public:
    ZamSFZUI();

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void d_parameterChanged(uint32_t index, float value) override;
    void d_programChanged(uint32_t index) override;
    void d_stateChanged(const char* key, const char* value) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageKnobDragStarted(ImageKnob* knob) override;
    void imageKnobDragFinished(ImageKnob* knob) override;
    void imageKnobValueChanged(ImageKnob* knob, float value) override;
    void imageButtonClicked(ImageButton*, int) override;

    void onDisplay() override;
private:
    Image fImgBackground;
    ScopedPointer<ImageKnob> fKnobGain;
    ScopedPointer<ImageButton> fButtonLoad;
    char* filepath;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMSFZUI_HPP_INCLUDED
