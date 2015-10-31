/*
 * ZaMaximX2 Stereo maximiser
 * Copyright (C) 2015  Damien Zammit <damien@zamaudio.com>
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

#include "ZaMaximX2Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZaMaximX2Plugin::ZaMaximX2Plugin()
    : Plugin(paramCount, 1, 0)
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZaMaximX2Plugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramRelease:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Release";
        parameter.symbol     = "rel";
        parameter.unit       = "ms";
        parameter.ranges.def = 25.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 500.0f;
        break;
    case paramCeiling:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Output Ceiling";
        parameter.symbol     = "ceil";
        parameter.unit       = "dB";
        parameter.ranges.def = -3.0f;
        parameter.ranges.min = -30.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramThresh:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Threshold";
        parameter.symbol     = "thresh";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -30.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramGainRed:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Gain Reduction";
        parameter.symbol     = "gr";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 40.0f;
        break;
    case paramOutputLevel:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Output Level";
        parameter.symbol     = "outlevel";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 0.0f;
        break;
    }
}


void ZaMaximX2Plugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "Zero";
		break;
	}
}

void ZaMaximX2Plugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		release = 150.0;
		ceiling = -0.5;
		thresdb = 0.0;
		gainred = 0.0;
		outlevel = -45.0;
		break;
	}

	activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZaMaximX2Plugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramRelease:
        return release;
        break;
    case paramThresh:
        return thresdb;
        break;
    case paramCeiling:
        return ceiling;
        break;
    case paramGainRed:
        return gainred;
        break;
    case paramOutputLevel:
        return outlevel;
        break;
    default:
        return 0.0f;
    }
}

void ZaMaximX2Plugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramRelease:
        release = value;
        break;
    case paramThresh:
        thresdb = value;
        break;
    case paramCeiling:
        ceiling = value;
        break;
    case paramGainRed:
        gainred = value;
        break;
    case paramOutputLevel:
        outlevel = value;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void ZaMaximX2Plugin::activate()
{
    int i;
    gainred = 0.0f;
    outlevel = -45.0f;
    for (i = 0; i < MAX_SAMPLES; i++) {
    	z[0][i] = 0.f;
    	z[1][i] = 0.f;
    }
    posz[0] = posz[1] = 0;
    smoothold[0] = smoothold[1] = 0.f;
    envold[0] = envold[1] = 0.f;
}

void ZaMaximX2Plugin::deactivate()
{
	activate();
}

double ZaMaximX2Plugin::normalise(double in, double gainr)
{
	if (ceiling < thresdb) {
		return in;
	}
	return from_dB(-(thresdb - ceiling + gainr)) * in;
}

double ZaMaximX2Plugin::clip(double in)
{
	if (in < -1.f)
		in = -1.f;
	if (in > 1.f)
		in = 1.f;
	return in;
}

void ZaMaximX2Plugin::pushsample(double in[], double sample, int *pos)
{
	(*pos)++;
	if (*pos >= MAX_SAMPLES) {
		*pos = 0;
	}
	in[*pos] = sample;
}

double ZaMaximX2Plugin::getoldsample(double in[], int pos)
{
	int p = pos - 1;
	if (p < 0) {
		p = MAX_SAMPLES;
	}
	return in[p];
}

double ZaMaximX2Plugin::avgall(double in[])
{
	int i;
	double avg = 0.f;
	for (i = 0; i < MAX_SAMPLES; i++) {
		avg += in[i];
	}
	avg = avg / (double)(MAX_SAMPLES);
	return avg;
}

double ZaMaximX2Plugin::maxsample(double in[])
{
	int i;
	double max = 0.f;
	for (i = 0; i < MAX_SAMPLES; i++) {
		if (fabs(in[i]) > fabs(max)) {
			max = in[i];
		}
	}
	return max;
}

void ZaMaximX2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint32_t i;
	double absx[2];
	double c[2];
	double srate = getSampleRate();
	double tau = exp(-1000. / (release*srate));
	double env[2];
	double smooth[2];
	double targetgain = from_dB(ceiling);

	double inL, inR;

	for (i = 0; i < frames; i++) {
		inL = inputs[0][i];
		inR = inputs[1][i];
		
		// input
		absx[0] = MAX(fabs(inL), fabs(inR));
		
		// Envelope extractor
		if (envold[0] > absx[0]) {
			env[0] = tau * envold[0] + (1. - tau) * absx[0];
		} else {
			env[0] = absx[0];
		}
		if (envold[1] > absx[0]) {
			env[1] = tau * envold[1] + (1. - tau) * absx[0];
		} else {
			env[1] = absx[0];
		}
		
		// control gain calculator
		c[0] = 1. / (1. / targetgain + (1. - 1. / targetgain)*env[0]);
		c[0] *= MIN(1., env[0] / from_dB(thresdb));
		c[1] = 1. / (1. / targetgain + (1. - 1. / targetgain)*env[1]);
		c[1] *= MIN(1., env[1] / from_dB(thresdb));

		// smoother
		if (c[0] > smoothold[0]) {
			smooth[0] = smoothold[0]*tau + c[0]*(1.-tau);
		} else {
			smooth[0] = c[0];
		}
		if (c[1] > smoothold[1]) {
			smooth[1] = smoothold[1]*tau + c[1]*(1.-tau);
		} else {
			smooth[1] = c[1];
		}

		outputs[0][i] = (float)clip(normalise(z[0][posz[0]] * smooth[0], 0.));
		outputs[1][i] = (float)clip(normalise(z[1][posz[1]] * smooth[1], 0.));

		outlevel = to_dB(MAX(fabs(outputs[0][i]), fabs(outputs[1][i]))) + (ceiling - thresdb);
		pushsample(&z[0][0], sanitize_denormal(inL), &posz[0]);
		pushsample(&z[1][0], sanitize_denormal(inR), &posz[1]);

		smoothold[0] = sanitize_denormal(smooth[0]);
		smoothold[1] = sanitize_denormal(smooth[1]);
		envold[0] = sanitize_denormal(env[0]);
		envold[1] = sanitize_denormal(env[1]);
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZaMaximX2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
