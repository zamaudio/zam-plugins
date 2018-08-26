/*
 * ZaMultiComp multiband compressor
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

#include "ZaMultiCompPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZaMultiCompPlugin::ZaMultiCompPlugin()
    : Plugin(paramCount, 2, 0)
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZaMultiCompPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramAttack1:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Attack1";
        parameter.symbol     = "att1";
        parameter.unit       = "ms";
        parameter.ranges.def = 10.0f;
        parameter.ranges.min = 0.1f;
        parameter.ranges.max = 100.0f;
        break;
    case paramAttack2:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Attack2";
        parameter.symbol     = "att2";
        parameter.unit       = "ms";
        parameter.ranges.def = 10.0f;
        parameter.ranges.min = 0.1f;
        parameter.ranges.max = 100.0f;
        break;
    case paramAttack3:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Attack3";
        parameter.symbol     = "att3";
        parameter.unit       = "ms";
        parameter.ranges.def = 10.0f;
        parameter.ranges.min = 0.1f;
        parameter.ranges.max = 100.0f;
        break;
    case paramRelease1:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Release1";
        parameter.symbol     = "rel1";
        parameter.unit       = "ms";
        parameter.ranges.def = 80.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 500.0f;
        break;
    case paramRelease2:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Release2";
        parameter.symbol     = "rel2";
        parameter.unit       = "ms";
        parameter.ranges.def = 80.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 500.0f;
        break;
    case paramRelease3:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Release3";
        parameter.symbol     = "rel3";
        parameter.unit       = "ms";
        parameter.ranges.def = 80.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 500.0f;
        break;
    case paramKnee1:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Knee1";
        parameter.symbol     = "kn1";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 8.0f;
        break;
    case paramKnee2:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Knee2";
        parameter.symbol     = "kn2";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 8.0f;
        break;
    case paramKnee3:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Knee3";
        parameter.symbol     = "kn3";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 8.0f;
        break;
    case paramRatio1:
        parameter.hints      = kParameterIsAutomable | kParameterIsLogarithmic;
        parameter.name       = "Ratio1";
        parameter.symbol     = "rat1";
        parameter.unit       = " ";
        parameter.ranges.def = 4.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramRatio2:
        parameter.hints      = kParameterIsAutomable | kParameterIsLogarithmic;
        parameter.name       = "Ratio2";
        parameter.symbol     = "rat2";
        parameter.unit       = " ";
        parameter.ranges.def = 4.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramRatio3:
        parameter.hints      = kParameterIsAutomable | kParameterIsLogarithmic;
        parameter.name       = "Ratio3";
        parameter.symbol     = "rat3";
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
    case paramOutputLevel:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Output";
        parameter.symbol     = "out";
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

void ZaMultiCompPlugin::initProgramName(uint32_t index, String& programName)
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

void ZaMultiCompPlugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		attack[0] = 10.0;
		attack[1] = 10.0;
		attack[2] = 10.0;
		release[0] = 80.0;
		release[1] = 80.0;
		release[2] = 80.0;
		knee[0] = 0.0;
		knee[1] = 0.0;
		knee[2] = 0.0;
		ratio[0] = 4.0;
		ratio[1] = 4.0;
		ratio[2] = 4.0;
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
		globalgain = 0.0;
		out = -45.0;
		outlevel[0] = -45;
		outlevel[1] = -45;
		outlevel[2] = -45;
		break;
	case 1:
		attack[0] = 10.0;
		attack[1] = 10.0;
		attack[2] = 10.0;
		release[0] = 200.0;
		release[1] = 200.0;
		release[2] = 200.0;
		knee[0] = 3.0;
		knee[1] = 3.0;
		knee[2] = 3.0;
		ratio[0] = 5.0;
		ratio[1] = 5.0;
		ratio[2] = 5.0;
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
		globalgain = 0.0;
		out = -45.0;
		outlevel[0] = -45.0;
		outlevel[1] = -45.0;
		outlevel[2] = -45.0;
		break;
	}
    /* Default variable values */
    max = 0.f;
    limit = 0.f;

    /* reset filter values */
    activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZaMultiCompPlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramAttack1:
        return attack[0];
        break;
    case paramAttack2:
        return attack[1];
        break;
    case paramAttack3:
        return attack[2];
        break;
    case paramRelease1:
        return release[0];
        break;
    case paramRelease2:
        return release[1];
        break;
    case paramRelease3:
        return release[2];
        break;
    case paramKnee1:
        return knee[0];
        break;
    case paramKnee2:
        return knee[1];
        break;
    case paramKnee3:
        return knee[2];
        break;
    case paramRatio1:
        return ratio[0];
        break;
    case paramRatio2:
        return ratio[1];
        break;
    case paramRatio3:
        return ratio[2];
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
    case paramOutputLevel:
        return out;
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

void ZaMultiCompPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramAttack1:
        attack[0] = value;
        break;
    case paramAttack2:
        attack[1] = value;
        break;
    case paramAttack3:
        attack[2] = value;
        break;
    case paramRelease1:
        release[0] = value;
        break;
    case paramRelease2:
        release[1] = value;
        break;
    case paramRelease3:
        release[2] = value;
        break;
    case paramKnee1:
        knee[0] = value;
        break;
    case paramKnee2:
        knee[1] = value;
        break;
    case paramKnee3:
        knee[2] = value;
        break;
    case paramRatio1:
        ratio[0] = value;
        break;
    case paramRatio2:
        ratio[1] = value;
        break;
    case paramRatio3:
        ratio[2] = value;
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
    case paramOutputLevel:
        out = value;
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

String ZaMultiCompPlugin::getState(const char*) const
{
    return String();
}

void ZaMultiCompPlugin::setState(const char*, const char*)
{
    reset = true;
}

void ZaMultiCompPlugin::initState(unsigned int, String&, String&)
{
}

// -----------------------------------------------------------------------
// Process

void ZaMultiCompPlugin::activate()
{
        int i;
        for (i = 0; i < MAX_COMP; i++)
                old_yl[i]=old_y1[i]=old_yg[i]=0.f;

	old_ll=old_l1=0.f;

	for (i = 0; i < MAX_FILT; i++) {
		simper[0][i].k = 0.f;
		simper[0][i].g = 0.f;
		simper[0][i].s[0] = 0.f;
		simper[0][i].s[1] = 0.f;

		simper[1][i].k = 0.f;
		simper[1][i].g = 0.f;
		simper[1][i].s[0] = 0.f;
		simper[1][i].s[1] = 0.f;
	}
	max = 0.f;
	for (i = 0; i < MAX_SAMPLES; i++) {
		outlevelold[0][i] = 0.f;
		outlevelold[1][i] = 0.f;
		outlevelold[2][i] = 0.f;
	}
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;

	oldxover1 = 0.f;
	oldxover2 = 0.f;
}

/*
 * A. Simper's filters
 * https://cytomic.com/files/dsp/SvfInputMixing.pdf
 */

void ZaMultiCompPlugin::linear_svf_set_xover(struct linear_svf *self, float sample_rate, float cutoff, float resonance)
{
	double w;

	self->k = 2. - 2. * resonance;
	w = M_PI * cutoff / sample_rate;
	self->g = tan(w);

}

void ZaMultiCompPlugin::linear_svf_reset(struct linear_svf *self)
{
	self->s[0] = self->s[1] = 0.f;
}

float ZaMultiCompPlugin::run_linear_svf_xover(struct linear_svf *self, float in, float mixlow, float mixhigh)
{
	double v[3];
	double g = self->g;
	double k = self->k;
	double s0 = self->s[0];
	double s1 = self->s[1];
	double g2 = g*g;
	double vhigh = in * mixhigh;
	double vband = in * 0.75;
	double vlow = in * mixlow;

	v[0] = in;
	v[1] = -1. / (1. + g2 + g*k) * (-s0 + g*s1 - g*k*s0 + g2*vband + g*vhigh - g*vlow - g2*k*vlow);
	v[2] = -1. / (1. + g2 + g*k) * (-g*s0 - s1 - g*vband + g2*vhigh + g*k*vhigh - g2*vlow);
	self->s[0] = 2. * v[1] - s0;
	self->s[1] = 2. * v[2] - s1;

	return (float)(vhigh + v[2]);
}

void ZaMultiCompPlugin::calc_lr4(float f, int i)
{
	float srate = getSampleRate();

	linear_svf_set_xover(&simper[0][i], srate, f, 0.25);
	linear_svf_set_xover(&simper[1][i], srate, f, 0.25);
}

void ZaMultiCompPlugin::run_lr4(int i, float in, float *outlo, float *outhi)
{
	*outlo = run_linear_svf_xover(&simper[0][i], in, 1., 0.);
	*outhi = run_linear_svf_xover(&simper[1][i], in, 0., 1.);
}

void ZaMultiCompPlugin::run_comp(int k, float in, float *out)
{
	float srate = getSampleRate();
        float width = (6.f * knee[k]) + 0.01;
        float attack_coeff = exp(-1000.f/(attack[k] * srate));
        float release_coeff = exp(-1000.f/(release[k] * srate));

	float checkwidth = 0.f;
        float cdb=0.f;
        float Lgain = 1.f;
        float Lxg, Lyg;
        float Lxl, Lyl;

        Lyg = 0.f;
	in = sanitize_denormal(in);
        Lxg = (in==0.f) ? -160.f : to_dB(fabs(in));
        Lxg = sanitize_denormal(Lxg);

	checkwidth = 2.f*fabs(Lxg-thresdb[k]);
	if (2.f*(Lxg-thresdb[k]) < -width) {
		Lyg = Lxg;
	} else if (checkwidth <= width) {
		Lyg = Lxg + (1.f/ratio[k]-1.f)*(Lxg-thresdb[k]+width/2.f)*(Lxg-thresdb[k]+width/2.f)/(2.f*width);
		Lyg = sanitize_denormal(Lyg);
	} else if (2.f*(Lxg-thresdb[k]) > width) {
		Lyg = thresdb[k] + (Lxg-thresdb[k])/ratio[k];
		Lyg = sanitize_denormal(Lyg);
	}

        Lxl = Lxg - Lyg;

	old_yl[k] = sanitize_denormal(old_yl[k]);

	if (Lxl < old_yl[k]) {
		Lyl = release_coeff * old_yl[k] + (1.f-release_coeff)*Lxl;
	} else if (Lxl > old_yl[k]) {
		Lyl = attack_coeff * old_yl[k]+(1.f-attack_coeff)*Lxl;
	} else {
		Lyl = Lxl;
	}
        Lyl = sanitize_denormal(Lyl);

        cdb = -Lyl;
        Lgain = from_dB(cdb);

	gainr[k] = Lyl;

	*out = in * Lgain;

        old_yl[k] = Lyl;
        old_yg[k] = Lyg;
}

void ZaMultiCompPlugin::pushsample(float samples[], float sample, int k)
{
	++pos[k];
	if (pos[k] >= MAX_SAMPLES)
		pos[k] = 0;
	samples[pos[k]] = sample;
}

float ZaMultiCompPlugin::averageabs(float samples[])
{
	int i;
	float average = 0.f;

	for (i = 0; i < MAX_SAMPLES; i++) {
		average += samples[i]*samples[i];
	}
	average /= (float) MAX_SAMPLES;
	return sqrt(average);
}

void ZaMultiCompPlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	float maxx = max;

        int tog1 = (toggle[0] > 0.5f) ? 1 : 0;
        int tog2 = (toggle[1] > 0.5f) ? 1 : 0;
        int tog3 = (toggle[2] > 0.5f) ? 1 : 0;

        int listen1 = (listen[0] > 0.5f) ? 1 : 0;
        int listen2 = (listen[1] > 0.5f) ? 1 : 0;
        int listen3 = (listen[2] > 0.5f) ? 1 : 0;

        if (oldxover1 != xover1) {
		// recalculate coeffs
		calc_lr4(xover1, 0);
		oldxover1 = xover1;
	}
        if (oldxover2 != xover2) {
		// recalculate coeffs
		calc_lr4(xover2, 1);
		oldxover2 = xover2;
	}

        for (uint32_t i = 0; i < frames; ++i) {
                float tmp1[2], tmp2[2], tmp3[2];
		float fil1[2], fil2[2], fil3[2], fil4[2];
		float outL[MAX_COMP+1] = {0.f};
		float inl = sanitize_denormal(inputs[0][i]);
		inl = (fabs(inl) < DANGER) ? inl : 0.f;

		int listenmode = 0;

		// Interleaved channel processing
                run_lr4(0, inl, &fil1[0], &fil2[0]);
                run_lr4(1, fil2[0], &fil3[0], &fil4[0]);

		pushsample(outlevelold[0], fil1[0], 0);
		outlevel[0] = averageabs(outlevelold[0]);
		outlevel[0] = (outlevel[0] == 0.f) ? -45.0 : to_dB(outlevel[0]);
		if (tog1)
			run_comp(0, fil1[0], &outL[0]);

		tmp1[0] = tog1 ? outL[0] * from_dB(makeup[0]) : fil1[0];

		pushsample(outlevelold[1], fil3[0], 1);
		outlevel[1] = averageabs(outlevelold[1]);
		outlevel[1] = (outlevel[1] == 0.f) ? -45.0 : to_dB(outlevel[1]);
		if (tog2)
			run_comp(1, fil3[0], &outL[1]);

                tmp2[0] = tog2 ? outL[1] * from_dB(makeup[1]) : fil3[0];

		pushsample(outlevelold[2], fil4[0], 2);
		outlevel[2] = averageabs(outlevelold[2]);
		outlevel[2] = (outlevel[2] == 0.f) ? -45.0 : to_dB(outlevel[2]);
		if (tog3)
			run_comp(2, fil4[0], &outL[2]);

                tmp3[0] = tog3 ? outL[2] * from_dB(makeup[2]) : fil4[0];

		outputs[0][i] = 0.f;
		if (listen1) {
			listenmode = 1;
			outputs[0][i] += outL[0] * tog1*from_dB(makeup[0])
					+ (1.-tog1) * tmp1[0];
		}
		if (listen2) {
			listenmode = 1;
			outputs[0][i] += outL[1] * tog2*from_dB(makeup[1])
					+ (1.-tog2) * tmp2[0];
		}
		if (listen3) {
			listenmode = 1;
			outputs[0][i] += outL[2] * tog3*from_dB(makeup[2])
					+ (1.-tog3) * tmp3[0];
		}
		if (!listenmode) {
			outputs[0][i] = tmp1[0] + tmp2[0] + tmp3[0];
		}
                outputs[0][i] = sanitize_denormal(outputs[0][i]);
                outputs[0][i] *= from_dB(globalgain);

		if (reset) {
			max = fabsf(outputs[0][i]);
			reset = false;
		} else {
			maxx = (fabsf(outputs[0][i]) > maxx) ? fabsf(outputs[0][i]) : sanitize_denormal(maxx);
		}
        }
	out = (maxx <= 0.f) ? -160.f : to_dB(maxx);
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZaMultiCompPlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
