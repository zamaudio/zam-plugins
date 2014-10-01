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

#include "ZamNoisePlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamNoisePlugin::ZamNoisePlugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{

    ZamNoisePlugin::init(d_getSampleRate());
    zamnoise = new Denoise(d_getSampleRate());

    // set default values
    d_setProgram(0);

    // reset
    d_deactivate();
}

ZamNoisePlugin::~ZamNoisePlugin()
{
	free(buffer.cbi);
	delete zamnoise;
}

// -----------------------------------------------------------------------
// Init

void ZamNoisePlugin::d_initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramNoiseToggle:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Noise Capture";
        parameter.symbol     = "noisecapture";
        parameter.unit       = "";
        parameter.ranges.def = 0.f;
        parameter.ranges.min = 0.f;
        parameter.ranges.max = 1.f;
        break;
    default:
	break;
    }
}

void ZamNoisePlugin::d_initProgramName(uint32_t index, d_string& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamNoisePlugin::d_getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramNoiseToggle:
        return noisetoggle;
        break;
    default:
	return 0.0f;
        break;
    }
}

void ZamNoisePlugin::d_setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramNoiseToggle:
        if (value == 1.f) {
		memset(buffer.cbi, 0, buffer.cbsize*sizeof(float));
	}
	noisetoggle = value;
        break;
    }
}

void ZamNoisePlugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    noisetoggle = 0.f;

    d_activate();
}


void ZamNoisePlugin::InstantiateCircularBuffer(CircularBuffer* buffer, unsigned long SampleRate) {
	buffer->cbsize = 16384;
	buffer->cbi = (float*) calloc(buffer->cbsize, sizeof(float));
}

void ZamNoisePlugin::init (float fsamp)
{
        InstantiateCircularBuffer(&buffer,fsamp);
        unsigned long N=buffer.cbsize;
        noverlap = 2;
}


// -----------------------------------------------------------------------
// Process

void ZamNoisePlugin::d_activate()
{
}

void ZamNoisePlugin::d_deactivate()
{
}

void ZamNoisePlugin::d_run(const float** inputs, float** outputs, uint32_t frames)
{
	zamnoise->process(inputs[0], outputs[0], buffer.cbi, frames, (int)noisetoggle);
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
	return new ZamNoisePlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
