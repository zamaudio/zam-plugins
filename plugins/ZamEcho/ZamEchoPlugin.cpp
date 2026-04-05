/*
 * ZamEcho mono echo effect
 * Copyright (C) 2026  Damien Zammit <damien@zamaudio.com>
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
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#include "ZamEchoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamEchoPlugin::ZamEchoPlugin()
    : Plugin(paramCount, 1, 0)
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamEchoPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramEcho:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Echo";
        parameter.symbol     = "echo";
        parameter.unit       = " ";
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    }
}


void ZamEchoPlugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "Zero";
		break;
	}
}

void ZamEchoPlugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		echo = 0.5f;
		break;
	}

	activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZamEchoPlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramEcho:
        return echo;
        break;
    default:
        return 0.0f;
    }
}

void ZamEchoPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramEcho:
        echo = value;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void ZamEchoPlugin::activate()
{
	int i;
	for (i = 0; i < MAX_DELAY; i++) {
		z[i] = 0.f;
	}
	posz = 0;
	tap = 0;
	fbstate = 0.f;
}

static inline float clamp(float sample)
{
	if (sample > 1.f)
		return 1.f;
	if (sample < -1.f)
		return -1.f;
	return sample;
}

void ZamEchoPlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint32_t i;
	float in;
	float srate = getSampleRate();
	TimePosition t = getTimePosition();
	int delaysamples;
	float bpm;
	float delayms = 300.0;

	if (t.bbt.valid) {
		bpm = t.bbt.beatsPerMinute;
		delayms = (float)t.bbt.beatType * 1000.f * 60.f / (bpm * 4.);
	}
	delaysamples = (int)(delayms * srate) / 1000;
	tap = delaysamples;

	for (i = 0; i < frames; i++) {
		int p;
		in = inputs[0][i];
		z[posz] = in + echo * fbstate;
		p = posz - tap;
		if (p<0) p += MAX_DELAY;
		fbstate = z[p];
		
		outputs[0][i] = clamp(z[posz]);
		if (++posz >= MAX_DELAY) {
			posz = 0;
		}
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamEchoPlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
