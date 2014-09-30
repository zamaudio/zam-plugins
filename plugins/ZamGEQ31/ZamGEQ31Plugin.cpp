/*
 * ZamGEQ31 31 band graphic equaliser
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
    d_setProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamGEQ31Plugin::d_initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramQ:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Q of filters";
        parameter.symbol     = "q";
        parameter.unit       = " ";
        parameter.ranges.def = 1.4f;
        parameter.ranges.min = 0.5f;
        parameter.ranges.max = 5.0f;
        break;
    case paramMaster:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Master Gain";
        parameter.symbol     = "master";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain1:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "20Hz";
        parameter.symbol     = "band1";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain2:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "25Hz";
        parameter.symbol     = "band2";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain3:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "31.5Hz";
        parameter.symbol     = "band3";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain4:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "40Hz";
        parameter.symbol     = "band4";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain5:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "50Hz";
        parameter.symbol     = "band5";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain6:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "63Hz";
        parameter.symbol     = "band6";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain7:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "80Hz";
        parameter.symbol     = "band7";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain8:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "100Hz";
        parameter.symbol     = "band8";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain9:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "125Hz";
        parameter.symbol     = "band9";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain10:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "160Hz";
        parameter.symbol     = "band10";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain11:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "200Hz";
        parameter.symbol     = "band11";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain12:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "250Hz";
        parameter.symbol     = "band12";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain13:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "315Hz";
        parameter.symbol     = "band13";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain14:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "400Hz";
        parameter.symbol     = "band14";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain15:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "500Hz";
        parameter.symbol     = "band15";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain16:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "630Hz";
        parameter.symbol     = "band16";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain17:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "800Hz";
        parameter.symbol     = "band17";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain18:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "1000Hz";
        parameter.symbol     = "band18";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain19:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "1250Hz";
        parameter.symbol     = "band19";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain20:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "1600Hz";
        parameter.symbol     = "band20";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain21:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "2000Hz";
        parameter.symbol     = "band21";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain22:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "2500Hz";
        parameter.symbol     = "band22";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain23:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "3150Hz";
        parameter.symbol     = "band23";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain24:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "4000Hz";
        parameter.symbol     = "band24";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain25:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "5000Hz";
        parameter.symbol     = "band25";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain26:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "6300Hz";
        parameter.symbol     = "band26";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain27:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "8000Hz";
        parameter.symbol     = "band27";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain28:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "10kHz";
        parameter.symbol     = "band28";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain29:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "12.5kHz";
        parameter.symbol     = "band29";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain30:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "16kHz";
        parameter.symbol     = "band30";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGain31:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "20kHz";
        parameter.symbol     = "band31";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 20.0f;
        break;
    }
}

void ZamGEQ31Plugin::d_initProgramName(uint32_t index, d_string& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamGEQ31Plugin::d_getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramQ:
        return q;
        break;
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
    case paramGain30:
        return gain[29];
        break;
    case paramGain31:
        return gain[30];
        break;
    default:
        return 0.0f;
    }
}

void ZamGEQ31Plugin::d_setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramQ:
        q = value;
        break;
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
    case paramGain30:
        gain[29] = value;
        break;
    case paramGain31:
        gain[30] = value;
        break;
    }
}

void ZamGEQ31Plugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    int i;
    /* Default parameter values */
    for (i = 0; i < MAX_FILT; i++) {
    	gain[i] = 0.0f;
    }

    freq[0] = 20.;
    freq[1] = 25.;
    freq[2] = 31.5;
    freq[3] = 40.;
    freq[4] = 50.;
    freq[5] = 63.;
    freq[6] = 80.;
    freq[7] = 100.;
    freq[8] = 125.;
    freq[9] = 160.;
    freq[10] = 200.;
    freq[11] = 250.;
    freq[12] = 315.;
    freq[13] = 400.;
    freq[14] = 500.;
    freq[15] = 630.;
    freq[16] = 800.;
    freq[17] = 1000.;
    freq[18] = 1250.;
    freq[19] = 1600.;
    freq[20] = 2000.;
    freq[21] = 2500.;
    freq[22] = 3150.;
    freq[23] = 4000.;
    freq[24] = 5000.;
    freq[25] = 6300.;
    freq[26] = 8000.;
    freq[27] = 10000.;
    freq[28] = 12500.;
    freq[29] = 16000.;
    freq[30] = 20000.;
    q = 1.4f;
    master = 0.f;

    /* reset filter values */
    d_activate();
}

// -----------------------------------------------------------------------
// Process

void ZamGEQ31Plugin::d_activate()
{
        int i;
	for (i = 0; i < MAX_FILT; ++i) {
		x1[0][i] = x2[0][i] = 0.f;
		y1[0][i] = y2[0][i] = 0.f;
		a[0][i] = b[0][i] = g[0][i] = 0.f;
        }
}

void ZamGEQ31Plugin::geq31(int i, int ch, float srate, float fc, float q)
{
	float t0;

	t0 = 2.*M_PI*fc/srate;
	b[ch][i] = (q - t0*0.5)/(2.*q+t0);
	a[ch][i] = (0.5 - b[ch][i])*0.5;
	g[ch][i] = (0.5 + b[ch][i])*cos(t0);
}

float ZamGEQ31Plugin::run_filter(int i, int ch, double in)
{
	double out;
	in = sanitize_denormal(in);
	out = 2.*(a[ch][i]*(in - x2[ch][i]) + g[ch][i]*y1[ch][i] - b[ch][i]*y2[ch][i]) + 1e-20f;
	out = sanitize_denormal(out);
	x2[ch][i] = sanitize_denormal(x1[ch][i]);
	y2[ch][i] = sanitize_denormal(y1[ch][i]);
	x1[ch][i] = in;
	y1[ch][i] = out;

	return (float) out;
}

void ZamGEQ31Plugin::d_run(const float** inputs, float** outputs, uint32_t frames)
{
	float srate = d_getSampleRate();
	
	int i;
	for (i = 0; i < MAX_FILT; i++) {
		geq31(i, 0, srate, freq[i], q);
	}

        for (uint32_t i = 0; i < frames; i++) {
                double tmp;
                double in = inputs[0][i];
                in = sanitize_denormal(in);
		tmp = 0.;

                for (int j = 0; j < MAX_FILT; j++) {
                	tmp += from_dB(gain[j]) * run_filter(j, 0, in);
		}

                outputs[0][i] = inputs[0][i];
                outputs[0][i] = (float) tmp;
		outputs[0][i] *= from_dB(master);
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamGEQ31Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
