/*
 * ZamPhono production/reproduction filters
 * Copyright (C) 2016  Damien Zammit <damien@zamaudio.com>
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
 */

#include "ZamPhonoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamPhonoPlugin::ZamPhonoPlugin()
    : Plugin(paramCount, 1, 0)
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamPhonoPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramToggle:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Reproduction/Production";
        parameter.symbol     = "inv";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramType:
        parameter.hints      = kParameterIsAutomable | kParameterIsInteger;
        parameter.name       = "Phono Filter Type";
        parameter.symbol     = "type";
        parameter.unit       = " ";
        parameter.ranges.def = 3.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 4.0f;
        break;
    }
}

void ZamPhonoPlugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "RIAA (Playback)";
		break;
	}
}

void ZamPhonoPlugin::loadProgram(uint32_t index)
{
	switch(index) {
	default:
	case 0:
		type = 3.0;
		inv = 0.0;
		break;
	}
    /* reset filter values */
    activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZamPhonoPlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramToggle:
        return inv;
        break;
    case paramType:
        return type;
        break;
    default:
        return 0.0f;
    }
}

void ZamPhonoPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramToggle:
        inv = value;
        break;
    case paramType:
        type = value;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void ZamPhonoPlugin::activate()
{
	zn1 = zn2 = zd1 = zd2 = 0.0;
}

void ZamPhonoPlugin::emphasis(float srate)
{
	float t,i,j,k,g,tau1,tau2,tau3,freq;

	switch((int)type) {
	case 0: //"Columbia"
		i = 100.f;
		j = 500.f;
		k = 1590.f;
		break;
	case 1: //"EMI"
		i = 70.f;
		j = 500.f;
		k = 2500.f;
		break;
	case 2: //"BSI(78rpm)"
		i = 50.f;
		j = 353.f;
		k = 3180.f;
		break;
	default:
	case 3: //"RIAA"
		tau1 = 0.003180f;
		tau2 = 0.000318f;
		tau3 = 0.000075f;
		i = 1.f / (2.f * M_PI * tau1);
		j = 1.f / (2.f * M_PI * tau2);
		k = 1.f / (2.f * M_PI * tau3);
		break;
	case 4: //"CD Emphasis"
		tau1 = 0.000050f;
		tau2 = 0.000015f;
		tau3 = 0.0000001f;// 1.6MHz out of audible range for null impact
		i = 1.f / (2.f * M_PI * tau1);
		j = 1.f / (2.f * M_PI * tau2);
		k = 1.f / (2.f * M_PI * tau3);
		break;
	}

	t = 1.f / srate;
	g = 1.0;
	
	i *= 2.f * M_PI;
	j *= 2.f * M_PI;
	k *= 2.f * M_PI;

	if (inv < 0.5) {
		//Reproduction
		g = 1.f / (4.f+2.f*i*t+2.f*k*t+i*k*t*t);
		b0 = (2.f*t+j*t*t)*g;
		b1 = (2.f*j*t*t)*g;
		b2 = (-2.f*t+j*t*t)*g;
		a1 = (-8.f+2.f*i*k*t*t)*g;
		a2 = (4.f-2.f*i*t-2.f*k*t+i*k*t*t)*g;
	} else {
		//Production
		g = 1.f / (2.f*t+j*t*t);
		b0 = (4.f+2.f*i*t+2.f*k*t+i*k*t*t)*g;
		b1 = (-8.f+2.f*i*k*t*t)*g;
		b2 = (4.f-2.f*i*t-2.f*k*t+i*k*t*t)*g;
		a1 = (2.f*j*t*t)*g;
		a2 = (-2.f*t+j*t*t)*g;
	}

	freq = 1000.0 * 2.0 * M_PI / srate;
	std::complex<double> z = 1.0 / exp(std::complex<double>(0.0, freq));
	g = std::abs((std::complex<double>(b0) + double(b1) * z + double(b2) * z*z) / (std::complex<double>(1.0) + double(a1) * z + double(a2) * z*z));
	b0 /= g;
	b1 /= g;
	b2 /= g;
}

double ZamPhonoPlugin::run_filter(double in)
{
	double out;

	in = sanitize_denormal(in);
	out = in * b0 + zn1 * b1 + zn2 * b2
		      - zd1 * a1 - zd2 * a2;
	out = sanitize_denormal(out);
	zn2 = sanitize_denormal(zn1);
	zd2 = sanitize_denormal(zd1);
	zn1 = in;
	zd1 = out;

	return out;
}


void ZamPhonoPlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	emphasis(getSampleRate());
	double tmp;
	double in;

	for (uint32_t i = 0; i < frames; i++) {
		in = inputs[0][i];
		tmp = run_filter(in);
		outputs[0][i] = in;
		outputs[0][i] = (float)tmp;
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
	return new ZamPhonoPlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
