/*
 * ZaMaximX2 stereo maximiser
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

#ifndef ZAMAXIMX2PLUGIN_HPP_INCLUDED
#define ZAMAXIMX2PLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

#define MAX_DELAY 480
#define MAX_AVG 120

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

    const char* getDescription() const noexcept override
    {
        return "Improved smooth peak limiter modelled from a DAFX paper";
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
	        return (expf(0.05f*gdb*logf(10.f)));
	}

	static inline float
	to_dB(float g) {
	        return (20.f*log10f(g));
	}

    void activate() override;
    void deactivate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;
    double normalise(double in);
    double clip(double in);
    double avgall(double in[]);
    double maxsample(double in[]);
    void pushsample(double in[], double sample, int *pos, int maxsamples);
    double getoldsample(double in[], int pos);

    // -------------------------------------------------------------------

private:
    float release,ceiling,thresdb,gainred,outlevel;//parameters
    int pose[2], posz[2], posc[2];
    double cn[2][MAX_DELAY];
    double emaxn[2][MAX_AVG];
    double z[2][MAX_DELAY];
    double emax_old[2];
    double e_old[2];
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMAXIMX2_HPP_INCLUDED
