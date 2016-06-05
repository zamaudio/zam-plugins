/*
 * ZamDelay mono delay effect
 * Copyright (C) 2015  Damien Zammit <damien@zamaudio.com>
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

#ifndef ZAMCOMPUI_HPP_INCLUDED
#define ZAMCOMPUI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "../../widgets/ZamWidgets.hpp"

#include "ZamDelayArtwork.hpp"

using DGL::Image;
using DGL::ZamKnob;
using DGL::ImageSwitch;
using DGL::ImageSlider;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamDelayUI : public UI,
                    public ZamKnob::Callback,
                    public ImageSwitch::Callback,
                    public ImageSlider::Callback
{
public:
    ZamDelayUI();

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(uint32_t index, float value) override;
    void programLoaded(uint32_t index) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageKnobDragStarted(ZamKnob* knob) override;
    void imageKnobDragFinished(ZamKnob* knob) override;
    void imageKnobValueChanged(ZamKnob* knob, float value) override;

    void imageSwitchClicked(ImageSwitch* tog, bool down) override;

    void imageSliderDragStarted(ImageSlider* s) override;
    void imageSliderDragFinished(ImageSlider* s) override;
    void imageSliderValueChanged(ImageSlider* s, float value) override;

    void onDisplay() override;

private:
    Image fImgBackground;
    ScopedPointer<ZamKnob> fKnobDelaytime, fKnobLPF, fKnobGain, fKnobDrywet, fKnobFeedback;
    ScopedPointer<ImageSwitch> fToggleInvert, fToggleBPM;
    ScopedPointer<ImageSlider> fSliderDiv;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif
