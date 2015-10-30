/*
 * ZaMaximX2 stereo maximiser
 * Copyright (C) 2015  Damien Zammit <damien@zamaudio.com>
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

#ifndef ZAMAXIMX2PLUGIN_HPP_INCLUDED
#define ZAMAXIMX2PLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

#define MAX_SAMPLES 64
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define MIN(a,b) ((a) > (b) ? (b) : (a))


START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZaMaximX2Plugin : public Plugin
{
public:
    enum Parameters
    {
        paramRelease = 0,
        paramCeiling,
        paramThresh,
        paramGainRed,
        paramOutputLevel,
        paramCount
    };

    ZaMaximX2Plugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZaMaximX2";
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
        return 0x1000;
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'M', 'X', '2');
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
    void deactivate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;
    float normalise(float in, float gainr);
    float clip(float in);
    float avgall(float in[]);
    float maxsample(float in[]);
    void pushsample(float in[], float sample, int *pos);
    void pushall(float in[], float sample, int *pos);
    float getoldsample(float in[], int pos);

    // -------------------------------------------------------------------

private:
    float release,ceiling,thresdb,gainred,outlevel;//parameters
    int pose[2], posz[2], posc[2];
    float cn[2][MAX_SAMPLES];
    float emaxn[2][MAX_SAMPLES];
    float z[2][MAX_SAMPLES];
    float emax_old[2];
    float eavg_old[2];
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMAXIMX2_HPP_INCLUDED
