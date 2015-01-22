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
#include "PianoNote.hpp"

#define STRIKE 0
#define ATTACK 1
#define SUSTAIN 2
#define RELEASE 1000
#define DECAY 1001
#define SILENT 1011

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamPianoPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramFreq,
	paramGain,
        paramGate,
	paramBright,
	paramDetuning,
	paramHammerHard,
	paramStiffness,
	paramReverbGain,
        paramReverbRoom,
	paramPanAngle,
	paramSpatialWidth,
        paramCount
    };
    ZamPianoPlugin();
    ~ZamPianoPlugin();

    PianoNote N[88];

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
        return d_cconst('Z', 'P', 'N', '2');
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

    void d_activate() override;
    void d_run(const float** inputs, float** outputs, uint32_t frames,
    		const MidiEvent* midievent, uint32_t midicount) override;
    // -------------------------------------------------------------------

	typedef struct {
		int state;
		double vel;
		int gatestate;
	} note_t;
	note_t note[128];

	float *intermed[88][2];
private:
        float pfreq;
        float pgain;
        float pgate;
        float pbright;
        float pdetune;
        float phammer;
        float pstiff;
        float prevgain;
        float prevroom;
        float ppanangle;
        float pspatialwidth;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMPIANO_HPP_INCLUDED
