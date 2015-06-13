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

#ifndef ZAMGEQ31X2PLUGIN_HPP_INCLUDED
#define ZAMGEQ31X2PLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

#define MAX_FILT 31

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamGEQ31X2Plugin : public Plugin
{
public:
    enum Parameters
    {
        paramMaster = 0,
        paramLink,
        paramGain1L,
        paramGain2L,
        paramGain3L,
        paramGain4L,
        paramGain5L,
        paramGain6L,
        paramGain7L,
        paramGain8L,
        paramGain9L,
        paramGain10L,
        paramGain11L,
        paramGain12L,
        paramGain13L,
        paramGain14L,
        paramGain15L,
        paramGain16L,
        paramGain17L,
        paramGain18L,
        paramGain19L,
        paramGain20L,
        paramGain21L,
        paramGain22L,
        paramGain23L,
        paramGain24L,
        paramGain25L,
        paramGain26L,
        paramGain27L,
        paramGain28L,
        paramGain29L,
        paramGain30L,
        paramGain31L,
        paramGain1R,
        paramGain2R,
        paramGain3R,
        paramGain4R,
        paramGain5R,
        paramGain6R,
        paramGain7R,
        paramGain8R,
        paramGain9R,
        paramGain10R,
        paramGain11R,
        paramGain12R,
        paramGain13R,
        paramGain14R,
        paramGain15R,
        paramGain16R,
        paramGain17R,
        paramGain18R,
        paramGain19R,
        paramGain20R,
        paramGain21R,
        paramGain22R,
        paramGain23R,
        paramGain24R,
        paramGain25R,
        paramGain26R,
        paramGain27R,
        paramGain28R,
        paramGain29R,
        paramGain30R,
        paramGain31R,
        paramCount
    };

    ZamGEQ31X2Plugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamGEQ31X2";
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
        return 0x1001;
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'G', '3', '2');
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
    double run_filter(int i, int ch, double in);
        double x1[2][MAX_FILT], x2[2][MAX_FILT], y1[2][MAX_FILT], y2[2][MAX_FILT];
        double a1[2][MAX_FILT], a2[2][MAX_FILT];
	double b0[2][MAX_FILT], b1[2][MAX_FILT], b2[2][MAX_FILT];
    // -------------------------------------------------------------------

private:
    float gain[MAX_FILT][2], freq[MAX_FILT], bw[MAX_FILT], master, link; //parameters
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMGEQ31X2_HPP_INCLUDED
