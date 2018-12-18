/*
 * ZamGrains Granular Delay
 * Copyright (C) 2018  Damien Zammit <damien@zamaudio.com>
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

#ifndef ZAMGRAINSUI_HPP_INCLUDED
#define ZAMGRAINSUI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "../../widgets/ZamWidgets.hpp"

#include "ZamGrainsArtwork.hpp"

using DGL::Image;
using DGL::ImageSwitch;
using DGL::ZamKnob;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamGrainsUI : public UI,
                    public ZamKnob::Callback,
                    public ImageSwitch::Callback
{
public:
    ZamGrainsUI();

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
    void imageSwitchClicked(ImageSwitch* toggle, bool down) override;

    void onDisplay() override;

private:
    float playpos, grainpos, finalpos;
    Image fImgBackground;
    ScopedPointer<ZamKnob> fKnobPlayspeed, fKnobGrainspeed, fKnobMaster, fKnobGrains, fKnobLooptime;
    ScopedPointer<ImageSwitch> fToggleFreeze;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif
