/*
 * ZaMultiCompX2 Stereo multiband compressor
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

#include "ZaMultiCompX2Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZaMultiCompX2Plugin::ZaMultiCompX2Plugin()
    : Plugin(paramCount, 2, 0)
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZaMultiCompX2Plugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramAttack:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Attack";
        parameter.symbol     = "att";
        parameter.unit       = "ms";
        parameter.ranges.def = 10.0f;
        parameter.ranges.min = 0.1f;
        parameter.ranges.max = 100.0f;
        break;
    case paramRelease:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Release";
        parameter.symbol     = "rel";
        parameter.unit       = "ms";
        parameter.ranges.def = 80.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 500.0f;
        break;
    case paramKnee:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Knee";
        parameter.symbol     = "kn";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 8.0f;
        break;
    case paramRatio:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Ratio";
        parameter.symbol     = "rat";
        parameter.unit       = " ";
        parameter.ranges.def = 4.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramThresh1:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Threshold 1";
        parameter.symbol     = "thr1";
        parameter.unit       = "dB";
        parameter.ranges.def = -20.0f;
        parameter.ranges.min = -60.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramThresh2:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Threshold 2";
        parameter.symbol     = "thr2";
        parameter.unit       = "dB";
        parameter.ranges.def = -18.0f;
        parameter.ranges.min = -60.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramThresh3:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Threshold 3";
        parameter.symbol     = "thr3";
        parameter.unit       = "dB";
        parameter.ranges.def = -16.0f;
        parameter.ranges.min = -60.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramMakeup1:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Makeup 1";
        parameter.symbol     = "mak1";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramMakeup2:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Makeup 2";
        parameter.symbol     = "mak2";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramMakeup3:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Makeup 3";
        parameter.symbol     = "mak3";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramGainR1:
        parameter.hints      = kParameterIsAutomable | kParameterIsOutput;
        parameter.name       = "Gain Reduction 1";
        parameter.symbol     = "gr1";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGainR2:
        parameter.hints      = kParameterIsAutomable | kParameterIsOutput;
        parameter.name       = "Gain Reduction 2";
        parameter.symbol     = "gr2";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGainR3:
        parameter.hints      = kParameterIsAutomable | kParameterIsOutput;
        parameter.name       = "Gain Reduction 3";
        parameter.symbol     = "gr3";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramXover1:
        parameter.hints      = kParameterIsAutomable | kParameterIsLogarithmic;
        parameter.name       = "Crossover freq 1";
        parameter.symbol     = "xover1";
        parameter.unit       = "Hz";
        parameter.ranges.def = 160.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 1400.0f;
        break;
    case paramXover2:
        parameter.hints      = kParameterIsAutomable | kParameterIsLogarithmic;
        parameter.name       = "Crossover freq 2";
        parameter.symbol     = "xover2";
        parameter.unit       = "Hz";
        parameter.ranges.def = 1400.0f;
        parameter.ranges.min = 1400.0f;
        parameter.ranges.max = 14000.0f;
        break;
    case paramToggle1:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "ZamComp 1 ON";
        parameter.symbol     = "toggle1";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramToggle2:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "ZamComp 2 ON";
        parameter.symbol     = "toggle2";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramToggle3:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "ZamComp 3 ON";
        parameter.symbol     = "toggle3";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramListen1:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Listen 1";
        parameter.symbol     = "listen1";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramListen2:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Listen 2";
        parameter.symbol     = "listen2";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramListen3:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Listen 3";
        parameter.symbol     = "listen3";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramGlobalGain:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Master Trim";
        parameter.symbol     = "globalgain";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramStereoDet:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Detection (MAX/avg)";
        parameter.symbol     = "stereodet";
        parameter.unit       = " ";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramOutputLevelL:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Output Left";
        parameter.symbol     = "outl";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramOutputLevelR:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Output Right";
        parameter.symbol     = "outr";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramOutputLevelLow:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Output low";
        parameter.symbol     = "outlo";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramOutputLevelMed:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Output medium";
        parameter.symbol     = "outmed";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramOutputLevelHigh:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Output high";
        parameter.symbol     = "outhi";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 20.0f;
        break;
    }
}

void ZaMultiCompX2Plugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "Zero";
		break;
	case 1:
		programName = "Presence";
		break;
	}
}

void ZaMultiCompX2Plugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		attack = 10.0;
		release = 80.0;
		knee = 0.0;
		ratio = 4.0;
		thresdb[0] = -20.0;
		thresdb[1] = -18.0;
		thresdb[2] = -16.0;
		makeup[0] = 0.0;
		makeup[1] = 0.0;
		makeup[2] = 0.0;
		xover1 = 160.0;
		xover2 = 1400.0;
		gainr[0] = 0.0;
		gainr[1] = 0.0;
		gainr[2] = 0.0;
		toggle[0] = 0.0;
		toggle[1] = 0.0;
		toggle[2] = 0.0;
		listen[0] = 0.0;
		listen[1] = 0.0;
		listen[2] = 0.0;
		stereodet = 1.0;
		globalgain = 0.0;
		outl = -45.0;
		outr = -45.0;
		outlevel[0] = -45;
		outlevel[1] = -45;
		outlevel[2] = -45;
		break;
	case 1:
		attack = 10.0;
		release = 200.0;
		knee = 3.0;
		ratio = 5.0;
		thresdb[0] = -20.0;
		thresdb[1] = -18.0;
		thresdb[2] = -16.0;
		makeup[0] = 9.0;
		makeup[1] = 6.0;
		makeup[2] = 6.0;
		xover1 = 160.0;
		xover2 = 1400.0;
		gainr[0] = 0.0;
		gainr[1] = 0.0;
		gainr[2] = 0.0;
		toggle[0] = 1.0;
		toggle[1] = 1.0;
		toggle[2] = 1.0;
		listen[0] = 0.0;
		listen[1] = 0.0;
		listen[2] = 0.0;
		stereodet = 1.0;
		globalgain = 0.0;
		outl = -45.0;
		outr = -45.0;
		outlevel[0] = -45.0;
		outlevel[1] = -45.0;
		outlevel[2] = -45.0;
		break;
	}
    /* Default variable values */
    maxL = 0.f;
    maxR = 0.f;
    limit = 0.f;

    /* reset filter values */
    activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZaMultiCompX2Plugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramAttack:
        return attack;
        break;
    case paramRelease:
        return release;
        break;
    case paramKnee:
        return knee;
        break;
    case paramRatio:
        return ratio;
        break;
    case paramThresh1:
        return thresdb[0];
        break;
    case paramThresh2:
        return thresdb[1];
        break;
    case paramThresh3:
        return thresdb[2];
        break;
    case paramMakeup1:
        return makeup[0];
        break;
    case paramMakeup2:
        return makeup[1];
        break;
    case paramMakeup3:
        return makeup[2];
        break;
    case paramGainR1:
        return gainr[0];
        break;
    case paramGainR2:
        return gainr[1];
        break;
    case paramGainR3:
        return gainr[2];
        break;
    case paramXover1:
        return xover1;
        break;
    case paramXover2:
        return xover2;
        break;
    case paramToggle1:
        return toggle[0];
        break;
    case paramToggle2:
        return toggle[1];
        break;
    case paramToggle3:
        return toggle[2];
        break;
    case paramListen1:
        return listen[0];
        break;
    case paramListen2:
        return listen[1];
        break;
    case paramListen3:
        return listen[2];
        break;
    case paramGlobalGain:
        return globalgain;
        break;
    case paramStereoDet:
        return stereodet;
        break;
    case paramOutputLevelL:
        return outl;
        break;
    case paramOutputLevelR:
        return outr;
        break;
    case paramOutputLevelLow:
        return outlevel[0];
        break;
    case paramOutputLevelMed:
        return outlevel[1];
        break;
    case paramOutputLevelHigh:
        return outlevel[2];
        break;
    default:
        return 0.0f;
    }
}

void ZaMultiCompX2Plugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramAttack:
        attack = value;
        break;
    case paramRelease:
        release = value;
        break;
    case paramKnee:
        knee = value;
        break;
    case paramRatio:
        ratio = value;
        break;
    case paramThresh1:
        thresdb[0] = value;
        break;
    case paramThresh2:
        thresdb[1] = value;
        break;
    case paramThresh3:
        thresdb[2] = value;
        break;
    case paramMakeup1:
        makeup[0] = value;
        break;
    case paramMakeup2:
        makeup[1] = value;
        break;
    case paramMakeup3:
        makeup[2] = value;
        break;
    case paramGainR1:
        gainr[0] = value;
        break;
    case paramGainR2:
        gainr[1] = value;
        break;
    case paramGainR3:
        gainr[2] = value;
        break;
    case paramXover1:
        xover1 = value;
        break;
    case paramXover2:
        xover2 = value;
        break;
    case paramToggle1:
        toggle[0] = value;
        if (value == 0.f)
	    gainr[0] = 0.f;
        break;
    case paramToggle2:
        toggle[1] = value;
        if (value == 0.f)
	    gainr[1] = 0.f;
        break;
    case paramToggle3:
        toggle[2] = value;
        if (value == 0.f)
	    gainr[2] = 0.f;
        break;
    case paramListen1:
        listen[0] = value;
        if (value == 0.f)
	    gainr[0] = 0.f;
        break;
    case paramListen2:
        listen[1] = value;
        if (value == 0.f)
	    gainr[1] = 0.f;
        break;
    case paramListen3:
        listen[2] = value;
        if (value == 0.f)
	    gainr[2] = 0.f;
        break;
    case paramGlobalGain:
        globalgain = value;
        break;
    case paramStereoDet:
        stereodet = value;
        break;
    case paramOutputLevelL:
        outl = value;
        break;
    case paramOutputLevelR:
        outr = value;
        break;
    case paramOutputLevelLow:
        outlevel[0] = value;
        break;
    case paramOutputLevelMed:
        outlevel[1] = value;
        break;
    case paramOutputLevelHigh:
        outlevel[2] = value;
        break;
    }
}

void ZaMultiCompX2Plugin::setState(const char*, const char*)
{
    resetl = true;
    resetr = true;
}

void ZaMultiCompX2Plugin::initState(unsigned int, String&, String&)
{
}

// -----------------------------------------------------------------------
// Process

void ZaMultiCompX2Plugin::activate()
{
        int i,j;
        for (i = 0; i < MAX_COMP; i++)
        	for (j = 0; j < 2; j++)
                	old_yl[j][i]=old_y1[j][i]=old_yg[j][i]=0.f;

	for (j = 0; j < 2; j++)
		old_ll[j]=old_l1[j]=0.f;

	for (i = 0; i < MAX_FILT; i++) {
        	for (j = 0; j < 2; j++) {
                	a0[j][i] = a1[j][i] = a2[j][i] = 0.f;
                	b1[j][i] = b2[j][i] = 0.f;
                	w1[j][i] = w2[j][i] = 0.f;
                	z1[j][i] = z2[j][i] = 0.f;
		}
        }
	maxL = maxR = 0.f;
	for (i = 0; i < MAX_SAMPLES; i++) {
		outlevelold[0][i] = 0.f;
		outlevelold[1][i] = 0.f;
		outlevelold[2][i] = 0.f;
	}
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
}

float ZaMultiCompX2Plugin::run_filter(int i, int ch, float in)
{
        in = sanitize_denormal(in);
        w1[ch][i] = sanitize_denormal(w1[ch][i]);
        w2[ch][i] = sanitize_denormal(w2[ch][i]);
	z1[ch][i] = sanitize_denormal(z1[ch][i]);
	z2[ch][i] = sanitize_denormal(z2[ch][i]);

        float out = in * a0[ch][i] 	+ w1[ch][i] * a1[ch][i] + w2[ch][i] * a2[ch][i]
					- z1[ch][i] * b1[ch][i] - z2[ch][i] * b2[ch][i];
	out = sanitize_denormal(out);
        w2[ch][i] = w1[ch][i];
        z2[ch][i] = z1[ch][i];
        w1[ch][i] = in;
	z1[ch][i] = out;
        return out;
}

void ZaMultiCompX2Plugin::set_lp_coeffs(float fc, float q, float sr, int i, int ch, float gain=1.0)
{
        float omega=(float)(2.f*M_PI*fc/sr);
        float sn=sin(omega);
        float cs=cos(omega);
        float alpha=(float)(sn/(2.f*q));
        float inv=(float)(1.0/(1.0+alpha));

        a2[ch][i] = a0[ch][i] =  (float)(gain*inv*(1.f - cs)*0.5f);
        a1[ch][i] = a0[ch][i] + a0[ch][i];
        b1[ch][i] = (float)(-2.f*cs*inv);
        b2[ch][i] = (float)((1.f - alpha)*inv);
}

void ZaMultiCompX2Plugin::set_hp_coeffs(float fc, float q, float sr, int i, int ch, float gain=1.0)
{
        float omega=(float)(2.f*M_PI*fc/sr);
        float sn=sin(omega);
        float cs=cos(omega);
        float alpha=(float)(sn/(2.f*q));
        float inv=(float)(1.f/(1.f+alpha));

        a0[ch][i] =  (float)(gain*inv*(1.f + cs)/2.f);
        a1[ch][i] =  -2.f * a0[ch][i];
        a2[ch][i] =  a0[ch][i];
        b1[ch][i] =  (float)(-2.f*cs*inv);
        b2[ch][i] =  (float)((1.f - alpha)*inv);
}

void ZaMultiCompX2Plugin::run_limit(float inL, float inR, float *outL, float *outR)
{
	float srate = getSampleRate();
        float width=0.01;
	float threshdb = -0.5;
        float attack_coeff = exp(-1000.f/(0.001 * srate));
        float release_coeff = exp(-1000.f/(50.0 * srate));

        float cdb=0.f;
        float Lgain = 1.f;
        float Rgain = 1.f;
        float Lxg, Lyg;
        float Rxg, Ryg;
        float Lxl, Lyl, Ly1;
        float Rxl, Ryl, Ry1;

        Lyg = Ryg = 0.f;
	inL = sanitize_denormal(inL);
	inR = sanitize_denormal(inR);
        Lxg = (inL==0.f) ? -160.f : to_dB(fabs(inL));
        Rxg = (inR==0.f) ? -160.f : to_dB(fabs(inR));
        Lxg = sanitize_denormal(Lxg);
        Rxg = sanitize_denormal(Rxg);

        Lyg = Lxg + (1.f/100.0-1.f)*(Lxg-threshdb+width/2.f)*(Lxg-threshdb+width/2.f)/(2.f*width);
        Lyg = sanitize_denormal(Lyg);
        Ryg = Rxg + (1.f/100.0-1.f)*(Rxg-threshdb+width/2.f)*(Rxg-threshdb+width/2.f)/(2.f*width);
        Ryg = sanitize_denormal(Ryg);

        if (2.f*(Lxg-threshdb) < -width) {
                Lyg = Lxg;
        } else {
                Lyg = threshdb + (Lxg-threshdb)/100.0;
                Lyg = sanitize_denormal(Lyg);
        }

        if (2.f*(Rxg-threshdb) < -width) {
                Ryg = Rxg;
        } else {
                Ryg = threshdb + (Rxg-threshdb)/100.0;
        	Ryg = sanitize_denormal(Ryg);
        }

        Lxl = Rxl = fmaxf(Lxg - Lyg, Rxg - Ryg);

        old_l1[0] = sanitize_denormal(old_l1[0]);
        old_l1[1] = sanitize_denormal(old_l1[1]);
        old_ll[0] = sanitize_denormal(old_ll[0]);
        old_ll[1] = sanitize_denormal(old_ll[1]);

        Ly1 = fmaxf(Lxl, release_coeff * old_l1[0]+(1.f-release_coeff)*Lxl);
        Lyl = attack_coeff * old_ll[0]+(1.f-attack_coeff)*Ly1;
        Ly1 = sanitize_denormal(Ly1);
        Lyl = sanitize_denormal(Lyl);

        cdb = -Lyl;
        Lgain = from_dB(cdb);

        Ry1 = fmaxf(Rxl, release_coeff * old_l1[1]+(1.f-release_coeff)*Rxl);
        Ryl = attack_coeff * old_ll[1]+(1.f-attack_coeff)*Ry1;
        Ry1 = sanitize_denormal(Ry1);
        Ryl = sanitize_denormal(Ryl);

        cdb = -Ryl;
        Rgain = from_dB(cdb);

	*outL = inL;
	*outL *= Lgain;
	*outR = inR;
	*outR *= Rgain;

	limit = fmaxf(Lyl, Ryl);

        old_ll[0] = Lyl;
        old_ll[1] = Ryl;
        old_l1[0] = Ly1;
        old_l1[1] = Ry1;
}

void ZaMultiCompX2Plugin::run_comp(int k, float inL, float inR, float *outL, float *outR)
{
	float srate = getSampleRate();
        float width = (6.f * knee) + 0.01;
        float slewwidth = 1.8f;
        float attack_coeff = exp(-1000.f/(attack * srate));
        float release_coeff = exp(-1000.f/(release * srate));
	int stereolink = (stereodet > 0.5f) ? STEREOLINK_MAX : STEREOLINK_AVERAGE;
	int attslew = 0;
	int relslew = 0;
	float checkwidth = 0.f;
        float cdb=0.f;
        float Lgain = 1.f;
        float Rgain = 1.f;
        float Lxg, Lyg;
        float Rxg, Ryg;
        float Lxl, Lyl, Ly1;
        float Rxl, Ryl, Ry1;

        Lyg = Ryg = 0.f;
	inL = sanitize_denormal(inL);
	inR = sanitize_denormal(inR);
        Lxg = (inL==0.f) ? -160.f : to_dB(fabs(inL));
        Rxg = (inR==0.f) ? -160.f : to_dB(fabs(inR));
        Lxg = sanitize_denormal(Lxg);
        Rxg = sanitize_denormal(Rxg);

	checkwidth = 2.f*fabs(Lxg-thresdb[k]);
	if (2.f*(Lxg-thresdb[k]) < -width) {
		Lyg = Lxg;
	} else if (checkwidth <= width) {
		Lyg = Lxg + (1.f/ratio-1.f)*(Lxg-thresdb[k]+width/2.f)*(Lxg-thresdb[k]+width/2.f)/(2.f*width);
		//Lyg = thresdb[k] + (Lxg-thresdb[k])/ratio;
		Lyg = sanitize_denormal(Lyg);
		if (checkwidth <= slewwidth) {
			if (Lyg >= old_yg[0][k]) {
				attslew = 1;
			} else {
				relslew = 1;
			}
		}
	} else if (2.f*(Lxg-thresdb[k]) > width) {
		Lyg = thresdb[k] + (Lxg-thresdb[k])/ratio;
		Lyg = sanitize_denormal(Lyg);
	}

	checkwidth = 2.f*fabs(Rxg-thresdb[k]);
	if (2.f*(Rxg-thresdb[k]) < -width) {
		Ryg = Rxg;
	} else if (checkwidth <= width) {
		Ryg = Rxg + (1.f/ratio-1.f)*(Rxg-thresdb[k]+width/2.f)*(Rxg-thresdb[k]+width/2.f)/(2.f*width);
		//Ryg = thresdb[k] + (Rxg-thresdb[k])/ratio;
		Ryg = sanitize_denormal(Ryg);
		if (checkwidth <= slewwidth) {
			if (Ryg >= old_yg[1][k]) {
				attslew = 1;
			} else {
				relslew = 1;
			}
		}
	} else if (2.f*(Rxg-thresdb[k]) > width) {
		Ryg = thresdb[k] + (Rxg-thresdb[k])/ratio;
		Ryg = sanitize_denormal(Ryg);
	}

	//attack_coeff = attslew ? exp(-1000.f/((attack + 2.0*(slewfactor - 1)) * srate)) : attack_coeff;
	//release_coeff = relslew ? exp(-1000.f/((release + 2.0*(slewfactor - 1)) * srate)) : release_coeff;

        if (stereolink == STEREOLINK_MAX) {
                Lxl = Rxl = fmaxf(Lxg - Lyg, Rxg - Ryg);
        } else {
                Lxl = Rxl = (Lxg - Lyg + Rxg - Ryg) / 2.f;
        }

        old_y1[0][k] = sanitize_denormal(old_y1[0][k]);
        old_y1[1][k] = sanitize_denormal(old_y1[1][k]);
        old_yl[0][k] = sanitize_denormal(old_yl[0][k]);
        old_yl[1][k] = sanitize_denormal(old_yl[1][k]);


        Ly1 = fmaxf(Lxl, release_coeff * old_y1[0][k]+(1.f-release_coeff)*Lxl);
        Lyl = attack_coeff * old_yl[0][k]+(1.f-attack_coeff)*Ly1;
        Ly1 = sanitize_denormal(Ly1);
        Lyl = sanitize_denormal(Lyl);

        cdb = -Lyl;
        Lgain = from_dB(cdb);

        Ry1 = fmaxf(Rxl, release_coeff * old_y1[1][k]+(1.f-release_coeff)*Rxl);
        Ryl = attack_coeff * old_yl[1][k]+(1.f-attack_coeff)*Ry1;
        Ry1 = sanitize_denormal(Ry1);
        Ryl = sanitize_denormal(Ryl);

        cdb = -Ryl;
        Rgain = from_dB(cdb);

        if (stereolink == STEREOLINK_MAX)
		gainr[k] = fmaxf(Lyl, Ryl);
	else
        	gainr[k] = (Lyl + Ryl) / 2.f;

	*outL = inL;
	*outL *= Lgain;
	*outR = inR;
	*outR *= Rgain;

        old_yl[0][k] = Lyl;
        old_yl[1][k] = Ryl;
        old_y1[0][k] = Ly1;
        old_y1[1][k] = Ry1;
        old_yg[0][k] = Lyg;
        old_yg[1][k] = Ryg;
}

void ZaMultiCompX2Plugin::pushsample(float samples[], float sample, int k)
{
	++pos[k];
	if (pos[k] >= MAX_SAMPLES)
		pos[k] = 0;
	samples[pos[k]] = sample;
}

float ZaMultiCompX2Plugin::averageabs(float samples[])
{
	int i;
	float average = 0.f;

	for (i = 0; i < MAX_SAMPLES; i++) {
		average += samples[i]*samples[i];
	}
	average /= (float) MAX_SAMPLES;
	return sqrt(average);
}

void ZaMultiCompX2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	float srate = getSampleRate();
	float maxxL = maxL;
	float maxxR = maxR;

        int tog1 = (toggle[0] > 0.5f) ? 1 : 0;
        int tog2 = (toggle[1] > 0.5f) ? 1 : 0;
        int tog3 = (toggle[2] > 0.5f) ? 1 : 0;

        int listen1 = (listen[0] > 0.5f) ? 1 : 0;
        int listen2 = (listen[1] > 0.5f) ? 1 : 0;
        int listen3 = (listen[2] > 0.5f) ? 1 : 0;

        set_lp_coeffs(xover1, ONEOVERROOT2, srate, 0, 0);
        set_lp_coeffs(xover1, ONEOVERROOT2, srate, 1, 0);
        set_hp_coeffs(xover1, ONEOVERROOT2, srate, 2, 0);
        set_hp_coeffs(xover1, ONEOVERROOT2, srate, 3, 0);
        set_lp_coeffs(xover2, ONEOVERROOT2, srate, 4, 0);
        set_lp_coeffs(xover2, ONEOVERROOT2, srate, 5, 0);
        set_hp_coeffs(xover2, ONEOVERROOT2, srate, 6, 0);
        set_hp_coeffs(xover2, ONEOVERROOT2, srate, 7, 0);

        set_lp_coeffs(xover1, ONEOVERROOT2, srate, 0, 1);
        set_lp_coeffs(xover1, ONEOVERROOT2, srate, 1, 1);
        set_hp_coeffs(xover1, ONEOVERROOT2, srate, 2, 1);
        set_hp_coeffs(xover1, ONEOVERROOT2, srate, 3, 1);
        set_lp_coeffs(xover2, ONEOVERROOT2, srate, 4, 1);
        set_lp_coeffs(xover2, ONEOVERROOT2, srate, 5, 1);
        set_hp_coeffs(xover2, ONEOVERROOT2, srate, 6, 1);
        set_hp_coeffs(xover2, ONEOVERROOT2, srate, 7, 1);

        for (uint32_t i = 0; i < frames; ++i) {
                float tmp1[2], tmp2[2], tmp3[2], tmp4[2], tmp5[2], tmp6[2];
		float fil1[2], fil2[2], fil3[2], fil4[2];
		float outL[MAX_COMP+1] = {0.f};
		float outR[MAX_COMP+1] = {0.f};
		float inl = sanitize_denormal(inputs[0][i]);
		float inr = sanitize_denormal(inputs[1][i]);
		inl = (fabs(inl) < DANGER) ? inl : 0.f;
		inr = (fabs(inr) < DANGER) ? inr : 0.f;

		int listenmode = 0;

		// Interleaved channel processing
                fil1[0] = run_filter(0, 0, inl);
                fil1[1] = run_filter(0, 1, inr);
                tmp1[0] = run_filter(1, 0, fil1[0]);
                tmp1[1] = run_filter(1, 1, fil1[1]);
		pushsample(outlevelold[0], std::max(tmp1[0], tmp1[1]), 0);
		outlevel[0] = averageabs(outlevelold[0]);
		outlevel[0] = (outlevel[0] == 0.f) ? -45.0 : to_dB(outlevel[0]);
		if (tog1)
			run_comp(0, tmp1[0], tmp1[1], &outL[0], &outR[0]);

		tmp2[0] = tog1 ? outL[0] * from_dB(makeup[0]) : tmp1[0];
                tmp2[1] = tog1 ? outR[0] * from_dB(makeup[0]) : tmp1[1];

                fil2[0] = run_filter(2, 0, inl);
                fil2[1] = run_filter(2, 1, inr);
                tmp3[0] = run_filter(3, 0, fil2[0]);
                tmp3[1] = run_filter(3, 1, fil2[1]);
                fil3[0] = run_filter(4, 0, tmp3[0]);
                fil3[1] = run_filter(4, 1, tmp3[1]);
                tmp4[0] = run_filter(5, 0, fil3[0]);
                tmp4[1] = run_filter(5, 1, fil3[1]);
		pushsample(outlevelold[1], std::max(tmp4[0], tmp4[1]), 1);
		outlevel[1] = averageabs(outlevelold[1]);
		outlevel[1] = (outlevel[1] == 0.f) ? -45.0 : to_dB(outlevel[1]);

		if (tog2)
			run_comp(1, tmp4[0], tmp4[1], &outL[1], &outR[1]);

                tmp3[0] = tog2 ? outL[1] * from_dB(makeup[1]) : tmp4[0];
                tmp3[1] = tog2 ? outR[1] * from_dB(makeup[1]) : tmp4[1];

                fil4[0] = run_filter(6, 0, inl);
                fil4[1] = run_filter(6, 1, inr);
                tmp5[0] = run_filter(7, 0, fil4[0]);
                tmp5[1] = run_filter(7, 1, fil4[1]);
		pushsample(outlevelold[2], std::max(tmp5[0], tmp5[1]), 2);
		outlevel[2] = averageabs(outlevelold[2]);
		outlevel[2] = (outlevel[2] == 0.f) ? -45.0 : to_dB(outlevel[2]);

		if (tog3)
			run_comp(2, tmp5[0], tmp5[1], &outL[2], &outR[2]);

                tmp6[0] = tog3 ? outL[2] * from_dB(makeup[2]) : tmp5[0];
                tmp6[1] = tog3 ? outR[2] * from_dB(makeup[2]) : tmp5[1];

		outputs[0][i] = outputs[1][i] = 0.f;
		if (listen1) {
			listenmode = 1;
			outputs[0][i] += outL[0] * tog1*from_dB(makeup[0])
					+ (1.-tog1) * tmp1[0];
			outputs[1][i] += outR[0] * tog1*from_dB(makeup[0])
					+ (1.-tog1) * tmp1[1];
		}
		if (listen2) {
			listenmode = 1;
			outputs[0][i] += outL[1] * tog2*from_dB(makeup[1])
					+ (1.-tog2) * tmp4[0];
			outputs[1][i] += outR[1] * tog2*from_dB(makeup[1])
					+ (1.-tog2) * tmp4[1];
		}
		if (listen3) {
			listenmode = 1;
			outputs[0][i] += outL[2] * tog3*from_dB(makeup[2])
					+ (1.-tog3) * tmp5[0];
			outputs[1][i] += outR[2] * tog3*from_dB(makeup[2])
					+ (1.-tog3) * tmp5[1];
		}
		if (!listenmode) {
                	outputs[0][i] = tmp2[0] + tmp3[0] + tmp6[0];
                	outputs[1][i] = tmp2[1] + tmp3[1] + tmp6[1];
		}
                outputs[0][i] = sanitize_denormal(outputs[0][i]);
                outputs[1][i] = sanitize_denormal(outputs[1][i]);
                outputs[0][i] *= from_dB(globalgain);
                outputs[1][i] *= from_dB(globalgain);

		tmp1[0] = outputs[0][i];
		tmp1[1] = outputs[1][i];
		run_limit(tmp1[0], tmp1[1], &outL[3], &outR[3]);
		outputs[0][i] = outL[3];
		outputs[1][i] = outR[3];

		if (resetl) {
			maxL = fabsf(outputs[0][i]);
			resetl = false;
		} else {
			maxxL = (fabsf(outputs[0][i]) > maxxL) ? fabsf(outputs[0][i]) : sanitize_denormal(maxxL);
		}
		if (resetr) {
			maxR = fabsf(outputs[1][i]);
			resetr = false;
		} else {
			maxxR = (fabsf(outputs[1][i]) > maxxR) ? fabsf(outputs[1][i]) : sanitize_denormal(maxxR);
		}
        }
	outl = (maxxL <= 0.f) ? -160.f : to_dB(maxxL+limit);
	outr = (maxxR <= 0.f) ? -160.f : to_dB(maxxR+limit);
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZaMultiCompX2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
