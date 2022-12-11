/*
 * ZamPhono
 * Copyright (C) 2016  Damien Zammit <damien@zamaudio.com>
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

#ifndef ZAMPHONOUI_HPP_INCLUDED
#define ZAMPHONOUI_HPP_INCLUDED

#include "DistrhoUI.hpp"

#include "ImageWidgets.hpp"
#include "ZamPhonoPlugin.hpp"

using DGL::Image;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamPhonoUI : public UI,
                 public ImageSlider::Callback,
                 public ImageSwitch::Callback
{
public:
    ZamPhonoUI();

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

    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(uint32_t index, float value) override;
    void programLoaded(uint32_t index) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageSliderDragStarted(ImageSlider* slider) override;
    void imageSliderDragFinished(ImageSlider* slider) override;
    void imageSliderValueChanged(ImageSlider* slider, float value) override;

    void imageSwitchClicked(ImageSwitch* toggle, bool down) override;

    void onDisplay() override;

private:
    Image fImgBackground;

    ScopedPointer<ImageSlider> fSliderNotch;
    ScopedPointer<ImageSwitch> fTogglePlaycut;

};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMPHONOUI_HPP_INCLUDED
