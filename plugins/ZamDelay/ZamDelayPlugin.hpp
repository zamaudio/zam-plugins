/*
 * ZamDelay mono compressor
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

#ifndef ZAMCOMPPLUGIN_HPP_INCLUDED
#define ZAMCOMPPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

#define MAX_DELAY 288000
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define MIN(a,b) ((a) > (b) ? (b) : (a))

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamDelayPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramInvert = 0,
        paramDelaytime,
        paramSync,
        paramLPF,
        paramDivisor,
        paramGain,
        paramDrywet,
        paramCount
    };

    ZamDelayPlugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamDelay";
    }

    const char* getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t getVersion() const noexcept override
    {
        return 0x1300;
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'M', 'D', 'L');
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
    void pushsample(float in, float dline[], int *pos, int *a, int max);
    float getsample(float in, float dline[], int pos, int age, int max);

    // -------------------------------------------------------------------

private:
    float invert, delaytime, sync, lpf, divisor, gain, drywet;
    float z[MAX_DELAY];
    int posz, age;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMCOMP_HPP_INCLUDED
