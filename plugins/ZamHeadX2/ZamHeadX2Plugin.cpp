/*
 * ZamHeadX2 HRTF simulator
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com>
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

#include "ZamHeadX2Plugin.hpp"
#include <assert.h>

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamHeadX2Plugin::ZamHeadX2Plugin()
    : Plugin(paramCount, 1, 1)
{
    signal = false;
    swap = 0;
    clv[swap] = new LV2convolv();
    clv[swap]->clv_configure("convolution.ir.preset", "0", "0");
    clv[swap]->clv_initialize(getSampleRate(), 2, 2, getBufferSize());

    clv[1] = new LV2convolv();
    clv[1]->clv_configure("convolution.ir.preset", "0", "0");
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

ZamHeadX2Plugin::~ZamHeadX2Plugin()
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

void ZamHeadX2Plugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramAzimuth:
        parameter.name       = "Azimuth";
        parameter.symbol     = "az";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -90.0f;
        parameter.ranges.max = 270.0f;
        break;
    case paramElevation:
        parameter.name       = "Elevation";
        parameter.symbol     = "elev";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 90.0f;
        break;
    case paramWidth:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Width";
        parameter.symbol     = "width";
        parameter.unit       = " ";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 2.5f;
        break;
    }
}


void ZamHeadX2Plugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "Zero";
		break;
	}
}

void ZamHeadX2Plugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		azimuth = 0.0;
		elevation = 0.0;
		width = 1.0;
		break;
	}

	activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZamHeadX2Plugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramAzimuth:
        return azimuth;
        break;
    case paramElevation:
        return elevation;
        break;
    case paramWidth:
        return width;
        break;
    }
    return 0.f;
}

void ZamHeadX2Plugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramAzimuth:
        azimuth = value;
	setState("reload", "");
        break;
    case paramElevation:
        elevation = value;
	setState("reload", "");
        break;
    case paramWidth:
        width = value;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void ZamHeadX2Plugin::activate()
{
	setState("reload", "");
	signal = true;
}

void ZamHeadX2Plugin::deactivate()
{
	signal = false;
}

String ZamHeadX2Plugin::getState(const char*) const
{
	return String("");
}

void ZamHeadX2Plugin::initState(unsigned int index, String& key, String& defval)
{
	if (index == 0) {
		key = String("reload");
	}
	defval = String("");
}

void ZamHeadX2Plugin::setState(const char* key, const char*)
{
	uint8_t other;
	char elev[4] = { 0 };
	char azim[4] = { 0 };
	int az, el;

	if (strcmp(key, "reload") == 0) {
		el = (int)((elevation + 45.) * 24. / 135.);
		if (el >= 24) el = 24;
		if (el < 0) el = 0;
		az = (int)((azimuth + 90.) * 49. / 360.);
		if (az >= 49) az = 49;
		if (az < 0) az = 0;
		if (az > 24) az = 49 - az;
		snprintf(elev, 3, "%d", el);
		snprintf(azim, 3, "%d", az);
		if ((az != azold) || (el != elold)) {
			other = !active;
			clv[other]->clv_release();
			clv[other]->clv_configure("convolution.ir.preset", elev, azim);
			clv[other]->clv_initialize(getSampleRate(), 2, 2, getBufferSize());
			swap = other;
		}
		azold = az;
		elold = el;
	}
}


void ZamHeadX2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	float m, s;
	uint32_t i;
	int nprocessed;
	active = swap;

	if (!signal) {
		memcpy(outputs[0], inputs[0], frames * sizeof(float));
		memcpy(outputs[1], inputs[1], frames * sizeof(float));
		return;
	}

	assert(frames < 8192);
	for (i = 0; i < frames; i++) {
		m = (inputs[0][i] + inputs[1][i]) * 0.5;
		s = (inputs[0][i] - inputs[1][i]) * 0.5 * width;
		tmpins[0][i] = m - s;
		tmpins[1][i] = m + s;
	}
 
	nprocessed = clv[active]->clv_convolve(tmpins, tmpouts, 2, 2, frames, from_dB(6.0));
	if (nprocessed <= 0) {
		memcpy(outputs[0], inputs[0], frames * sizeof(float));
		memcpy(outputs[1], inputs[1], frames * sizeof(float));
	} else {
		memcpy(outputs[0], tmpouts[0], frames * sizeof(float));
		memcpy(outputs[1], tmpouts[1], frames * sizeof(float));
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamHeadX2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
