/*
 * ZamGEQ31 31 band graphic equaliser
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

#ifndef ZAMGEQ31PLUGIN_HPP_INCLUDED
#define ZAMGEQ31PLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

#define MAX_FILT 31

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamGEQ31Plugin : public Plugin
{
public:
    enum Parameters
    {
        paramMaster = 0,
        paramGain1,
        paramGain2,
        paramGain3,
        paramGain4,
        paramGain5,
        paramGain6,
        paramGain7,
        paramGain8,
        paramGain9,
        paramGain10,
        paramGain11,
        paramGain12,
        paramGain13,
        paramGain14,
        paramGain15,
        paramGain16,
        paramGain17,
        paramGain18,
        paramGain19,
        paramGain20,
        paramGain21,
        paramGain22,
        paramGain23,
        paramGain24,
        paramGain25,
        paramGain26,
        paramGain27,
        paramGain28,
        paramGain29,
        paramCount
    };

    ZamGEQ31Plugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamGEQ31";
    }

    const char* getDescription() const noexcept override
    {
        return "31 band graphic equaliser, good for eq of live spaces, removing unwanted noise from a track etc.";
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
        return d_version(4, 1, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'G', '3', '1');
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

	static inline double
	sanitize_denormal(double v) {
	        if(!std::isnormal(v))
	                return 0.f;
	        return v;
	}

	static inline double
	from_dB(double gdb) {
	        return (exp(0.05*gdb*log(10.0)));
	}

	static inline double
	to_dB(double g) {
	        return (20.0*log10(g));
	}

	static inline int
	sign(double x) {
        	return (x >= 0.f ? 1 : -1);
	}

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    void geq(int i, float srate, float g);
    double run_filter(int i, double in);
        double omegaL[29], omegaU[29], k[29], v[29], a[29], cm[29][21], a0m[29][21];
	int m[29];
	double w11[29][21], w12[29][21], w21[29][21], w22[29][21];
    // -------------------------------------------------------------------

private:
    float gain[29], gainold[29], master; //parameters
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMGEQ31_HPP_INCLUDED
