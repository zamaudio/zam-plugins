/*
 * ZamHeadX2 HRTF simulator 
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

#ifndef ZAMWIDTHX2PLUGIN_HPP_INCLUDED
#define ZAMWIDTHX2PLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "convolution.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamHeadX2Plugin : public Plugin
{
public:
    enum Parameters
    {
        paramAzimuth = 0,
        paramElevation,
        paramWidth,
        paramCount
    };

    ZamHeadX2Plugin();
    ~ZamHeadX2Plugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamHeadX2";
    }

    const char* getDescription() const noexcept override
    {
        return "HRTF acoustic filtering plugin for directional sound.";
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
        return d_version(4, 0, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'H', 'D', '2');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;
    void initProgramName(uint32_t index, String& programName) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;
    void  loadProgram(uint32_t index);

    void reload();

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
    void deactivate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;
    void pushsample(float* buf, float val, int i, uint32_t maxframes);
    float getsample(float* buf, int i, uint32_t maxframes);

    // -------------------------------------------------------------------

private:
    bool signal;
    float elevation, azimuth, width;
    int azold, elold;
    int swap, active;
    float **tmpins;
    float **tmpouts;
    LV2convolv *clv[2];
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMHEADX2_HPP_INCLUDED
