/*
 * ZamChild670 stereo tube limiter
 * Copyright (C) 2015  Damien Zammit <damien@zamaudio.com>
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

#ifndef ZAMCHILD670PLUGIN_HPP_INCLUDED
#define ZAMCHILD670PLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "wavechild670.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamChild670Plugin : public Plugin
{
public:
    enum Parameters
    {
        paramInlevel = 0,
        paramAC,
        paramDC,
        paramTau,
        paramOutlevel,
        paramCount
    };

    ZamChild670Plugin();
    ~ZamChild670Plugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamChild670";
    }

    const char* d_getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* d_getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t d_getVersion() const noexcept override
    {
        return 0x1000;
    }

    int64_t d_getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'C', 'L', 'D');
    }

    // -------------------------------------------------------------------
    // Init

    void d_initParameter(uint32_t index, Parameter& parameter) override;
    void d_initProgramName(uint32_t index, d_string& programName) override;

    // -------------------------------------------------------------------
    // Internal data

    float d_getParameterValue(uint32_t index) const override;
    void  d_setParameterValue(uint32_t index, float value) override;
    void  d_setProgram(uint32_t index) override;

    // -------------------------------------------------------------------
    // Process

	static inline float
	sanitize_denormal(float v) {
	        if(!std::isnormal(v))
	                return 0.f;
	        return v;
	}

	static inline float
	from_dB(float gdb) {
	        return (exp(gdb/20.f*log(10.f)));
	}

	static inline float
	to_dB(float g) {
	        return (20.f*log10(g));
	}

    void d_activate() override;
    void d_run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------

private:
    Wavechild670Parameters *params;
    Wavechild670 *zamchild;
    float inputLevel, ACThreshold, timeConstantSelect, DCThreshold, outputGain;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMCOMPX2_HPP_INCLUDED
