/*
 * ZamNoise  Noise detection and removal plugin 
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

#ifndef ZAMNOISEPLUGIN_HPP_INCLUDED
#define ZAMNOISEPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "Denoise.hpp"

typedef struct {
	unsigned long cbsize; // size of circular buffer
	float* cbi;
} CircularBuffer;


START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamNoisePlugin : public Plugin
{
public:
    enum Parameters
    {
        paramNoiseToggle,
        paramCount
    };

    ZamNoisePlugin();
    ~ZamNoisePlugin() override;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamNoise";
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
        return d_cconst('Z', 'N', 'O', 'I');
    }

    // -------------------------------------------------------------------
    // Init

    void d_initParameter(uint32_t index, Parameter& parameter) ;
    void d_initProgramName(uint32_t index, d_string& programName) ;

    // -------------------------------------------------------------------
    // Internal data

    float d_getParameterValue(uint32_t index) const ;
    void  d_setParameterValue(uint32_t index, float value) ;
    void  d_setProgram(uint32_t index) ;

    // -------------------------------------------------------------------
    void d_activate() ;
    void d_deactivate() ;
    void d_run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------
	float noisetoggle;

	void InstantiateCircularBuffer(CircularBuffer* buffer, unsigned long SampleRate);
	inline void IncrementPointer(CircularBuffer& buffer);	
	void init(float srate);
	int noverlap;
	CircularBuffer buffer;
	Denoise* zamnoise;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMNOISE_HPP_INCLUDED
