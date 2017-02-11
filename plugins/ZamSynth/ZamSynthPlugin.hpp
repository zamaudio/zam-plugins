/*
 * ZamSynth polyphonic synthesiser
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

#ifndef ZAMSYNTHPLUGIN_HPP_INCLUDED
#define ZAMSYNTHPLUGIN_HPP_INCLUDED

#include <string.h>
#include "DistrhoPlugin.hpp"
#define MAX_VOICES 128
#define AREAHEIGHT 250
#define MAX_ENV AREAHEIGHT

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamSynthPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramGain,
        paramSpeed,
        paramGraph,
        paramCount
    };

    ZamSynthPlugin();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "ZamSynth";
    }

    const char* getDescription() const noexcept override
    {
        return "";
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
        return d_version(3, 8, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'S', 'T', 'H');
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

    float wavetable(float in);
    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames,
    		const MidiEvent* midievent, uint32_t midicount) override;
    void setState(const char* key, const char* value) override;
    void initStateKey(unsigned int key, String& val) override;
    // -------------------------------------------------------------------

private:
    float gain, graph, speed;
    int nvoices;
    float wave_y[AREAHEIGHT];
    float env_y[MAX_ENV];
	typedef struct v {
		bool playing;
		int notenum;
		int envpos;
		int slowcount;
		float curamp;
		float vi;
		float rampstate;
	} Voice;
    Voice voice[128];
    Voice* curvoice;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMSYNTH_HPP_INCLUDED
