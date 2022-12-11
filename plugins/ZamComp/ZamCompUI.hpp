/*
 * ZamComp stereo compressor
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

#ifndef ZAMCOMPUI_HPP_INCLUDED
#define ZAMCOMPUI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "../../widgets/ZamWidgets.hpp"

using DGL::Image;
using DGL::ZamKnob;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamCompUI : public UI,
                    public ZamKnob::Callback,
                    public ImageSwitch::Callback
{
public:
    ZamCompUI();

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

    void onDisplay() override;

private:
    Image fImgBackground;
    ScopedPointer<ZamKnob> fKnobAttack, fKnobRelease, fKnobThresh;
    ScopedPointer<ZamKnob> fKnobRatio, fKnobKnee, fKnobMakeup, fKnobSlew;
    ScopedPointer<ImageSwitch> fToggleSidechain;

    Image fLedRedImg;
    float fLedRedValue;
    Image fLedYellowImg;
    float fLedYellowValue;
    Image fTogOn;
    Image fTogOff;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMCOMPUI_HPP_INCLUDED
