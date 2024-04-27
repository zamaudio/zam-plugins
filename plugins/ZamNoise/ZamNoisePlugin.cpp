/*
 * ZamNoise  Noise detection and removal plugin
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

#include "ZamNoisePlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamNoisePlugin::ZamNoisePlugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    buffer.cbsize = 0;
    zamnoise = new Denoise(getSampleRate());
    ZamNoisePlugin::init();

    // set default values
    loadProgram(0);

    // reset
    deactivate();
}

ZamNoisePlugin::~ZamNoisePlugin()
{
	buffer.cbsize = 0;
	delete zamnoise;
	free(buffer.cbi);
}

// -----------------------------------------------------------------------
// Init

void ZamNoisePlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramNoiseToggle:
        parameter.hints      = kParameterIsAutomatable | kParameterIsBoolean;
        parameter.name       = "Noise Capture";
        parameter.symbol     = "noisecapture";
        parameter.unit       = "";
        parameter.ranges.def = 0.f;
        parameter.ranges.min = 0.f;
        parameter.ranges.max = 1.f;
        break;
    case paramReductionAmount:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Reduction Amount";
        parameter.symbol     = "amount";
        parameter.unit       = "%";
        parameter.ranges.def = 50.f;
        parameter.ranges.min = 0.f;
        parameter.ranges.max = 100.f;
        break;
    default:
	break;
    }
}

void ZamNoisePlugin::initProgramName(uint32_t index, String& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamNoisePlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramNoiseToggle:
        return noisetoggle;
        break;
    case paramReductionAmount:
        return amount;
        break;
    default:
	return 0.0f;
        break;
    }
}

void ZamNoisePlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramNoiseToggle:
        if (value == 1.f) {
		if (buffer.cbsize > 0)
			memset(buffer.cbi, 0, buffer.cbsize*sizeof(float));
	}
	noisetoggle = value;
        break;
    case paramReductionAmount:
        amount = value;
        break;
    }
}

void ZamNoisePlugin::loadProgram(uint32_t index)
{
    if (index != 0)
        return;

    noisetoggle = 0.f;
    amount = 50.f;

    activate();
}


void ZamNoisePlugin::InstantiateCircularBuffer(CircularBuffer* buf) {
	buf->cbi = (float*) calloc(DENOISE_MAX_FFT, sizeof(float));
	buf->cbsize = DENOISE_MAX_FFT;
}

void ZamNoisePlugin::init (void)
{
        InstantiateCircularBuffer(&buffer);
}


// -----------------------------------------------------------------------
// Process

void ZamNoisePlugin::activate()
{
}

void ZamNoisePlugin::deactivate()
{
}

void ZamNoisePlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	if (buffer.cbsize > 0) {
		zamnoise->process(inputs[0], outputs[0], buffer.cbi, frames, (int)noisetoggle, amount / 100.);
	} else {
		uint32_t i;
		for (i = 0; i < frames; i++) {
			outputs[0][i] = inputs[0][i];
		}
	}
}

void ZamNoisePlugin::sampleRateChanged(double newSampleRate)
{
	buffer.cbsize = 0;
	delete zamnoise;
	free(buffer.cbi);
	zamnoise = new Denoise(newSampleRate);
	ZamNoisePlugin::init();
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
	return new ZamNoisePlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
