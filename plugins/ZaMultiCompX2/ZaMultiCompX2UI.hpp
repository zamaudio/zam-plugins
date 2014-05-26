/*
 * ZaMultiCompX2 stereo multiband compressor 
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

#ifndef ZAMULTICOMPUI_HPP_INCLUDED
#define ZAMULTICOMPUI_HPP_INCLUDED

#include "DistrhoUI.hpp"

#include "ImageKnob.hpp"
#include "ImageSwitch.hpp"

#include "ZaMultiCompX2Artwork.hpp"

#define COMPOINTS 1000
#define MAX_COMP 3

using DGL::Image;
using DGL::ImageKnob;
using DGL::ImageSwitch;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZaMultiCompX2UI : public UI,
                        public ImageKnob::Callback,
                        public ImageSwitch::Callback
{
public:
    ZaMultiCompX2UI();

protected:
    // -------------------------------------------------------------------
    // Information

    uint d_getWidth() const noexcept override
    {
        return ZaMultiCompX2Artwork::zamulticompx2Width;
    }

    uint d_getHeight() const noexcept override
    {
        return ZaMultiCompX2Artwork::zamulticompx2Height;
    }

    void compcurve(float in, int k, float* x, float* y);
    void calc_compcurves(void);

    // -------------------------------------------------------------------
    // DSP Callbacks

    void d_parameterChanged(uint32_t index, float value) override;
    void d_programChanged(uint32_t index) override;
    void d_stateChanged(const char*, const char*) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageKnobDragStarted(ImageKnob* knob) override;
    void imageKnobDragFinished(ImageKnob* knob) override;
    void imageKnobValueChanged(ImageKnob* knob, float value) override;

    void imageSwitchClicked(ImageSwitch* toggle, int button) override;

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
    ImageKnob* fKnobAttack;
    ImageKnob* fKnobRelease;
    ImageKnob* fKnobThresh1;
    ImageKnob* fKnobThresh2;
    ImageKnob* fKnobThresh3;
    ImageKnob* fKnobRatio;
    ImageKnob* fKnobKnee;
    ImageKnob* fKnobGlobalGain;
    ImageKnob* fKnobMakeup1;
    ImageKnob* fKnobMakeup2;
    ImageKnob* fKnobMakeup3;
    ImageKnob* fKnobXover1;
    ImageKnob* fKnobXover2;
    ImageSwitch* fSwitchBypass1;
    ImageSwitch* fSwitchBypass2;
    ImageSwitch* fSwitchBypass3;
    ImageSwitch* fSwitchListen1;
    ImageSwitch* fSwitchListen2;
    ImageSwitch* fSwitchListen3;
    ImageSwitch* fSwitchStereo;

    Image fLedRedImg;
    float fLedRedValue1;
    float fLedRedValue2;
    float fLedRedValue3;
    Image fLedYellowImg;
    float fLedYellowValueL;
    float fLedYellowValueR;
    DGL::Rectangle<int> fCanvasArea;
    float fThresh[MAX_COMP];
    float fRatio;
    float fKnee;
    float fMakeup[MAX_COMP];
    float fBypass[MAX_COMP];
    float fMaster;
    float compx[MAX_COMP][COMPOINTS];
    float compy[MAX_COMP][COMPOINTS];
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMULTICOMPUI_HPP_INCLUDED
