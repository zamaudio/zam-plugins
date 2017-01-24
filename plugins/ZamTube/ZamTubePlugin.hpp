/*
 * ZamTube triode WDF distortion model
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

class ZamTubePlugin : public Plugin
{
public:
	Triode v;
	TubeStageCircuit ckt;
	T e;
	T ci;
	T ck;
	T co;
	T ro;
	T rp;
	T rg;
	T ri;
	T rk;

        float   fConst0;
        float   fConst1;
        float   fConst2;
        float   fConst3;
        float   fRec0[4];
        float   fRec1[4];
        float   fRec2[4];
        float   fRec3[4];
        float   fRec4[4];
        float   fRec5[4];
        float   fRec6[4];
        float   fRec7[4];
        float   fRec8[4];
        float   fRec9[4];
        float   fRec10[4];
        float   fRec11[4];
        float   fRec12[4];
        float   fRec13[4];
        float   fConst4;
        float   fConst5;
        float   fRec14[4];
        float   fConst6;
        float   fRec15[4];
        float   fRec16[4];
        float   fRec17[4];
        float   fRec18[4];
        float   fRec19[4];
        float   fRec20[4];
        float   fRec21[4];
        float   fRec22[4];
        float   fRec23[4];
        float   fRec24[4];

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
        return d_version(3, 7, 0);
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
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMTUBE_HPP_INCLUDED
