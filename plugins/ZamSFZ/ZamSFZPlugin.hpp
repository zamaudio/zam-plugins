/*
 * ZamSFZ polyphonic SFZ player
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

#ifndef ZAMSFZPLUGIN_HPP_INCLUDED
#define ZAMSFZPLUGIN_HPP_INCLUDED

#include <string.h>
#include "DistrhoPlugin.hpp"
#include "Sfz.hpp"

#define MAX_VOICES 128
#define MAX_ENV (MAX_SAMPLES / 10)

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamSFZPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramGain,
        paramSpeed,
        paramCount
    };

    ZamSFZPlugin();

    Sfz sfz;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamSFZ";
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
        return d_cconst('Z', 'S', 'F', 'Z');
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

    float wavetable(int i, int note);
    void d_activate() override;
    void d_run(const float** inputs, float** outputs, uint32_t frames,
    		const MidiEvent* midievent, uint32_t midicount) override;
    void d_setState(const char* key, const char* value) override;
    void d_initState(unsigned int, d_string&, d_string&) override;
    // -------------------------------------------------------------------

private:
    std::string filename, path;
    bool loading;
    float gain, graph, speed;
    int nvoices;
    float* wave_y[128];
    float env_y[MAX_ENV];
	typedef struct v {
		bool playing;
		int notenum;
		int envpos;
		int slowcount;
		float curamp;
		float vi;
		int rampstate;
		int maxsamples;
	} Voice;
    Voice voice[128];
    Voice* curvoice;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMSFZ_HPP_INCLUDED
