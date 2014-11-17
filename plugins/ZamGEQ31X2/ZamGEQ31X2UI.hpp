/*
 * ZamGEQ31  31 band graphic equaliser
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

#ifndef ZAMGEQ31UI_HPP_INCLUDED
#define ZAMGEQ31UI_HPP_INCLUDED

#include "DistrhoUI.hpp"

#include "ImageKnob.hpp"
#include "ImageSlider.hpp"

#include "ZamGEQ31X2Artwork.hpp"
#include "ZamGEQ31X2Plugin.hpp"

using DGL::Image;
using DGL::ImageSlider;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamGEQ31X2UI : public UI,
                 public ImageSlider::Callback
{
public:
    ZamGEQ31X2UI();

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

    void d_parameterChanged(uint32_t index, float value) override;
    void d_programChanged(uint32_t index) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageSliderDragStarted(ImageSlider* slider) override;
    void imageSliderDragFinished(ImageSlider* slider) override;
    void imageSliderValueChanged(ImageSlider* slider, float value) override;

    void onDisplay() override;

private:
    Image fImgBackground;
    ScopedPointer<ImageSlider> fSliderMaster;
    ScopedPointer<ImageSlider> fSliderGain1L;
    ScopedPointer<ImageSlider> fSliderGain2L;
    ScopedPointer<ImageSlider> fSliderGain3L;
    ScopedPointer<ImageSlider> fSliderGain4L;
    ScopedPointer<ImageSlider> fSliderGain5L;
    ScopedPointer<ImageSlider> fSliderGain6L;
    ScopedPointer<ImageSlider> fSliderGain7L;
    ScopedPointer<ImageSlider> fSliderGain8L;
    ScopedPointer<ImageSlider> fSliderGain9L;
    ScopedPointer<ImageSlider> fSliderGain10L;
    ScopedPointer<ImageSlider> fSliderGain11L;
    ScopedPointer<ImageSlider> fSliderGain12L;
    ScopedPointer<ImageSlider> fSliderGain13L;
    ScopedPointer<ImageSlider> fSliderGain14L;
    ScopedPointer<ImageSlider> fSliderGain15L;
    ScopedPointer<ImageSlider> fSliderGain16L;
    ScopedPointer<ImageSlider> fSliderGain17L;
    ScopedPointer<ImageSlider> fSliderGain18L;
    ScopedPointer<ImageSlider> fSliderGain19L;
    ScopedPointer<ImageSlider> fSliderGain20L;
    ScopedPointer<ImageSlider> fSliderGain21L;
    ScopedPointer<ImageSlider> fSliderGain22L;
    ScopedPointer<ImageSlider> fSliderGain23L;
    ScopedPointer<ImageSlider> fSliderGain24L;
    ScopedPointer<ImageSlider> fSliderGain25L;
    ScopedPointer<ImageSlider> fSliderGain26L;
    ScopedPointer<ImageSlider> fSliderGain27L;
    ScopedPointer<ImageSlider> fSliderGain28L;
    ScopedPointer<ImageSlider> fSliderGain29L;
    ScopedPointer<ImageSlider> fSliderGain30L;
    ScopedPointer<ImageSlider> fSliderGain31L;
    ScopedPointer<ImageSlider> fSliderGain1R;
    ScopedPointer<ImageSlider> fSliderGain2R;
    ScopedPointer<ImageSlider> fSliderGain3R;
    ScopedPointer<ImageSlider> fSliderGain4R;
    ScopedPointer<ImageSlider> fSliderGain5R;
    ScopedPointer<ImageSlider> fSliderGain6R;
    ScopedPointer<ImageSlider> fSliderGain7R;
    ScopedPointer<ImageSlider> fSliderGain8R;
    ScopedPointer<ImageSlider> fSliderGain9R;
    ScopedPointer<ImageSlider> fSliderGain10R;
    ScopedPointer<ImageSlider> fSliderGain11R;
    ScopedPointer<ImageSlider> fSliderGain12R;
    ScopedPointer<ImageSlider> fSliderGain13R;
    ScopedPointer<ImageSlider> fSliderGain14R;
    ScopedPointer<ImageSlider> fSliderGain15R;
    ScopedPointer<ImageSlider> fSliderGain16R;
    ScopedPointer<ImageSlider> fSliderGain17R;
    ScopedPointer<ImageSlider> fSliderGain18R;
    ScopedPointer<ImageSlider> fSliderGain19R;
    ScopedPointer<ImageSlider> fSliderGain20R;
    ScopedPointer<ImageSlider> fSliderGain21R;
    ScopedPointer<ImageSlider> fSliderGain22R;
    ScopedPointer<ImageSlider> fSliderGain23R;
    ScopedPointer<ImageSlider> fSliderGain24R;
    ScopedPointer<ImageSlider> fSliderGain25R;
    ScopedPointer<ImageSlider> fSliderGain26R;
    ScopedPointer<ImageSlider> fSliderGain27R;
    ScopedPointer<ImageSlider> fSliderGain28R;
    ScopedPointer<ImageSlider> fSliderGain29R;
    ScopedPointer<ImageSlider> fSliderGain30R;
    ScopedPointer<ImageSlider> fSliderGain31R;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMGEQ31UI_HPP_INCLUDED
