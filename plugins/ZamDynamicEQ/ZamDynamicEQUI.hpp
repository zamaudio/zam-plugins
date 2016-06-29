/*
 * ZamDynamicEQ
 * Copyright (C) 2016  Damien Zammit <damien@zamaudio.com>
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
 */

#ifndef ZAMDYNAMICEQUI_HPP_INCLUDED
#define ZAMDYNAMICEQUI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "../../widgets/ZamWidgets.hpp"

#include "ZamDynamicEQArtwork.hpp"

#define EQPOINTS 575
#include <complex>

using DGL::Image;
using DGL::ZamKnob;
using DGL::ZamSwitch;
using DGL::ImageSwitch;
using DGL::ImageSlider;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamDynamicEQUI : public UI,
                        public ZamKnob::Callback,
                        public ZamSwitch::Callback,
                        public ImageSwitch::Callback
{
public:
    ZamDynamicEQUI();

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
    void imageSwitchClicked(ZamSwitch* toggle, bool down) override;

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

	void calceqcurve(float x[], float y[]);
	void peq(double G0, double G, double GB, double w0, double Dw, double *a0, double *a1, double *a2, double *b0, double *b1, double *b2, double *gn);
	void highshelfeq(double, double G, double, double w0, double, double q, double B[], double A[]);
	void lowshelfeq(double, double G, double, double w0, double, double q, double B[], double A[]);
	

private:
    Image fImgBackground, fHighOnImg, fHighOffImg, fLowOnImg, fLowOffImg;
    Image fPeakOnImg, fPeakOffImg, fTogOffImg, fTogOnImg, fSliderEq;

    ScopedPointer<ZamSwitch> fToggleLow, fTogglePeak, fToggleHigh;
    ScopedPointer<ImageSwitch> fToggleSidechain, fToggleBoostCut;
    ScopedPointer<ZamKnob> fKnobAttack, fKnobRelease;
    ScopedPointer<ZamKnob> fKnobThresh, fKnobRatio, fKnobKnee, fKnobTargetWidth;
    ScopedPointer<ZamKnob> fKnobMax, fKnobSlew, fKnobTargetFreq, fKnobDetectFreq;

    DGL::Rectangle<int> fCanvasArea;
    float eqx[EQPOINTS], eqy[EQPOINTS];
    double a0x,a1x,a2x,b0x,b1x,b2x,gainx;
    double a0y,a1y,a2y,b0y,b1y,b2y,gainy;
    double Bl[3];
    double Al[3];
    double Bh[3];
    double Ah[3];
    float fControlGain;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif
