/*
 * ZamGate gate plugin
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

#include "ZamGatePlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamGatePlugin::ZamGatePlugin()
	: Plugin(paramCount, 1, 0) // 1 program, 0 states
{
	// set default values
	loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamGatePlugin::initProgramName(uint32_t index, String& programName)
{
	if (index != 0)
		return;

	programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

void ZamGatePlugin::initParameter(uint32_t index, Parameter& parameter)
{
	switch (index)
	{
	case paramAttack:
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Attack";
		parameter.symbol = "att";
		parameter.unit = "ms";
		parameter.ranges.def = 50.0f;
		parameter.ranges.min = 0.1f;
		parameter.ranges.max = 500.0f;
		break;
	case paramRelease:
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Release";
		parameter.symbol = "rel";
		parameter.unit = "ms";
		parameter.ranges.def = 100.0f;
		parameter.ranges.min = 0.1f;
		parameter.ranges.max = 500.0f;
		break;
	case paramThresh:
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Threshold";
		parameter.symbol = "thr";
		parameter.unit = "dB";
		parameter.ranges.def = -60.0f;
		parameter.ranges.min = -60.0f;
		parameter.ranges.max = 0.0f;
		break;
	case paramMakeup:
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Makeup";
		parameter.symbol = "mak";
		parameter.unit = "dB";
		parameter.ranges.def = 0.0f;
		parameter.ranges.min = -30.0f;
		parameter.ranges.max = 30.0f;
		break;
	case paramSidechain:
		parameter.hints = kParameterIsAutomable | kParameterIsBoolean;
		parameter.name = "Sidechain";
		parameter.symbol = "sidechain";
		parameter.unit = " ";
		parameter.ranges.def = 0.0f;
		parameter.ranges.min = 0.0f;
		parameter.ranges.max = 1.0f;
		break;
	case paramGateclose:
		parameter.hints = kParameterIsAutomable;
		parameter.name = "Max gate close";
		parameter.symbol = "close";
		parameter.unit = "dB";
		parameter.ranges.def = -50.0f;
		parameter.ranges.min = -50.0f;
		parameter.ranges.max = 0.0f;
		break;
	case paramOpenshut:
		parameter.hints = kParameterIsAutomable | kParameterIsBoolean;
		parameter.name = "Mode open/shut";
		parameter.symbol = "mode";
		parameter.unit = " ";
		parameter.ranges.def = 0.0f;
		parameter.ranges.min = 0.0f;
		parameter.ranges.max = 1.0f;
		break;
	case paramGainR:
		parameter.hints = kParameterIsOutput;
		parameter.name = "Gain Reduction";
		parameter.symbol = "gainr";
		parameter.unit = "dB";
		parameter.ranges.def = 0.0f;
		parameter.ranges.min = 0.0f;
		parameter.ranges.max = 40.0f;
		break;
	case paramOutputLevel:
		parameter.hints = kParameterIsOutput;
		parameter.name = "Output Level";
		parameter.symbol = "outlevel";
		parameter.unit = "dB";
		parameter.ranges.def = -45.0f;
		parameter.ranges.min = -45.0f;
		parameter.ranges.max = 20.0f;
		break;
	}
}

void ZamGatePlugin::initAudioPort(bool input, uint32_t index, AudioPort& port)
{
	Plugin::initAudioPort(input, index, port);

	if ((index == 1) && input) {
		port.hints |= kAudioPortIsSidechain;
		port.name = "Sidechain Input";
		port.symbol = "sidechain_in";
	}
}

// -----------------------------------------------------------------------
// Internal data

float ZamGatePlugin::getParameterValue(uint32_t index) const
{
	switch (index)
	{
	case paramAttack:
		return attack;
		break;
	case paramRelease:
		return release;
		break;
	case paramThresh:
		return thresdb;
		break;
	case paramMakeup:
		return makeup;
		break;
	case paramSidechain:
		return sidechain;
		break;
	case paramGateclose:
		return gateclose;
		break;
	case paramOpenshut:
		return openshut;
		break;
	case paramGainR:
		return gainr;
		break;
	case paramOutputLevel:
		return outlevel;
		break;
	default:
		return 0.0f;
	}
}

void ZamGatePlugin::setParameterValue(uint32_t index, float value)
{
	switch (index)
	{
	case paramAttack:
		attack = value;
		break;
	case paramRelease:
		release = value;
		break;
	case paramThresh:
		thresdb = value;
		break;
	case paramMakeup:
		makeup = value;
		break;
	case paramSidechain:
		sidechain = value;
		break;
	case paramGateclose:
		gateclose = value;
		break;
	case paramOpenshut:
		openshut = value;
		break;
	case paramGainR:
		gainr = value;
		break;
	case paramOutputLevel:
		outlevel = value;
		break;
	}
}

void ZamGatePlugin::loadProgram(uint32_t)
{
	attack = 50.0;
	release = 100.0;
	thresdb = -60.0;
	gainr = 0.0;
	makeup = 0.0;
	sidechain = 0.0;
	openshut = 0.0;
	gateclose = -50.f;
	outlevel = -45.0;
	activate();
}

// -----------------------------------------------------------------------
// Process

void ZamGatePlugin::activate()
{
	int i;
	gatestatel = 0.f;
	posl = 0;
	for (i = 0; i < MAX_GATE; i++) {
		samplesl[i] = 0.f;
	}
}

void ZamGatePlugin::pushsamplel(float samples[], float sample)
{
	++posl;
	if (posl >= MAX_GATE)
		posl = 0;
	samples[posl] = sample;
}

float ZamGatePlugin::averageabs(float samples[])
{
	int i;
	float average = 0.f;

	for (i = 0; i < MAX_GATE; i++) {
		average += samples[i]*samples[i];
	}
	average /= (float) MAX_GATE;
	return sqrt(average);
}

void ZamGatePlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint32_t i;
	float absample;
	float att;
	float rel;
	float gl;
	float fs;
	fs = getSampleRate();
	gl = gatestatel;
	att = 1000.f / (attack * fs);
	rel = 1000.f / (release * fs);
	bool usesidechain = (sidechain < 0.5) ? false : true;
	float in0;
	float side;
	float max = 0.f;
	float mingate = (gateclose == -50.f) ? 0.f : from_dB(gateclose);

	for(i = 0; i < frames; i++) {
		in0 = inputs[0][i];
		side = inputs[1][i];
		if (usesidechain) {
			pushsamplel(samplesl, side);
		} else {
			pushsamplel(samplesl, in0);
		}
		absample = averageabs(samplesl);
		if (openshut < 0.5) {
			if (absample > from_dB(thresdb)) {
				gl += att;
				if (gl > 1.f)
					gl = 1.f;
			} else {
				gl -= rel;
				if (gl < mingate)
					gl = mingate;
			}
		} else {
			if (absample > from_dB(thresdb)) {
				gl -= att;
				if (gl < mingate)
					gl = mingate;
			} else {
				gl += rel;
				if (gl > 1.f)
					gl = 1.f;
			}
		}
		gatestatel = gl;

		outputs[0][i] = gl * from_dB(makeup) * in0;
		gainr = (gl > 0) ? sanitize_denormal(-to_dB(gl)) : 40.0;
		gainr = std::min(gainr, 40.f);
		max = (fabsf(outputs[0][i]) > max) ? fabsf(outputs[0][i]) : sanitize_denormal(max);
	}
	outlevel = (max == 0.f) ? -45.f : to_dB(max);
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
	return new ZamGatePlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
