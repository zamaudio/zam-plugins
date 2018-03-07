/*
 * ZamGrains Granular Delay
 * Copyright (C) 2018  Damien Zammit <damien@zamaudio.com>
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
 */

#ifndef ZAMGRAINSPLUGIN_HPP_INCLUDED
#define ZAMGRAINSPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

// 1 second of delay at 192kHz
#define MAX_DELAY 192000

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamGrainsPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramGain = 0,
        paramGrains,
        paramGrainspeed,
        paramPlayspeed,
        paramDelaytime,
        paramGrainpos,
        paramPlaypos,
        paramFinalpos,
        paramCount
    };

    ZamGrainsPlugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamGrains";
    }

    const char* getDescription() const noexcept override
    {
        return "A granular delay plugin";
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
        return d_cconst('Z', 'M', 'G', 'R');
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

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;
    float sample_and_hold(int ctrl, float input, int *state);
    float hanning(int pos, int windowsize);

    // -------------------------------------------------------------------

private:
    int currgrains, zidx, zidx2, zidxold, zidx2old, samphold, samphold2;
    float grains, grainspeed, playspeed, delaytime, gain, delaytimeout, playpos, grainpos, finalpos;
    float delaytimeold;
    float z[MAX_DELAY];
    unsigned int posz;
    unsigned int posphasor;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMGRAINS_HPP_INCLUDED
