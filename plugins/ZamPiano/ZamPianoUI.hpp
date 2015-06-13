/*
 * ZamPiano polyphonic synthesiser
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

#ifndef ZAMPIANOUI_HPP_INCLUDED
#define ZAMPIANOUI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageKnob.hpp"
#include "ZamPianoArtwork.hpp"

using DGL::Image;
using DGL::ImageKnob;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamPianoUI : public UI,
                   public ImageKnob::Callback
{
public:
    ZamPianoUI();

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(uint32_t index, float value) override;
    void programLoaded(uint32_t index) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageKnobDragStarted(ImageKnob* knob) override;
    void imageKnobDragFinished(ImageKnob* knob) override;
    void imageKnobValueChanged(ImageKnob* knob, float value) override;

    void onDisplay() override;
private:
    Image fImgBackground;
    ScopedPointer<ImageKnob> fKnobBright;
    ScopedPointer<ImageKnob> fKnobDetune;
    ScopedPointer<ImageKnob> fKnobHammer;
    ScopedPointer<ImageKnob> fKnobStiff;
    ScopedPointer<ImageKnob> fKnobReverbGain;
    ScopedPointer<ImageKnob> fKnobReverbRoom;
    ScopedPointer<ImageKnob> fKnobPan;
    ScopedPointer<ImageKnob> fKnobWidth;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMPIANOUI_HPP_INCLUDED
