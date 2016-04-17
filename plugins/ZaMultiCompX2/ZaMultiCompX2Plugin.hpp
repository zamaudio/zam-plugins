/*
 * ZaMultiCompX2 stereo multiband compressor
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

#ifndef ZAMULTICOMPX2PLUGIN_HPP_INCLUDED
#define ZAMULTICOMPX2PLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include <algorithm>

START_NAMESPACE_DISTRHO

#define MAX_FILT 2
#define MAX_COMP 3
#define MAX_SAMPLES 480
#define STEREOLINK_MAX 1
#define STEREOLINK_AVERAGE 0
#define DANGER 100000.f
#define EPS 1e-20f

// -----------------------------------------------------------------------

class ZaMultiCompX2Plugin : public Plugin
{
public:
    enum Parameters
    {
        paramAttack1 = 0,
        paramAttack2,
        paramAttack3,
        paramRelease1,
        paramRelease2,
        paramRelease3,
        paramKnee1,
        paramKnee2,
        paramKnee3,
        paramRatio1,
        paramRatio2,
        paramRatio3,
        paramThresh1,
        paramThresh2,
        paramThresh3,

        paramMakeup1,
        paramMakeup2,
        paramMakeup3,

        paramXover1,
        paramXover2,

	paramGainR1,
	paramGainR2,
	paramGainR3,

	paramToggle1,
	paramToggle2,
	paramToggle3,

	paramListen1,
	paramListen2,
	paramListen3,

	paramStereoDet,
	paramGlobalGain,
	paramOutputLevelL,
	paramOutputLevelR,
	paramOutputLevelLow,
	paramOutputLevelMed,
	paramOutputLevelHigh,
        paramCount
    };

    enum States
    {
        stateReadMeter,
	stateCount
    };

    ZaMultiCompX2Plugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZaMultiCompX2";
    }

    const char* getDescription() const noexcept override
    {
        return "Flagship of zam-plugins:\n\
Stereo version of ZaMultiComp, with individual threshold controls for each band and real-time visualisation of comp curves.";
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
        return 0x1700;
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'M', 'M', '2');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) ;
    void initProgramName(uint32_t index, String& programName) ;
    void initState(uint32_t, String&, String&) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;
    void loadProgram(uint32_t index) override;
    String getState(const char* key) const override;
    void setState(const char* key, const char* value) override;

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

    void run_comp(int k, float inL, float inR, float *outL, float *outR);
    void run_limit(float inL, float inR, float *outL, float *outR);
    void run_lr4(int i, int ch, float in, float *outlo, float *outhi);
    void calc_lr4(float f, int i, int ch);

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

	void pushsample(float samples[], float sample, int k);
	float averageabs(float samples[]);
    // -------------------------------------------------------------------

private:
    float attack[MAX_COMP],release[MAX_COMP],knee[MAX_COMP],ratio[MAX_COMP],thresdb[MAX_COMP],makeup[MAX_COMP],globalgain,stereodet;
    float gainr[MAX_COMP],toggle[MAX_COMP],listen[MAX_COMP],maxL,maxR,outl,outr,xover1,xover2;
    float old_yl[2][MAX_COMP], old_y1[2][MAX_COMP], old_yg[2][MAX_COMP];
    float old_ll[2], old_l1[2];
    float outlevel[3];
    int pos[3];
    float outlevelold[3][MAX_SAMPLES];
    float oldxover1, oldxover2;
    bool resetl;
    bool resetr;
    // Crossover filter coefficients
    float c1[2][MAX_FILT];
    float c2[2][MAX_FILT];
    float c3[2][MAX_FILT];
    float c4[2][MAX_FILT];
    float gl[2][MAX_FILT];
    float gh[2][MAX_FILT];

    //Crossover filter states
    float z1[2][MAX_FILT];
    float z2[2][MAX_FILT];
    float z3[2][MAX_FILT];
    float z4[2][MAX_FILT];
    float z5[2][MAX_FILT];
    float z6[2][MAX_FILT];
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif
