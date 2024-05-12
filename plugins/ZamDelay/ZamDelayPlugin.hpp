/*
 * ZamDelay mono delay effect
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
 */

#ifndef ZAMCOMPPLUGIN_HPP_INCLUDED
#define ZAMCOMPPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

// 8 seconds of delay at 96kHz
#define MAX_DELAY 768000
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define MIN(a,b) ((a) > (b) ? (b) : (a))
#define LIN_INTERP(f,a,b) ((a) + (f) * ((b) - (a)))

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
        paramFeedback,
        paramDelaytimeout,
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

    const char* getDescription() const noexcept override
    {
        return "A simple mono delay plugin";
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
        return d_version(4, 3, 0);
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
	        return (expf(0.05f*gdb*logf(10.f)));
	}

	static inline float
	to_dB(float g) {
	        return (20.f*log10f(g));
	}

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;
    void clearfilter(void);
    void lpfRbj(float fc, float srate);
    float runfilter(float in);

    // -------------------------------------------------------------------

private:
    float invert, feedb, delaytime, sync, lpf, divisor, gain, drywet, delaytimeout;
    float invertold, feedbold, delaytimeold, syncold, lpfold, divisorold, gainold, drywetold, delaytimeoutold, delaysamplesold;
    float z[MAX_DELAY];
    unsigned int posz;
    unsigned int tap[2];
    int active;
    int next;
    int age;
    float A0, A1, A2, A3, A4, A5, B0, B1, B2, B3, B4, B5;
    float state[4];
    float fbstate;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMCOMP_HPP_INCLUDED
