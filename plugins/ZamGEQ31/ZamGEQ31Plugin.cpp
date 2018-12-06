/*
 * ZamGEQ31 1/3 octave band graphic equaliser
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com>
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

#include "ZamGEQ31Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamGEQ31Plugin::ZamGEQ31Plugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamGEQ31Plugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramMaster:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Master Gain";
        parameter.symbol     = "master";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -30.f;
        parameter.ranges.max = 30.f;
        break;
    case paramGain1:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "32Hz";
        parameter.symbol     = "band1";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain2:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "40Hz";
        parameter.symbol     = "band2";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain3:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "50Hz";
        parameter.symbol     = "band3";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain4:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "63Hz";
        parameter.symbol     = "band4";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain5:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "79Hz";
        parameter.symbol     = "band5";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain6:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "100Hz";
        parameter.symbol     = "band6";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain7:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "126Hz";
        parameter.symbol     = "band7";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain8:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "158Hz";
        parameter.symbol     = "band8";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain9:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "200Hz";
        parameter.symbol     = "band9";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain10:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "251Hz";
        parameter.symbol     = "band10";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain11:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "316Hz";
        parameter.symbol     = "band11";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain12:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "398Hz";
        parameter.symbol     = "band12";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain13:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "501Hz";
        parameter.symbol     = "band13";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain14:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "631Hz";
        parameter.symbol     = "band14";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain15:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "794Hz";
        parameter.symbol     = "band15";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain16:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "999Hz";
        parameter.symbol     = "band16";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain17:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "1257Hz";
        parameter.symbol     = "band17";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain18:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "1584Hz";
        parameter.symbol     = "band18";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain19:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "1997Hz";
        parameter.symbol     = "band19";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain20:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "2514Hz";
        parameter.symbol     = "band20";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain21:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "3165Hz";
        parameter.symbol     = "band21";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain22:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "3986Hz";
        parameter.symbol     = "band22";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain23:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "5017Hz";
        parameter.symbol     = "band23";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain24:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "6318Hz";
        parameter.symbol     = "band24";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain25:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "7963Hz";
        parameter.symbol     = "band25";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain26:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "10032Hz";
        parameter.symbol     = "band26";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain27:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "12662Hz";
        parameter.symbol     = "band27";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain28:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "16081Hz";
        parameter.symbol     = "band28";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain29:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "20801Hz";
        parameter.symbol     = "band29";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    }
}

void ZamGEQ31Plugin::initProgramName(uint32_t index, String& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamGEQ31Plugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramMaster:
        return master;
        break;
    case paramGain1:
        return gain[0];
        break;
    case paramGain2:
        return gain[1];
        break;
    case paramGain3:
        return gain[2];
        break;
    case paramGain4:
        return gain[3];
        break;
    case paramGain5:
        return gain[4];
        break;
    case paramGain6:
        return gain[5];
        break;
    case paramGain7:
        return gain[6];
        break;
    case paramGain8:
        return gain[7];
        break;
    case paramGain9:
        return gain[8];
        break;
    case paramGain10:
        return gain[9];
        break;
    case paramGain11:
        return gain[10];
        break;
    case paramGain12:
        return gain[11];
        break;
    case paramGain13:
        return gain[12];
        break;
    case paramGain14:
        return gain[13];
        break;
    case paramGain15:
        return gain[14];
        break;
    case paramGain16:
        return gain[15];
        break;
    case paramGain17:
        return gain[16];
        break;
    case paramGain18:
        return gain[17];
        break;
    case paramGain19:
        return gain[18];
        break;
    case paramGain20:
        return gain[19];
        break;
    case paramGain21:
        return gain[20];
        break;
    case paramGain22:
        return gain[21];
        break;
    case paramGain23:
        return gain[22];
        break;
    case paramGain24:
        return gain[23];
        break;
    case paramGain25:
        return gain[24];
        break;
    case paramGain26:
        return gain[25];
        break;
    case paramGain27:
        return gain[26];
        break;
    case paramGain28:
        return gain[27];
        break;
    case paramGain29:
        return gain[28];
        break;
    default:
        return 0.0f;
    }
}

void ZamGEQ31Plugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramMaster:
        master = value;
        break;
    case paramGain1:
        gain[0] = value;
        break;
    case paramGain2:
        gain[1] = value;
        break;
    case paramGain3:
        gain[2] = value;
        break;
    case paramGain4:
        gain[3] = value;
        break;
    case paramGain5:
        gain[4] = value;
        break;
    case paramGain6:
        gain[5] = value;
        break;
    case paramGain7:
        gain[6] = value;
        break;
    case paramGain8:
        gain[7] = value;
        break;
    case paramGain9:
        gain[8] = value;
        break;
    case paramGain10:
        gain[9] = value;
        break;
    case paramGain11:
        gain[10] = value;
        break;
    case paramGain12:
        gain[11] = value;
        break;
    case paramGain13:
        gain[12] = value;
        break;
    case paramGain14:
        gain[13] = value;
        break;
    case paramGain15:
        gain[14] = value;
        break;
    case paramGain16:
        gain[15] = value;
        break;
    case paramGain17:
        gain[16] = value;
        break;
    case paramGain18:
        gain[17] = value;
        break;
    case paramGain19:
        gain[18] = value;
        break;
    case paramGain20:
        gain[19] = value;
        break;
    case paramGain21:
        gain[20] = value;
        break;
    case paramGain22:
        gain[21] = value;
        break;
    case paramGain23:
        gain[22] = value;
        break;
    case paramGain24:
        gain[23] = value;
        break;
    case paramGain25:
        gain[24] = value;
        break;
    case paramGain26:
        gain[25] = value;
        break;
    case paramGain27:
        gain[26] = value;
        break;
    case paramGain28:
        gain[27] = value;
        break;
    case paramGain29:
        gain[28] = value;
        break;
    }
}

void ZamGEQ31Plugin::geq(int i, float srate, float g)
{
	int mm;
	float w, omegaU, omegaL, omegaB, omegaM, glinear, alpham;
	// 2dB tolerance
	//const int stack[29] = {	2, 1, 1, 1, 1, 1, 1, 1,
	//			1, 1, 1, 1, 1, 1, 1, 1,
	//			1, 1, 1, 1, 1, 1, 1, 1,
	//			1, 1, 1, 2, 3 };
	
	// 0.15dB tolerance
	const int stack[29] = {	8, 8, 8, 8, 8, 8, 9, 9,
				9, 9, 9, 9, 9, 9, 9, 9,
				9, 9, 9, 9, 9, 9, 9, 10,
				11, 12, 15, 20, 20 };

	const float omegaLimit = 0.95 * M_PI;

	w = 2. * M_PI / srate;
	omegaU = w * this->omegaU[i];
	omegaL = w * this->omegaL[i];

	if (omegaU > omegaLimit) {
		m[i] = 0;
		return;
	}

	omegaB = omegaU - omegaL;
	omegaM = 2. * atan(sqrt(tan(omegaU/2.) * tan(omegaL/2.)));

	glinear = from_dB(g);
	m[i] = 2. * stack[i];
	
	k[i] = (1. / pow(glinear, (1. / (2. * m[i])))) * tan(omegaB / 2.);
	v[i] = pow(glinear, (1. / m[i])) - 1;
	a[i] = cos(omegaM);

	for (mm = 1; mm <= m[i] / 2; mm++) {
		alpham = (0.5 - (2. * mm - 1) / (2. * m[i])) * M_PI;
		cm[i][mm] = cos(alpham);
		a0m[i][mm] = 1. / (1. + 2. * k[i]*cm[i][mm] + k[i]*k[i]);
	}
}

double ZamGEQ31Plugin::run_filter(int i, double in)
{
	int j;
        double out, a1out, a2out, v1, v2;
        in = sanitize_denormal(in);
	a1out = 0.;
	a2out = 0.;
	v1 = v2 = 0.;
	out = in;

	// higher order
	for (j = 1; j <= m[i] / 2; j++) {

		// allpass 1
		a1out = a[i]*(w11[i][j] + a[i]*w12[i][j]) - w12[i][j];

		// allpass 2
		a2out = a[i]*(w21[i][j] + a[i]*w22[i][j]) - w22[i][j];

		// intermediate values
		v1 = (a2out - 2. * a1out) + k[i]*(2. * (-cm[i][j]) * a2out +
			k[i]*(2. * a1out + a2out));
		v2 = a2out + 2. * a1out;

		// 4th order output section
		out = in + v[i] * 2. * (-cm[i][j])*(-a0m[i][j]*(k[i]*in - v1) + a2out) +
			v[i]*(2. + v[i])*k[i]*(a0m[i][j]*(k[i]*in - v1) + v2);
		
		// update state
		w12[i][j] = w11[i][j] + a[i]*w12[i][j];
		w11[i][j] = a0m[i][j]*(k[i]*in - v1);
		w22[i][j] = w21[i][j] + a[i]*w22[i][j];
		w21[i][j] = a1out;
		
		in = out;
	}
	return out;
}

void ZamGEQ31Plugin::loadProgram(uint32_t index)
{
    if (index != 0)
        return;

    int i;
    /* Default parameter values */
    for (i = 0; i < 29; i++) {
    	gain[i] = 0.0f;
    }

    omegaL[0] = 28.2;
    omegaL[1] = 35.5;
    omegaL[2] = 44.7;
    omegaL[3] = 56.2;
    omegaL[4] = 70.8;
    omegaL[5] = 89.1;
    omegaL[6] = 112.;
    omegaL[7] = 141.;
    omegaL[8] = 178.;
    omegaL[9] = 224.;
    omegaL[10] = 282.;
    omegaL[11] = 355.;
    omegaL[12] = 447.;
    omegaL[13] = 562.;
    omegaL[14] = 708.;
    omegaL[15] = 891.;
    omegaL[16] = 1120.;
    omegaL[17] = 1410.;
    omegaL[18] = 1780.;
    omegaL[19] = 2240.;
    omegaL[20] = 2820.;
    omegaL[21] = 3550.;
    omegaL[22] = 4470.;
    omegaL[23] = 5620.;
    omegaL[24] = 7080.;
    omegaL[25] = 8910.;
    omegaL[26] = 11200.;
    omegaL[27] = 14100.;
    omegaL[28] = 17800.;

    omegaU[0] = 35.5;
    omegaU[1] = 44.7;
    omegaU[2] = 56.2;
    omegaU[3] = 70.8;
    omegaU[4] = 89.1;
    omegaU[5] = 112.;
    omegaU[6] = 141.;
    omegaU[7] = 178.;
    omegaU[8] = 224.;
    omegaU[9] = 282.;
    omegaU[10] = 355.;
    omegaU[11] = 447.;
    omegaU[12] = 562.;
    omegaU[13] = 708.;
    omegaU[14] = 891.;
    omegaU[15] = 1120.;
    omegaU[16] = 1410.;
    omegaU[17] = 1780.;
    omegaU[18] = 2240.;
    omegaU[19] = 2820.;
    omegaU[20] = 3550.;
    omegaU[21] = 4470.;
    omegaU[22] = 5620.;
    omegaU[23] = 7080.;
    omegaU[24] = 8910.;
    omegaU[25] = 11200.;
    omegaU[26] = 14100.;
    omegaU[27] = 17800.;
    omegaU[28] = 22400.;

    master = 0.f;
    float srate = getSampleRate();
    for (i = 0; i < 29; ++i) {
        geq(i, srate, 0.);
        gainold[i] = 0.;
    }

    /* reset filter values */
    activate();
}

// -----------------------------------------------------------------------
// Process

void ZamGEQ31Plugin::activate()
{
    int i, j;
    for (i = 0; i < 29; i++) {
        for (j = 0; j < 21; j++) {
            w11[i][j] = 0.;
            w12[i][j] = 0.;
            w21[i][j] = 0.;
            w22[i][j] = 0.;
        }
    }

}

void ZamGEQ31Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	float srate = getSampleRate();
	
	uint32_t i, j;
	for (i = 0; i < 29; i++) {
		if (gain[i] != gainold[i]) {
			geq(i, srate, gain[i]);
			gainold[i] = gain[i];
		}
	}

	for (i = 0; i < frames; i++) {
		double tmp, filtered;
		double in = inputs[0][i];
		in = sanitize_denormal(in);
		tmp = in;
		filtered = tmp;

		for (j = 0; j < 29; j++) {
			if (gain[j] != 0.f) {
				filtered = run_filter(j, tmp);
				tmp = filtered;
			}
		}
		outputs[0][i] = inputs[0][i];
		outputs[0][i] = (float) (filtered * from_dB(master));
	}
}

Plugin* createPlugin()
{
    return new ZamGEQ31Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
