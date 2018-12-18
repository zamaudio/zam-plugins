/*
 * ZamEQ2 2 band parametric equaliser
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

#ifndef ZAMEQ2UI_HPP_INCLUDED
#define ZAMEQ2UI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "../../widgets/ZamWidgets.hpp"

#include "ZamEQ2Artwork.hpp"
#include "ZamEQ2Plugin.hpp"

#define EQPOINTS 1000
#include <complex>

using DGL::Image;
using DGL::ZamKnob;
using DGL::ImageSlider;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamEQ2UI : public UI,
                 public ZamKnob::Callback,
                 public ImageSlider::Callback
{
public:
    ZamEQ2UI();

protected:
    // -------------------------------------------------------------------

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

	float toIEC(float db) {
         float def = 0.0f; /* Meter deflection %age */

         if (db < -70.0f) {
                 def = 0.0f;
         } else if (db < -60.0f) {
                 def = (db + 70.0f) * 0.25f;
         } else if (db < -50.0f) {
                 def = (db + 60.0f) * 0.5f + 5.0f;
         } else if (db < -40.0f) {
                 def = (db + 50.0f) * 0.75f + 7.5;
         } else if (db < -30.0f) {
                 def = (db + 40.0f) * 1.5f + 15.0f;
         } else if (db < -20.0f) {
                 def = (db + 30.0f) * 2.0f + 30.0f;
         } else if (db < 0.0f) {
                 def = (db + 20.0f) * 2.5f + 50.0f;
         } else {
                 def = 100.0f;
         }

         return (def * 2.0f);
        }

	void calceqcurve(float x[], float y[]);
    void peq(double G0, double G, double GB, double w0, double Dw,
		double *a0, double *a1, double *a2,
		double *b0, double *b1, double *b2, double *gn);
    void lowshelfeq(double G0, double G, double GB,
    		double w0, double Dw, double q, double B[], double A[]);
    void highshelfeq(double G0, double G, double GB,
    		double w0, double Dw, double q, double B[], double A[]);

    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(uint32_t index, float value) override;
    void programLoaded(uint32_t index) override;

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
    ScopedPointer<ZamKnob> fKnobGain1;
    ScopedPointer<ZamKnob> fKnobQ1, fKnobFreq1, fKnobGain2;
    ScopedPointer<ZamKnob> fKnobQ2, fKnobFreq2;
    ScopedPointer<ZamKnob> fKnobGainL, fKnobFreqL;
    ScopedPointer<ZamKnob> fKnobGainH, fKnobFreqH;
    ScopedPointer<ImageSlider> fSliderMaster;
    float eqx[EQPOINTS];
    float eqy[EQPOINTS];
    DGL::Rectangle<int> fCanvasArea;
        double a0x,a1x,a2x,b0x,b1x,b2x,gainx;
        double a0y,a1y,a2y,b0y,b1y,b2y,gainy;
        double Bl[3];
        double Al[3];
        double Bh[3];
        double Ah[3];
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMEQ2UI_HPP_INCLUDED
