/*
 * ZamDelay mono delay effect
 * Copyright (C) 2015  Damien Zammit <damien@zamaudio.com>
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

#include "ZamDelayPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamDelayPlugin::ZamDelayPlugin()
    : Plugin(paramCount, 1, 0)
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamDelayPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramInvert:
        parameter.hints      = kParameterIsAutomatable | kParameterIsBoolean;
        parameter.name       = "Invert";
        parameter.symbol     = "inv";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramDelaytime:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Time";
        parameter.symbol     = "time";
        parameter.unit       = "ms";
        parameter.ranges.def = 160.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 8000.0f;
        break;
    case paramSync:
        parameter.hints      = kParameterIsAutomatable | kParameterIsBoolean;
        parameter.name       = "Sync BPM";
        parameter.symbol     = "sync";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramLPF:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "LPF";
        parameter.symbol     = "lpf";
        parameter.unit       = "Hz";
        parameter.ranges.def = 6000.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 20000.0f;
        break;
    case paramDivisor:
        parameter.hints      = kParameterIsAutomatable | kParameterIsInteger;
        parameter.name       = "Divisor";
        parameter.symbol     = "div";
        parameter.unit       = " ";
        parameter.ranges.def = 3.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 5.0f;
        break;
    case paramGain:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Output Gain";
        parameter.symbol     = "gain";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -60.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramDrywet:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Dry/Wet";
        parameter.symbol     = "drywet";
        parameter.unit       = " ";
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramFeedback:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Feedback";
        parameter.symbol     = "feedb";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramDelaytimeout:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Delaytime";
        parameter.symbol     = "delaytime";
        parameter.unit       = "ms";
        parameter.ranges.def = 160.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 8000.0f;
        break;
    }
}


void ZamDelayPlugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "Zero";
		break;
	}
}

void ZamDelayPlugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		invert = 0.f;
		delaytime = 160.f;
		sync = 0.f;
		lpf = 6000.f;
		divisor = 3.f;
		gain = 0.f;
		drywet = 0.5f;
		feedb = 0.0f;
		delaytimeout = 160.f;
		break;
	}

	activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZamDelayPlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramInvert:
        return invert;
        break;
    case paramDelaytime:
        return delaytime;
        break;
    case paramSync:
        return sync;
        break;
    case paramLPF:
        return lpf;
        break;
    case paramDivisor:
        return divisor;
        break;
    case paramGain:
        return gain;
        break;
    case paramDrywet:
        return drywet;
        break;
    case paramFeedback:
        return feedb;
        break;
    case paramDelaytimeout:
        return delaytimeout;
        break;
    default:
        return 0.0f;
    }
}

void ZamDelayPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramInvert:
        invert = value;
        break;
    case paramDelaytime:
        delaytime = value;
        break;
    case paramSync:
        sync = value;
        break;
    case paramLPF:
        lpf = value;
        break;
    case paramDivisor:
        divisor = value;
        break;
    case paramGain:
        gain = value;
        break;
    case paramDrywet:
        drywet = value;
        break;
    case paramFeedback:
        feedb = value;
        break;
    case paramDelaytimeout:
        delaytimeout = value;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void ZamDelayPlugin::activate()
{
	int i;
	for (i = 0; i < MAX_DELAY; i++) {
		z[i] = 0.f;
	}
	posz = 0;
	tap[0] = 0;
	tap[1] = 0;
	active = 0;
	next = 1;
	age = 0;
	clearfilter();
	fbstate = 0.f;

	lpfold = 0.f;
	divisorold = 0.f;
	gainold = 0.f;
	invertold = 0.f;
	delaytimeold = 0.f;
	syncold = 0.f;
	drywetold = 0.f;
	delaytimeoutold = 0.f;
	delaysamplesold = 1.f;
}



void ZamDelayPlugin::lpfRbj(float fc, float srate)
{
	float w0, alpha, cw, sw, q;
	q = 0.707;
	w0 = (2. * M_PI * fc / srate);
	sw = sin(w0);
	cw = cos(w0);
	alpha = sw / (2. * q);

	A0 = 1. + alpha;
	A1 = -2. * cw;
	A2 = 1. - alpha;
	B0 = (1. - cw) / 2.;
	B1 = (1. - cw);
	B2 = B0;

	A3 = 1. + alpha;
	A4 = -2. * cw;
	A5 = 1. - alpha;
	B3 = (1. - cw) / 2.;
	B4 = (1. - cw);
	B5 = B3;
}

void ZamDelayPlugin::clearfilter(void)
{
	state[0] = state[1] = state[2] = state[3] = 0.f;
}

float ZamDelayPlugin::runfilter(float in)
{
	float out;
	in = sanitize_denormal(in);

	out = B0/A0*in + B1/A0*state[0] + B2/A0*state[1]
			-A1/A0*state[2] - A2/A0*state[3] + 1e-12;

	state[1] = state[0];
	state[0] = in;
	state[3] = state[2];
	state[2] = out;
	return out;
}

void ZamDelayPlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint32_t i;
	float in;
	float srate = getSampleRate();
	TimePosition t = getTimePosition();
	float bpm = 120.f;
	int delaysamples;
	unsigned int tmp;
	float inv;
	float xfade;
	int recalc;
	if (invert < 0.5) {
		inv = -1.f;
	} else {
		inv = 1.f;
	}
	
	recalc = 0;
	delaytimeout = delaytime;
	if (t.bbt.valid) {
		bpm = t.bbt.beatsPerMinute;
		if (sync > 0.5f) {
			delaytimeout = (float)t.bbt.beatType * 1000.f * 60.f / (bpm * powf(2., divisor - 1.));
		}
	}
	delaysamples = (int)(delaytimeout * srate) / 1000;
	
	if (lpf != lpfold) {
		lpfRbj(lpf, srate);
	}
	if (divisor != divisorold) {
		recalc = 1;
	}
	if (gain != gainold) {
		recalc = 1;
	}
	if (invert != invertold) {
		recalc = 1;
	}
	if (delaytime != delaytimeold) {
		recalc = 1;
	}
	if (sync != syncold) {
		recalc = 1;
	}
	if (delaytimeout != delaytimeoutold) {
		recalc = 1;
	}
	
	if (recalc) {
		tap[next] = delaysamples;
	}

	xfade = 0.f;
	for (i = 0; i < frames; i++) {
		in = inputs[0][i];
		z[posz] = in + feedb * fbstate;
		fbstate = 0.f;
		int p = posz - tap[active]; // active line
		if (p<0) p += MAX_DELAY;
		fbstate += z[p];
		
		if (recalc) {
			xfade += 1.0f / (float)frames;
			fbstate *= (1.-xfade);
			int p = posz - tap[next]; // next line
			if (p<0) p += MAX_DELAY;
			fbstate += z[p] * xfade;
		}
		outputs[0][i] = from_dB(gain) * ((1.-drywet)*in + drywet * -inv * runfilter(fbstate));
		if (++posz >= MAX_DELAY) {
			posz = 0;
		}
	}
	lpfold = lpf;
	divisorold = divisor;
	gainold = gain;
	invertold = invert;
	delaytimeold = delaytime;
	syncold = sync;
	drywetold = drywet;
	delaytimeoutold = delaytimeout;
	delaysamplesold = delaysamples;
	if (recalc) {
		tmp = active;
		active = next;
		next = tmp;
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamDelayPlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
