/*
 * ZamEQ2 2 band parametric equaliser
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

#ifndef ZAMEQ2UI_HPP_INCLUDED
#define ZAMEQ2UI_HPP_INCLUDED

#include "DistrhoUI.hpp"

#include "ImageKnob.hpp"

#include "ZamEQ2Artwork.hpp"
#include "ZamEQ2Plugin.hpp"

using DGL::Image;
using DGL::ImageKnob;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamEQ2UI : public UI,
                  public ImageKnob::Callback
{
public:
    ZamEQ2UI();
    ~ZamEQ2UI() override;

protected:
    // -------------------------------------------------------------------
    // Information

    unsigned int d_getWidth() const noexcept override
    {
        return ZamEQ2Artwork::zameq2Width;
    }

    unsigned int d_getHeight() const noexcept override
    {
        return ZamEQ2Artwork::zameq2Height;
    }

    // -------------------------------------------------------------------
    // DSP Callbacks

    void d_parameterChanged(uint32_t index, float value) override;
    void d_programChanged(uint32_t index) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageKnobDragStarted(ImageKnob* knob) override;
    void imageKnobDragFinished(ImageKnob* knob) override;
    void imageKnobValueChanged(ImageKnob* knob, float value) override;

    void onDisplay() override;

private:
    Image fImgBackground;
    ImageKnob* fKnobGain1;
    ImageKnob* fKnobQ1;
    ImageKnob* fKnobFreq1;
    ImageKnob* fKnobGain2;
    ImageKnob* fKnobQ2;
    ImageKnob* fKnobFreq2;
    ImageKnob* fKnobGainL;
    ImageKnob* fKnobFreqL;
    ImageKnob* fKnobGainH;
    ImageKnob* fKnobFreqH;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMEQ2UI_HPP_INCLUDED
