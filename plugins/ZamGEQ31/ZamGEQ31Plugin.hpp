/*
 * ZamGEQ31 31 band graphic equaliser
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
        paramQ = 0,
        paramMaster,
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
        paramGain30,
        paramGain31,
        paramCount
    };

    ZamGEQ31Plugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamGEQ31";
    }

    const char* d_getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* d_getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t d_getVersion() const noexcept override
    {
        return 0x1000;
    }

    long d_getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'G', '3', '1');
    }

    // -------------------------------------------------------------------
    // Init

    void d_initParameter(uint32_t index, Parameter& parameter) ;
    void d_initProgramName(uint32_t index, d_string& programName) ;

    // -------------------------------------------------------------------
    // Internal data

    float d_getParameterValue(uint32_t index) const override;
    void  d_setParameterValue(uint32_t index, float value) override;
    void  d_setProgram(uint32_t index) ;

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
	        return (exp(gdb/20.f*log(10.f)));
	}

	static inline double
	to_dB(double g) {
	        return (20.f*log10(g));
	}

	static inline int
	sign(double x) {
        	return (x >= 0.f ? 1 : -1);
	}

    void d_activate() override;
    void d_run(const float** inputs, float** outputs, uint32_t frames) override;

    void geq31(int i, int ch, float srate, float fc, float q);
    double run_filter(int i, int ch, double in);
        double x1[1][MAX_FILT], x2[1][MAX_FILT], y1[1][MAX_FILT], y2[1][MAX_FILT];
        double a[1][MAX_FILT], b[1][MAX_FILT], g[1][MAX_FILT];
    // -------------------------------------------------------------------

private:
    float gain[MAX_FILT], freq[MAX_FILT], master, q; //parameters
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMGEQ31_HPP_INCLUDED
