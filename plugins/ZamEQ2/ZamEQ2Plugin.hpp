/*
 * ZamEQ2 2 band parametric equaliser
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

#ifndef ZAMEQ2PLUGIN_HPP_INCLUDED
#define ZAMEQ2PLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

#define MAX_FILT 4

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamEQ2Plugin : public Plugin
{
public:
    enum Parameters
    {
        paramGain1 = 0,
        paramQ1,
        paramFreq1,
        paramGain2,
        paramQ2,
        paramFreq2,
        paramGainL,
        paramFreqL,
        paramGainH,
        paramFreqH,
        paramMaster,
        paramTogglePeaks,
        paramCount
    };

    ZamEQ2Plugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamEQ2";
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
        return d_version(3, 6, 1);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'E', 'Q', '2');
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

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    void peq(int i, int ch, float srate, float fc, float g, float bw);
    void lowshelf(int i, int ch, float srate, float fc, float g);
    void highshelf(int i, int ch, float srate, float fc, float g);
    float run_filter(int i, int ch, double in);
        double x1[1][MAX_FILT], x2[1][MAX_FILT], y1[1][MAX_FILT], y2[1][MAX_FILT];
        double b0[1][MAX_FILT], b1[1][MAX_FILT], b2[1][MAX_FILT];
        double a1[1][MAX_FILT], a2[1][MAX_FILT];
    // -------------------------------------------------------------------

private:
    float gain1,q1,freq1,gain2,q2,freq2,gainl,freql,gainh,freqh,master,togglepeaks; //parameters
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMEQ2_HPP_INCLUDED
