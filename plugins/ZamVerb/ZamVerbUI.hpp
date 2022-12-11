/*
 * ZamVerb
 * Copyright (C) 2017 Damien Zammit <damien@zamaudio.com>
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
 */

#ifndef ZAMVERBUI_HPP_INCLUDED
#define ZAMVERBUI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "../../widgets/ZamWidgets.hpp"

using DGL::Image;
using DGL::ZamKnob;
using DGL::ImageSlider;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamVerbUI : public UI,
                  public ZamKnob::Callback,
                  public ImageSlider::Callback
{
public:
    ZamVerbUI();

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(uint32_t index, float value) override;
    void programLoaded(uint32_t index) override;
    void stateChanged(const char*, const char*) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageKnobDragStarted(ZamKnob* knob) override;
    void imageKnobDragFinished(ZamKnob* knob) override;
    void imageKnobValueChanged(ZamKnob* knob, float value) override;

    void imageSliderDragStarted(ImageSlider* slider) override;
    void imageSliderDragFinished(ImageSlider* slider) override;
    void imageSliderValueChanged(ImageSlider* slider, float value) override;

    void onDisplay() override;

private:
    Image fImgBackground;
    ScopedPointer<ImageSlider> fSliderNotch;
    ScopedPointer<ZamKnob> fKnobMaster, fKnobWetdry;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMVERBUI_HPP_INCLUDED
