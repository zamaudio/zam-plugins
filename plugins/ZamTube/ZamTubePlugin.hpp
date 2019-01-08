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
#define DANGER 1000.f

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
	Triode v;
	TubeStageCircuit ckt;
	T ci;
	T ck;
	T co;
	T e;
	T er;
	T rp;
	T rg;
	T ro;
	T ri;
	T rk;

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
        return d_version(3, 10, 0);
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
	        return (exp(gdb/20.f*log(10.f)));
	}

	static inline float
	to_dB(float g) {
	        return (20.f*log10(g));
	}

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------

private:
	float tubedrive,bass,middle,treble,tonestack,mastergain,insane; //parameters
	const float ts[25][7] = {
	//	R1	R2	R3	R4	C1	C2	C3
		{250e+3,1e+6,	25e+3,	56e+3,	250e-12,20e-9,	20e-9},
		{250e+3,250e+3,	25e+3,	100e+3,	250e-12,100e-9,	47e-9},
		{250e+3,250e+3,	10e+3,	100e+3,	120e-12,100e-9,	47e-9},
		{250e+3,250e+3,	4.8e+3,	100e+3,	250e-12,100e-9,	47e-9},
		{220e+3,1e+6,	22e+3,	33e+3,	470e-12,22e-9,	22e-9},
		{250e+3,1e+6,	25e+3,	56e+3,	500e-12,22e-9,	22e-9},
		{250e+3,1e+6,	25e+3,	33e+3,	270e-12,22e-9,	22e-9},
		{250e+3,1e+6,	25e+3,	33e+3,	500e-12,22e-9,	22e-9},
		{250e+3,250e+3,	25e+3,	56e+3,	250e-12,47e-9,	47e-9},
		{1e+6,	1e+6,	10e+3,	100e+3,	50e-12,	22e-9,	22e-9},
		{220e+3,220e+3,	220e+3,	100e+3,	470e-12,100e-9,	47e-9},
		{250e+3,1e+6,	25e+3,	47e+3,	470e-12,20e-9,	20e-9},
		{500e+3,1e+6,	10e+3,	47e+3,	470e-12,22e-9,	22e-9},
		{250e+3,250e+3,	20e+3,	68e+3,	270e-12,22e-9,	22e-9},
		{250e+3,250e+3,	10e+3,	100e+3,	270e-12,100e-9,	40e-9},
		{250e+3,250e+3,	10e+3,	41e+3,	240e-12,33e-9,	82e-9},
		{250e+3,1e+6,	25e+3,	32e+3,	470e-12,22e-9,	22e-9},
		{250e+3,250e+3,	10e+3,	100e+3,	100e-12,100e-9,	47e-9},
		{250e+3,1e+6,	33e+3,	51e+3,	220e-12,15e-9,	47e-9},
		{220e+3,1e+6,	22e+3,	68e+3,	470e-12,22e-9,	22e-9},
		{220e+3,220e+3,	10e+3,	100e+3,	220e-12,47e-9,	47e-9},
		{250e+3,250e+3,	25e+3,	100e+3,	250e-12,22e-9,	22e-9},
		{250e+3,250e+3,	10e+3,	100e+3,	250e-12,100e-9,	47e-9},
		{250e+3,250e+3,	25e+3,	130e+3,	250e-12,100e-9,	22e-9},
		{1e+6,	1e+6,	94e+3,	270e+3,	25e-12,	60e-9,	20e-9}
	};
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMTUBE_HPP_INCLUDED
