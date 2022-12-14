/*
 * ZamVerb
 * Copyright (C) 2017 Damien Zammit <damien@zamaudio.com>
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

#include "ZamVerbPlugin.hpp"
#include <stdio.h>
#include <assert.h>

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamVerbPlugin::ZamVerbPlugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    signal = false;
    swap = 0;
    active = 0;
    clv[0] = new LV2convolv();
    clv[0]->clv_configure("convolution.ir.preset", "0");
    clv[0]->clv_initialize(getSampleRate(), 2, 2, getBufferSize());

    clv[1] = new LV2convolv();
    clv[1]->clv_configure("convolution.ir.preset", "0");
    clv[1]->clv_initialize(getSampleRate(), 2, 2, getBufferSize());

    // Extra buffer for outputs since plugin can work in place
    tmpouts = (float **)malloc (2 * sizeof(float*));
    tmpouts[0] = (float *)calloc (1, 8192 * sizeof(float));
    tmpouts[1] = (float *)calloc (1, 8192 * sizeof(float));

    // Extra buffer for inputs since convolve might clobber ins
    tmpins = (float **)malloc (2 * sizeof(float*));
    tmpins[0] = (float *)calloc (1, 8192 * sizeof(float));
    tmpins[1] = (float *)calloc (1, 8192 * sizeof(float));

    // set default values
    loadProgram(0);
}

ZamVerbPlugin::~ZamVerbPlugin()
{
    free(tmpouts[0]);
    free(tmpouts[1]);
    free(tmpouts);

    free(tmpins[0]);
    free(tmpins[1]);
    free(tmpins);

    delete clv[0];
    delete clv[1];
}

// -----------------------------------------------------------------------
// Init

void ZamVerbPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramMaster:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Master level";
        parameter.symbol     = "master";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -30.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramWetdry:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Wet / Dry";
        parameter.symbol     = "wetdry";
        parameter.unit       = "%";
        parameter.ranges.def = 50.f;
        parameter.ranges.min = 0.f;
        parameter.ranges.max = 100.f;
        break;
    case paramRoom:
        parameter.hints      = kParameterIsInteger;
        parameter.name       = "Room";
        parameter.symbol     = "room";
        parameter.unit       = " ";
        parameter.ranges.def = 0.f;
        parameter.ranges.min = 0.f;
        parameter.ranges.max = 6.f;
        break;
    }
}

void ZamVerbPlugin::initProgramName(uint32_t index, String& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamVerbPlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramMaster:
        return master;
        break;
    case paramWetdry:
        return wetdry;
        break;
    case paramRoom:
        return room;
        break;
    default:
        return 0.0f;
    }
}

void ZamVerbPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramMaster:
        master = value;
        break;
    case paramWetdry:
        wetdry = value;
        break;
    case paramRoom:
        room = value;
        reload();
        break;
    }
}

void ZamVerbPlugin::loadProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    master = 0.0f;
    wetdry = 50.f;
    room = 0.f;

    activate();
}

// -----------------------------------------------------------------------
// Process

void ZamVerbPlugin::activate()
{
	reload();
	signal = true;
}

void ZamVerbPlugin::deactivate()
{
	signal = false;
}

void ZamVerbPlugin::reload()
{
	uint8_t other;
	char preset[2] = { 0 };

	snprintf(preset, 2, "%d", (int)room);
	other = !active;
	signal = false;
	clv[other]->clv_release();
	clv[other]->clv_configure("convolution.ir.preset", preset);
	clv[other]->clv_initialize(getSampleRate(), 2, 2, getBufferSize());
	swap = other;
	signal = true;
}

void ZamVerbPlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint32_t i;
	int nprocessed;
	active = swap;
	float _wetdry = 0.01f * wetdry;

	if (!signal) {
		memcpy(outputs[0], inputs[0], frames * sizeof(float));
		memcpy(outputs[1], inputs[1], frames * sizeof(float));
		return;
	}

	assert(frames < 8192);
	memcpy(tmpins[0], inputs[0], frames * sizeof(float));
	memcpy(tmpins[1], inputs[1], frames * sizeof(float));
	nprocessed = clv[active]->clv_convolve(tmpins, tmpouts, 2, 2, frames, from_dB(-16.f));
	if (nprocessed <= 0) {
		memcpy(outputs[0], inputs[0], frames * sizeof(float));
		memcpy(outputs[1], inputs[1], frames * sizeof(float));
	} else {
		for (i = 0; i < frames; i++) {
			outputs[0][i] = (_wetdry * tmpouts[0][i] + (1.f - _wetdry) * inputs[0][i]) * from_dB(master);
			outputs[1][i] = (_wetdry * tmpouts[1][i] + (1.f - _wetdry) * inputs[1][i]) * from_dB(master);
		}
	}
}

void ZamVerbPlugin::bufferSizeChanged(uint32_t)
{
	reload();
}

void ZamVerbPlugin::sampleRateChanged(double)
{
	reload();
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamVerbPlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
