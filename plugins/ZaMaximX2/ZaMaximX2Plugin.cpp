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
        parameter.ranges.def = 10.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 100.0f;
        break;
    case paramCeiling:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Output Ceiling";
        parameter.symbol     = "ceil";
        parameter.unit       = "dB";
        parameter.ranges.def = -0.5f;
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
		release = 10.0;
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
    	cn[0][i] = z[0][i] = emaxn[0][i] = 0.f;
    	cn[1][i] = z[1][i] = emaxn[1][i] = 0.f;
    }
    posz[0] = posz[1] = 0;
    pose[0] = pose[1] = 0;
    posc[0] = posc[1] = 0;
    emax_old[0] = emax_old[1] = 0.f;
    eavg_old[0] = eavg_old[1] = 0.f;
}

void ZaMaximX2Plugin::deactivate()
{
	activate();
}

float ZaMaximX2Plugin::normalise(float in, float gainr)
{
	if (ceiling < thresdb) {
		return in;
	}
	return from_dB(-(thresdb - ceiling + gainr)) * in;
}

float ZaMaximX2Plugin::clip(float in)
{
	if (in < -1.f)
		in = -1.f;
	if (in > 1.f)
		in = 1.f;
	return in;
}

void ZaMaximX2Plugin::pushsample(float in[], float sample, int *pos)
{
	(*pos)++;
	if (*pos >= MAX_SAMPLES) {
		*pos = 0;
	}
	in[*pos] = sample;
}

float ZaMaximX2Plugin::getoldsample(float in[], int pos)
{
	int p = pos - 1;
	if (p < 0) {
		p = MAX_SAMPLES;
	}
	return in[p];
}

float ZaMaximX2Plugin::avgall(float in[])
{
	int i;
	float avg = 0.f;
	for (i = 0; i < MAX_SAMPLES; i++) {
		avg += in[i];
	}
	avg = avg / (float)(MAX_SAMPLES);
	return avg;
}

float ZaMaximX2Plugin::maxsample(float in[])
{
	int i;
	float max = 0.f;
	for (i = 0; i < MAX_SAMPLES; i++) {
		if (in[i] > max) {
			max = in[i];
		}
	}
	return max;
}

void ZaMaximX2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint32_t i;
	float N = (float)MAX_SAMPLES;
	float M = (float)MAX_SAMPLES;
	float absx[2];
	float c[2];
	float xmax[2];
	float emax[2];
	float eavg[2];
	float g[2];
	float att = 0.2;
	float rel = 0.01;
	float a = 0.;
	float beta = 0.;
	float aatt = 1. - exp(log(att / (1.+att)) / (N + 1.));
	float arel = 1. - exp(log(rel / (1.+rel)) / (N + 1.));
	float beta_rel = 0.f;
	float beta_att = 0.f;
	for (i = 0; i < M; i++) {
		beta_att += powf(1. - aatt, N + 1. - i);
		beta_rel += powf(1. - arel, N + 1. - i);
	}
	beta_att /= M;
	beta_rel /= M;

	float maxx;
	int n;

	for (i = 0; i < frames; i++) {
		absx[0] = fabsf(inputs[0][i]);
		xmax[0] = maxsample(&cn[0][0]);
		
		absx[1] = fabsf(inputs[1][i]);
		xmax[1] = maxsample(&cn[1][0]);
		
		maxx = MAX(xmax[0], xmax[1]);

		if ((maxx > emax_old[0]) ||  (maxx > emax_old[1])) {
			a = aatt;
			beta = beta_att;
		} else {
			a = arel;
			beta = beta_rel;
		}
		
		c[0] = MAX(absx[0], (absx[0]-beta*emax_old[0]) / (1. - beta));
		emax[0] = a*xmax[0] + (1. - a)*emax_old[0];
		eavg[0] = avgall(&emaxn[0][0]);
		if (eavg[0] == 0.f) {
			g[0] = 1.;
		} else {
			g[0] = MIN(1., from_dB(thresdb) / eavg[0]);
		}
		c[1] = MAX(absx[1], (absx[1]-beta*emax_old[1]) / (1. - beta));
		emax[1] = a*xmax[1] + (1. - a)*emax_old[1];
		eavg[1] = avgall(&emaxn[1][0]);
		if (eavg[1] == 0.f) {
			g[1] = 1.;
		} else {
			g[1] = MIN(1., from_dB(thresdb) / eavg[1]);
		}

		gainred = MAX(-to_dB(g[0]), -to_dB(g[1]));
		outlevel = sanitize_denormal(to_dB(maxx)) + (ceiling);

		outputs[0][i] = inputs[0][i];
		outputs[1][i] = inputs[1][i];
		outputs[0][i] = clip(normalise(z[0][posz[0]] * g[0], -to_dB(0.90)));
		outputs[1][i] = clip(normalise(z[1][posz[1]] * g[1], -to_dB(0.90)));


		pushsample(&cn[0][0], sanitize_denormal(c[0]), &posc[0]);
		pushsample(&z[0][0], sanitize_denormal(inputs[0][i]), &posz[0]);
		pushsample(&emaxn[0][0], sanitize_denormal(emax[0]), &pose[0]);

		pushsample(&cn[1][0], sanitize_denormal(c[1]), &posc[1]);
		pushsample(&z[1][0], sanitize_denormal(inputs[1][i]), &posz[1]);
		pushsample(&emaxn[1][0], sanitize_denormal(emax[1]), &pose[1]);

		emax_old[0] = sanitize_denormal(emax[0]);
		eavg_old[0] = sanitize_denormal(eavg[0]);
		emax_old[1] = sanitize_denormal(emax[1]);
		eavg_old[1] = sanitize_denormal(eavg[1]);
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZaMaximX2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
