/*
 * ZamTube triode WDF distortion model
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

#ifndef ZAMTUBEPLUGIN_HPP_INCLUDED
#define ZAMTUBEPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "triode.h"
#include "wdfcircuits.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

template <int N> inline float faustpower(float x) 		{ return powf(x,N); }
template <int N> inline double faustpower(double x) 	{ return pow(x,N); }
template <int N> inline int faustpower(int x) 			{ return faustpower<N/2>(x) * faustpower<N-N/2>(x); }
template <> 	 inline int faustpower<0>(int)	 		{ return 1; }
template <> 	 inline int faustpower<1>(int x) 		{ return x; }

#define FAUSTFLOAT float
typedef long double quad;

#define TOLERANCE 1e-6
#define DANGER 180.f

#define R1	0
#define R2	1
#define R3	2
#define R4	3
#define C1	4
#define C2	5
#define C3	6

class ZamTubePlugin : public Plugin
{
public:
	TubeStageCircuit ckt;
	T ci[2];
	T ck[2];
	T co[2];
	T e[2];
	T er[2];
	T rg[2];
	T ro[2];
	T rk[2];

        float   fConst0;
        float   fConst1;
        float   fConst2;
        float   fRec0[4];

	float fSamplingFreq;
    enum Parameters
    {
        paramTubedrive = 0,
        paramBass,
        paramMiddle,
        paramTreble,
        paramToneStack,
        paramGain,
	paramInsane,
        paramCount
    };

    ZamTubePlugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamTube";
    }

    const char* getDescription() const noexcept override
    {
        return "Wicked distortion effect.\n\
Wave digital filter physical model of a triode tube amplifier stage, with modelled tone stacks from real guitar amplifiers (thanks D. Yeh et al).";
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
        return d_version(3, 12, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'T', 'U', 'B');
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

    // -------------------------------------------------------------------

private:
	float tubedrive,bass,middle,treble,tonestack,mastergain,insane,insaneold; //parameters

	float ts[25][7];

};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMTUBE_HPP_INCLUDED
