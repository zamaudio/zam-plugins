/*
 * ZamPiano physically modelled piano synth 
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

#ifndef ZAMPIANOPLUGIN_HPP_INCLUDED
#define ZAMPIANOPLUGIN_HPP_INCLUDED

#include <string.h>
#include "DistrhoPlugin.hpp"

#define STRIKE 0
#define ATTACK 1
#define SUSTAIN 2
#define RELEASE 3
#define SILENT 4

#define MAX_SAMP_FRAME 4096

#include "fft.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamPianoPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramGain,
        paramSpeed,
        paramGraph,
        paramCount
    };
    fft fftvars;
    ZamPianoPlugin();
    ~ZamPianoPlugin();
protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamPiano";
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

    int64_t d_getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'P', 'N', 'O');
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

    double f0(int n);
    double q0(int n);
    double p(int n);
    double alpha(int n);
    double a(int i, int n);
    double w(int i, double f);
    double mu(int n);
    double c(int n);
    double mhammer(int n);
    double lstring(int n);
    double mstring(int n);
    double fk1(double fk, double dt, double t, int n);
    void damp(int n, int *state, float *out, uint32_t frames);
    void e(int n, double vhammer, int *state, float *out, uint32_t frames);
    void dumpvals(void);

    void d_activate() override;
    void d_run(const float** inputs, float** outputs, uint32_t frames,
    		const MidiEvent* midievent, uint32_t midicount) override;
    //void d_setState(const char* key, const char* value) override;
    //void d_initStateKey(unsigned int key, d_string& val) override;
    // -------------------------------------------------------------------

	typedef struct {
		int state;
		double vel;
	} note_t;
	note_t note[128];

	float oldbuf[2][MAX_SAMP_FRAME];

	double ff0[128], ff1[128], integrala[128], 
		integralb[128], timepos[128];
private:
    float gain;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMPIANO_HPP_INCLUDED
