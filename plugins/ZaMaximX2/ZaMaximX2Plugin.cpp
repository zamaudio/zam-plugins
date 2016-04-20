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
        parameter.ranges.max = 100.0f;
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
		release = 25.0;
		ceiling = -3.0;
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
    for (i = 0; i < MAX_DELAY; i++) {
        cn[0][i] = z[0][i] = 0.f;
        cn[1][i] = z[1][i] = 0.f;
    }
    for (i = 0; i < MAX_AVG; i++) {
        emaxn[1][i] = 0.f;
        emaxn[0][i] = 0.f;
    }
    posz[0] = posz[1] = 0;
    pose[0] = pose[1] = 0;
    posc[0] = posc[1] = 0;
    emax_old[0] = emax_old[1] = 0.f;
    e_old[0] = e_old[1] = 0.f;
}

void ZaMaximX2Plugin::deactivate()
{
	activate();
}

double ZaMaximX2Plugin::normalise(double in)
{
	if (ceiling < thresdb) {
		return in;
	}
	return from_dB(-thresdb + ceiling) * in;
}

void ZaMaximX2Plugin::pushsample(double in[], double sample, int *pos, int maxsamples)
{
	(*pos)++;
	if (*pos >= maxsamples) {
		*pos = 0;
	}
	in[*pos] = sample;
}

double ZaMaximX2Plugin::avgall(double in[])
{
	int i;
	double avg = 0.f;
	for (i = 0; i < MAX_AVG; i++) {
		avg += in[i];
	}
	avg = avg / (double)(MAX_AVG);
	return avg;
}

double ZaMaximX2Plugin::maxsample(double in[])
{
	int i;
	double max = 0.f;
	for (i = 0; i < MAX_DELAY; i++) {
		if (fabs(in[i]) > fabs(max)) {
			max = in[i];
		}
	}
	return max;
}

void ZaMaximX2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint32_t i;
	double N = (double)MAX_DELAY;
	double navg = (double)MAX_AVG;
	double absx[2];
	double c[2];
	double xmax[2];
	double emax[2];
	double avge[2];
	double g[2];
	double srate = getSampleRate();
	double alpha = 1.0001;
	double aatt = 1. - pow( (alpha - 1.) / alpha,  1. / ( N + 1. ) );
	double a;
	double beta;
	double e;
	double target;
	double max;
	double maxx = 0;
	beta = 0.;
	for (i = 0; i < navg; i++) {
		beta += pow(1. - aatt, N + 1. - i);
	}
	beta /= navg;

	double inL, inR;

	for (i = 0; i < frames; i++) {
		inL = inputs[0][i];
		inR = inputs[1][i];
		absx[0] = fmaxf(fabsf(inL), fabsf(inR));
		c[0] = fmaxf(absx[0], (absx[0]-beta*e_old[0]) / (1. - beta));
		xmax[0] = maxsample(&cn[0][0]);
		target = xmax[0];

		if (target > emax_old[0]) {
			a = 1000 / (aatt * srate);
		} else {
			a = 1000 / (release * srate);
		}
		emax[0] = (a*target + (1. - a)*emax_old[0]);
//push
		e = avgall(&emaxn[0][0]);
		avge[0] = e;
		avge[0] += emaxn[0][(pose[0]+1+MAX_AVG) % MAX_AVG]/(navg+1.);
		e = avge[0];

		if (e == 0.f) {
			g[0] = 1.;
		} else {
			g[0] = sanitize_denormal(fminf(1., from_dB(thresdb) / e));
		}

		gainred = -to_dB(g[0]);

		outputs[0][i] = z[0][(posz[0]+1+MAX_DELAY) % MAX_DELAY] * g[0] * from_dB(ceiling - thresdb);
		outputs[1][i] = z[1][(posz[1]+1+MAX_DELAY) % MAX_DELAY] * g[0] * from_dB(ceiling - thresdb);

		max = fmaxf(fabsf(outputs[0][i]), fabsf(outputs[1][i]));

		if (maxx < max)
			maxx = max;
		
		pushsample(&emaxn[0][0], sanitize_denormal(emax[0]), &pose[0], MAX_AVG);
		pushsample(&cn[0][0], sanitize_denormal(c[0]), &posc[0], MAX_DELAY);
		pushsample(&z[0][0], sanitize_denormal(inL), &posz[0], MAX_DELAY);
		pushsample(&z[1][0], sanitize_denormal(inR), &posz[1], MAX_DELAY);

		emax_old[0] = sanitize_denormal(emax[0]);
		e_old[0] = sanitize_denormal(e);
	}
	outlevel = (maxx == 0.f) ? -160. : to_dB(maxx);
//	if (outlevel > 0.) printf("g=%f out=%f\n", gainred, outlevel);
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZaMaximX2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
