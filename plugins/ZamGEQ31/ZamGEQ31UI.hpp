/*
 * ZamGEQ31  31 band graphic equaliser
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

#ifndef ZAMGEQ31UI_HPP_INCLUDED
#define ZAMGEQ31UI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"
#include "../../widgets/ZamWidgets.hpp"

#include "ZamGEQ31Artwork.hpp"
#include "ZamGEQ31Plugin.hpp"

using DGL::Image;
using DGL::ZamKnob;
using DGL::ImageSlider;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamGEQ31UI : public UI,
                 public ZamKnob::Callback,
                 public ImageSlider::Callback
{
public:
    ZamGEQ31UI();

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
    ScopedPointer<ZamKnob> fKnobMaster;
    ScopedPointer<ZamKnob> fKnobQ;
    ScopedPointer<ImageSlider> fSliderGain1;
    ScopedPointer<ImageSlider> fSliderGain2;
    ScopedPointer<ImageSlider> fSliderGain3;
    ScopedPointer<ImageSlider> fSliderGain4;
    ScopedPointer<ImageSlider> fSliderGain5;
    ScopedPointer<ImageSlider> fSliderGain6;
    ScopedPointer<ImageSlider> fSliderGain7;
    ScopedPointer<ImageSlider> fSliderGain8;
    ScopedPointer<ImageSlider> fSliderGain9;
    ScopedPointer<ImageSlider> fSliderGain10;
    ScopedPointer<ImageSlider> fSliderGain11;
    ScopedPointer<ImageSlider> fSliderGain12;
    ScopedPointer<ImageSlider> fSliderGain13;
    ScopedPointer<ImageSlider> fSliderGain14;
    ScopedPointer<ImageSlider> fSliderGain15;
    ScopedPointer<ImageSlider> fSliderGain16;
    ScopedPointer<ImageSlider> fSliderGain17;
    ScopedPointer<ImageSlider> fSliderGain18;
    ScopedPointer<ImageSlider> fSliderGain19;
    ScopedPointer<ImageSlider> fSliderGain20;
    ScopedPointer<ImageSlider> fSliderGain21;
    ScopedPointer<ImageSlider> fSliderGain22;
    ScopedPointer<ImageSlider> fSliderGain23;
    ScopedPointer<ImageSlider> fSliderGain24;
    ScopedPointer<ImageSlider> fSliderGain25;
    ScopedPointer<ImageSlider> fSliderGain26;
    ScopedPointer<ImageSlider> fSliderGain27;
    ScopedPointer<ImageSlider> fSliderGain28;
    ScopedPointer<ImageSlider> fSliderGain29;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMGEQ31UI_HPP_INCLUDED
