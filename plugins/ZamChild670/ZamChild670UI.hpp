/*
 * ZamChild670 stereo tube compressor
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

#ifndef ZAMCHILD670UI_HPP_INCLUDED
#define ZAMCHILD670UI_HPP_INCLUDED

#include "DistrhoUI.hpp"

#include "ImageKnob.hpp"
#include "ZamChild670Artwork.hpp"

using DGL::Image;
using DGL::ImageKnob;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamChild670UI : public UI,
                    public ImageKnob::Callback
{
public:
    ZamChild670UI();

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
    ScopedPointer<ImageKnob> fKnobIngain, fKnobOutgain;
    ScopedPointer<ImageKnob> fKnobAC, fKnobDC;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMCHILD670UI_HPP_INCLUDED
