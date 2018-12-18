/*
 * ZamAutoSat
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
 */

#ifndef ZAMAUTOSATPLUGIN_HPP_INCLUDED
#define ZAMAUTOSATPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamAutoSatPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramCount
    };

    ZamAutoSatPlugin();
    ~ZamAutoSatPlugin() override;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamAutoSat";
    }

    const char* getDescription() const noexcept override
    {
        return "An automatic saturation plugin, has been known to provide smooth levelling to live mic channels.\n\
You can apply this plugin generously without affecting the tone.";
    }

    const char* getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* getHomePage() const noexcept override
    {
        return "http://www.zamaudio.com";
    }

    const char* getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(3, 10, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'S', 'A', 'T');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    void activate() override;
    void deactivate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------

};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMAUTOSAT_HPP_INCLUDED
