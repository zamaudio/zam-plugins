/*
 * ZamAutoSat mono compressor
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

#ifndef ZAMAUTOSATUI_HPP_INCLUDED
#define ZAMAUTOSATUI_HPP_INCLUDED

#include "DistrhoUI.hpp"

#include "Image.hpp"

#include "ZamAutoSatArtwork.hpp"
#include "ZamAutoSatPlugin.hpp"

using DGL::Image;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamAutoSatUI : public UI
{
public:
    ZamAutoSatUI();
    ~ZamAutoSatUI() override;

protected:
    // -------------------------------------------------------------------
    // Information

    unsigned int d_getWidth() const noexcept override
    {
        return ZamAutoSatArtwork::zamautosatWidth;
    }

    unsigned int d_getHeight() const noexcept override
    {
        return ZamAutoSatArtwork::zamautosatHeight;
    }

    // -------------------------------------------------------------------
    // DSP Callbacks

    void d_parameterChanged(uint32_t index, float value) override;
    void d_programChanged(uint32_t index) override;

    // -------------------------------------------------------------------

    void onDisplay() override;

private:
    Image fImgBackground;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // ZAMAUTOSATUI_HPP_INCLUDED
