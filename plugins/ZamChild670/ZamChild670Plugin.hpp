/*
 * ZamChild670 stereo tube limiter
 * Copyright (C) 2015  Damien Zammit <damien@zamaudio.com>
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

    const char* getLabel() const noexcept override
    {
        return "ZamChild670";
    }

    const char* getDescription() const noexcept override
    {
        return "";
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
        return d_version(3, 8, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'C', 'L', 'D');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;
    void initProgramName(uint32_t index, String& programName) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;
    void  loadProgram(uint32_t index) override;

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
	        return (expf(0.05f*gdb*logf(10.f)));
	}

	static inline float
	to_dB(float g) {
	        return (20.f*log10f(g));
	}

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------

private:
    Wavechild670Parameters *params;
    Wavechild670 *zamchild;
    float inputLevel, ACThreshold, timeConstantSelect, DCThreshold, outputGain;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMCOMPX2_HPP_INCLUDED
