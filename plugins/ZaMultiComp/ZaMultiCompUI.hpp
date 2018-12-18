/*
 * ZaMultiComp multiband compressor
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

#ifndef ZAMULTICOMPUI_HPP_INCLUDED
#define ZAMULTICOMPUI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "../../widgets/ZamWidgets.hpp"

#include "ZaMultiCompArtwork.hpp"

#define COMPOINTS 1000
#define MAX_COMP 3

using DGL::Image;
using DGL::ZamKnob;
using DGL::ImageSwitch;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZaMultiCompUI : public UI,
                        public ZamKnob::Callback,
                        public ImageSwitch::Callback
{
public:
    ZaMultiCompUI();

protected:
    // -------------------------------------------------------------------

    void compcurve(float in, int k, float* x, float* y);
    void compdot(float in, int k, float* x, float* y);
    void calc_compcurves(void);

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

    void imageSwitchClicked(ImageSwitch* toggle, bool down) override;

    void onDisplay() override;

inline double
to_dB(double g) {
        return (20.*log10(g));
}

inline double
from_dB(double gdb) {
        return (exp(gdb/20.*log(10.)));
}

inline double
sanitize_denormal(double value) {
        if (!std::isnormal(value)) {
                return (0.);
        }
        return value;
}

private:
    Image fImgBackground;
    ScopedPointer<ZamKnob> fKnobAttack1, fKnobAttack2, fKnobAttack3; 
    ScopedPointer<ZamKnob> fKnobRelease1, fKnobRelease2, fKnobRelease3;
    ScopedPointer<ZamKnob> fKnobThresh1, fKnobThresh2, fKnobThresh3;
    ScopedPointer<ZamKnob> fKnobRatio1;
    ScopedPointer<ZamKnob> fKnobRatio2;
    ScopedPointer<ZamKnob> fKnobRatio3;
    ScopedPointer<ZamKnob> fKnobKnee1;
    ScopedPointer<ZamKnob> fKnobKnee2;
    ScopedPointer<ZamKnob> fKnobKnee3;
    ScopedPointer<ZamKnob> fKnobGlobalGain;
    ScopedPointer<ZamKnob> fKnobMakeup1, fKnobMakeup2, fKnobMakeup3;
    ScopedPointer<ZamKnob> fKnobXover1, fKnobXover2;
    ScopedPointer<ImageSwitch> fToggleBypass1, fToggleBypass2, fToggleBypass3;
    ScopedPointer<ImageSwitch> fToggleListen1, fToggleListen2, fToggleListen3;

    Image fLedRedImg;
    float fLedRedValue[3];
    Image fLedYellowImg;
    float fLedYellowValueL;
    DGL::Rectangle<int> fCanvasArea;
    float fThresh[MAX_COMP];
    float fListen[MAX_COMP];
    float fRatio[MAX_COMP];
    float fKnee[MAX_COMP];
    float fMakeup[MAX_COMP];
    float fBypass[MAX_COMP];
    float fMaster;
    float compx[MAX_COMP][COMPOINTS];
    float compy[MAX_COMP][COMPOINTS];
    float dotx[MAX_COMP];
    float doty[MAX_COMP];
    float outlevel[3];
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMULTICOMPUI_HPP_INCLUDED
