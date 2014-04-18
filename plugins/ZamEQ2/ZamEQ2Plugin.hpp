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

#ifndef ZAMCOMPPLUGIN_HPP_INCLUDED
#define ZAMCOMPPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

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
    ~ZamEQ2Plugin() override;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamEQ2";
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
        return d_cconst('Z', 'E', 'Q', '2');
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
    void d_deactivate() override;
    void d_run(float** inputs, float** outputs, uint32_t frames) override;

    void peq(double G0, double G, double GB, double w0, double Dw, 
		double *a0, double *a1, double *a2, 
		double *b0, double *b1, double *b2, double *gn);
    void lowshelfeq(double G0, double G, double GB, 
    		double w0, double Dw, double q, double B[], double A[]);
    void highshelfeq(double G0, double G, double GB, 
    		double w0, double Dw, double q, double B[], double A[]);
    // -------------------------------------------------------------------

private:
    float gain1,q1,freq1,gain2,q2,freq2,gainl,freql,gainh,freqh,master,togglepeaks; //parameters
        double x1,x2,y1,y2;
        double x1a,x2a,y1a,y2a;
        double zln1,zln2,zld1,zld2;
        double zhn1,zhn2,zhd1,zhd2;
        double a0x,a1x,a2x,b0x,b1x,b2x,gainx;
        double a0y,a1y,a2y,b0y,b1y,b2y,gainy;
        double Bl[3];
        double Al[3];
        double Bh[3];
        double Ah[3];
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMCOMP_HPP_INCLUDED
