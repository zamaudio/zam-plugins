/*
 * ZamVerb
 * Copyright (C) 2017 Damien Zammit <damien@zamaudio.com>
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

#ifndef ZAMVERBPLUGIN_HPP_INCLUDED
#define ZAMVERBPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "convolution.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamVerbPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramMaster = 0,
        paramWetdry,
        paramRoom,
        paramCount
    };

    ZamVerbPlugin();
    ~ZamVerbPlugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamVerb";
    }

    const char* getDescription() const noexcept override
    {
        return "Convolution Reverb effect";
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
        return d_cconst('Z', 'V', 'R', 'B');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) ;
    void initProgramName(uint32_t index, String& programName) ;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;
    void  loadProgram(uint32_t index) override;

    void reload();

    // -------------------------------------------------------------------
    // Process

	static inline float
	sanitize_denormal(float v) {
	        if(!std::isnormal(v) || !std::isfinite(v))
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
    void bufferSizeChanged(uint32_t newBufferSize) override;
    void sampleRateChanged(double newSampleRate) override;

    LV2convolv *clv[2];
    int8_t swap;
    int8_t active;
    bool signal;
    // -------------------------------------------------------------------

    float **tmpouts;
    float **tmpins;
private:
    float master,wetdry,room,room_old; //parameters
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMVERB_HPP_INCLUDED
