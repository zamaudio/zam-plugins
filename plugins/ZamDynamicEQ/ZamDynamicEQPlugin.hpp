/*
 * ZamDynamicEQ
 * Copyright (C) 2016  Damien Zammit <damien@zamaudio.com>
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

#ifndef ZAMDYNAMICEQPLUGIN_HPP_INCLUDED
#define ZAMDYNAMICEQPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include <algorithm>

START_NAMESPACE_DISTRHO

#define MAX_SAMPLES 12

// -----------------------------------------------------------------------

class ZamDynamicEQPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramAttack = 0,
        paramRelease,
        paramKnee,
        paramRatio,
        paramThresh,
        paramMaxBoostCut,
        paramSlew,
        paramSidechain,
        paramToggleLow,
        paramTogglePeak,
        paramToggleHigh,
        paramDetectFreq,
        paramTargetFreq,
        paramTargetWidth,
        paramBoostCut,
        paramControlGain,
        paramCount
    };

    ZamDynamicEQPlugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamDynamicEQ";
    }

    const char* getDescription() const noexcept override
    {
        return "Mono dynamic EQ";
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
        return d_version(3, 11, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'M', 'D', 'Q');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) ;
    void initProgramName(uint32_t index, String& programName) ;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;
    void loadProgram(uint32_t index) override;

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

    void peq(double G0, double G, double GB, double w0, double Dw,
             double *a0, double *a1, double *a2, double *b0, double *b1, double *b2, double *gn);
    void highshelfeq(double, double G, double, double w0, double, double q, double B[], double A[]);
    void lowshelfeq(double, double G, double, double w0, double, double q, double B[], double A[]);

    void run_comp(double input, double* output);
    // -------------------------------------------------------------------

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;
    void initAudioPort(bool input, uint32_t index, AudioPort& port) override;

	void run_highshelf(double input, double* output);
	void run_lowshelf(double input, double* output);
	void run_peq1(double input, double* output);
	void run_peq2(double input, double* output);

    // -------------------------------------------------------------------

private:

	void reset_low(void);
	void reset_high(void);
	void reset_peak(void);
	
	struct linear_svf {
		float g, k;
		float a[3];
		float m[3];

		float s[2][2];
	};

	struct linear_svf bandpass;

	void linear_svf_set_params(struct linear_svf *self,
			float sample_rate, float cutoff, float resonance);
	void linear_svf_reset(struct linear_svf *self);
	float run_linear_svf(struct linear_svf *self, int c, float in);

    float attack,release,knee,ratio,thresdb,max,slewfactor,sidechain, toglow, togpeak, toghigh, detectfreq, targetfreq, targetwidth, boostcut, controlgain; //parameters
    float oldL_yl, oldL_y1, oldL_yg;

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

#endif
