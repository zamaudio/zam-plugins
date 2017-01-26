/*
 * ZamDynamicEQ
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

#include "ZamDynamicEQPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamDynamicEQPlugin::ZamDynamicEQPlugin()
    : Plugin(paramCount, 2, 0)
{
    // set default values
    loadProgram(0);
}

void ZamDynamicEQPlugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "Zero";
		break;
	case 1:
		programName = "PoppySnare";
		break;
	case 2:
		programName = "VocalLeveller";
		break;
	}
}

// -----------------------------------------------------------------------
// Process

void ZamDynamicEQPlugin::reset_low()
{
    zln1=zln2=zld1=zld2=0.f;
}

void ZamDynamicEQPlugin::reset_high()
{
    zhn1=zhn2=zhd1=zhd2=0.f;
}

void ZamDynamicEQPlugin::reset_peak()
{
    x1a=x2a=y1a=y2a=0.f;
}

void ZamDynamicEQPlugin::activate()
{
    oldL_yl = oldL_y1 = oldL_yg = 0.f;

    gainy = 0.f;

    reset_low();
    reset_high();
    reset_peak();
    linear_svf_reset(&bandpass);
}

void ZamDynamicEQPlugin::initParameter(uint32_t index, Parameter& parameter)
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
        parameter.hints      = kParameterIsAutomable | kParameterIsLogarithmic;
        parameter.name       = "Ratio";
        parameter.symbol     = "rat";
        parameter.unit       = " ";
        parameter.ranges.def = 1.5f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 5.0f;
        break;
    case paramThresh:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Threshold";
        parameter.symbol     = "thr";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -80.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramMaxBoostCut:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Max Boost/Cut";
        parameter.symbol     = "max";
        parameter.unit       = "dB";
        parameter.ranges.def = 10.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 10.0f;
        break;
    case paramSlew:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Slew";
        parameter.symbol     = "slew";
        parameter.unit       = " ";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 150.0f;
        break;
    case paramSidechain:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Sidechain";
        parameter.symbol     = "sidech";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramToggleLow:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Low Shelf";
        parameter.symbol     = "togglelow";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramTogglePeak:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Peak";
        parameter.symbol     = "togglepeak";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramToggleHigh:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "High Shelf";
        parameter.symbol     = "togglehigh";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramDetectFreq:
        parameter.hints      = kParameterIsAutomable | kParameterIsLogarithmic;
        parameter.name       = "Detect Frequency";
        parameter.symbol     = "detectfreq";
        parameter.unit       = "Hz";
        parameter.ranges.def = 1000.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 16000.0f;
        break;
    case paramTargetFreq:
        parameter.hints      = kParameterIsAutomable | kParameterIsLogarithmic;
        parameter.name       = "Target Frequency";
        parameter.symbol     = "targetfreq";
        parameter.unit       = "Hz";
        parameter.ranges.def = 1000.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 16000.0f;
        break;
    case paramTargetWidth:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Target width";
        parameter.symbol     = "targetwidth";
        parameter.unit       = "oct";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 5.0f;
        break;
    case paramBoostCut:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Boost / Cut";
        parameter.symbol     = "boostcut";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramControlGain:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Control Gain";
        parameter.symbol     = "controlgain";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -10.0f;
        parameter.ranges.max = 0.0f;
        break;
    }
}

void ZamDynamicEQPlugin::initAudioPort(bool input, uint32_t index, AudioPort& port)
{
	Plugin::initAudioPort(input, index, port);

	if ((index == 1) && input) {
		port.hints |= kAudioPortIsSidechain;
		port.name = "Sidechain Input";
		port.symbol = "sidechain_in";
	}
}

void ZamDynamicEQPlugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		attack = 10.0;
		release = 80.0;
		knee = 0.0;
		ratio = 1.5;
		thresdb = 0.0;
		max = 10.0;
		slewfactor = 1.0;
		sidechain = 0.0;
		detectfreq = 1000.0;
		targetfreq = 1000.0;
		targetwidth = 1.0;
		controlgain = 0.0;
		boostcut = 0.0;
		togpeak = 1.0;
		toglow = 0.0;
		toghigh = 0.0;
		break;
	case 1:
		attack = 10.0;
		release = 10.0;
		knee = 1.0;
		ratio = 1.5;
		thresdb = -18.0;
		max = 10.0;
		slewfactor = 20.0;
		sidechain = 0.0;
		detectfreq = 1000.0;
		targetfreq = 1000.0;
		targetwidth = 1.0;
		controlgain = 0.0;
		boostcut = 0.0;
		togpeak = 1.0;
		toglow = 0.0;
		toghigh = 0.0;
		break;
	case 2:
		attack = 50.0;
		release = 400.0;
		knee = 8.0;
		ratio = 1.5;
		thresdb = -16.0;
		max = 10.0;
		slewfactor = 1.0;
		sidechain = 0.0;
		detectfreq = 1000.0;
		targetfreq = 1000.0;
		targetwidth = 1.0;
		controlgain = 0.0;
		boostcut = 0.0;
		togpeak = 1.0;
		toglow = 0.0;
		toghigh = 0.0;
		break;
	}
	activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZamDynamicEQPlugin::getParameterValue(uint32_t index) const
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
    case paramThresh:
        return thresdb;
        break;
    case paramMaxBoostCut:
        return max;
        break;
    case paramSlew:
        return slewfactor;
        break;
    case paramSidechain:
        return sidechain;
        break;
    case paramToggleLow:
        return toglow;
        break;
    case paramTogglePeak:
        return togpeak;
        break;
    case paramToggleHigh:
        return toghigh;
        break;
    case paramDetectFreq:
        return detectfreq;
        break;
    case paramTargetFreq:
        return targetfreq;
        break;
    case paramTargetWidth:
        return targetwidth;
        break;
    case paramBoostCut:
        return boostcut;
        break;
    case paramControlGain:
        return controlgain;
        break;
    default:
        return 0.0f;
    }
}

void ZamDynamicEQPlugin::setParameterValue(uint32_t index, float value)
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
    case paramThresh:
        thresdb = value;
        break;
    case paramMaxBoostCut:
        max = value;
        break;
    case paramSlew:
        slewfactor = value;
        break;
    case paramSidechain:
        sidechain = value;
        break;
    case paramToggleLow:
        toglow = value;
        reset_low();
        reset_high();
        reset_peak();
        break;
    case paramTogglePeak:
        togpeak = value;
        reset_low();
        reset_high();
        reset_peak();
        break;
    case paramToggleHigh:
        toghigh = value;
        reset_low();
        reset_high();
        reset_peak();
        break;
    case paramDetectFreq:
        detectfreq = value;
        break;
    case paramTargetFreq:
        targetfreq = value;
        break;
    case paramTargetWidth:
        targetwidth = value;
        break;
    case paramBoostCut:
        boostcut = value;
        reset_low();
        reset_high();
        reset_peak();
        break;
    case paramControlGain:
        controlgain = value;
        break;
    }
}

/*
 * bandpass SVF
 * http://www.cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf
 */

void ZamDynamicEQPlugin::linear_svf_set_params(struct linear_svf *self, float sample_rate, float cutoff, float resonance)
{
	self->g = tanf(M_PI * (cutoff / sample_rate));
	self->k = 1.f / resonance;

	self->a[0] = 1.f / (1.f + self->g * (self->g + self->k));
	self->a[1] = self->g * self->a[0];
	self->a[2] = self->g * self->a[1];

	self->m[0] = 0.f;
	self->m[1] = 1.f;
	self->m[2] = 0.f;
}

void ZamDynamicEQPlugin::linear_svf_reset(struct linear_svf *self)
{
	self->s[0][0]     = 
		self->s[0][1] = 
		self->s[1][0] = 
		self->s[1][1] = 0.f;
}

float ZamDynamicEQPlugin::run_linear_svf(struct linear_svf *self, int c, float in)
{
	float v[3];

	v[2] = in - self->s[c][1];
	v[0] = (self->a[0] * self->s[c][0]) + (self->a[1] * v[2]);
	v[1] = self->s[c][1] + (self->a[1] * self->s[c][0]) + (self->a[2] * v[2]);

	self->s[c][0] = (2.f * v[0]) - self->s[c][0];
	self->s[c][1] = (2.f * v[1]) - self->s[c][1];

	return
		(self->m[0] * in)
		+ (self->m[1] * v[0])
		+ (self->m[2] * v[1]);
}

void ZamDynamicEQPlugin::peq(double G0, double G, double GB, double w0, double Dw,
        double *a0, double *a1, double *a2, double *b0, double *b1, double *b2, double *gn) {

        double F,G00,F00,num,den,G1,G01,G11,F01,F11,W2,Dww,C,D,B,A;
        F = fabs(G*G - GB*GB);
        G00 = fabs(G*G - G0*G0);
        F00 = fabs(GB*GB - G0*G0);
        num = G0*G0 * (w0*w0 - M_PI*M_PI)*(w0*w0 - M_PI*M_PI)
                + G*G * F00 * M_PI*M_PI * Dw*Dw / F;
        den = (w0*w0 - M_PI*M_PI)*(w0*w0 - M_PI*M_PI)
                + F00 * M_PI*M_PI * Dw*Dw / F;
        G1 = sqrt(num/den);
        G01 = fabs(G*G - G0*G1);
        G11 = fabs(G*G - G1*G1);
        F01 = fabs(GB*GB - G0*G1);
        F11 = fabs(GB*GB - G1*G1);
        W2 = sqrt(G11 / G00) * tan(w0/2.f)*tan(w0/2.f);
        Dww = (1.f + sqrt(F00 / F11) * W2) * tan(Dw/2.f);
        C = F11 * Dww*Dww - 2.f * W2 * (F01 - sqrt(F00 * F11));
        D = 2.f * W2 * (G01 - sqrt(G00 * G11));
        A = sqrt((C + D) / F);
        B = sqrt((G*G * C + GB*GB * D) / F);
        *gn = G1;
        *b0 = (G1 + G0*W2 + B) / (1.f + W2 + A);
        *b1 = -2.f*(G1 - G0*W2) / (1.f + W2 + A);
        *b2 = (G1 - B + G0*W2) / (1.f + W2 + A);
        *a0 = 1.f;
        *a1 = -2.f*(1.f - W2) / (1.f + W2 + A);
        *a2 = (1 + W2 - A) / (1.f + W2 + A);

        *b1 = sanitize_denormal(*b1);
        *b2 = sanitize_denormal(*b2);
        *a0 = sanitize_denormal(*a0);
        *a1 = sanitize_denormal(*a1);
        *a2 = sanitize_denormal(*a2);
        *gn = sanitize_denormal(*gn);
        if (!std::isnormal(*b0)) { *b0 = 1.f; }
 }

void ZamDynamicEQPlugin::lowshelfeq(double, double G, double, double w0, double, double q, double B[], double A[]) {
        double alpha,b0,b1,b2,a0,a1,a2;
        G = powf(10.f,G/20.f);
        double AA  = sqrt(G);

        alpha = sin(w0)/2.f * sqrt( (AA + 1.f/AA)*(1.f/q - 1.f) + 2.f );
        b0 =    AA*( (AA+1.f) - (AA-1.f)*cos(w0) + 2.f*sqrt(AA)*alpha );
        b1 =  2.f*AA*( (AA-1.f) - (AA+1.f)*cos(w0)                   );
        b2 =    AA*( (AA+1.f) - (AA-1.f)*cos(w0) - 2.f*sqrt(AA)*alpha );
        a0 =        (AA+1.f) + (AA-1.f)*cos(w0) + 2.f*sqrt(AA)*alpha;
        a1 =   -2.f*( (AA-1.f) + (AA+1.f)*cos(w0)                   );
        a2 =        (AA+1.f) + (AA-1.f)*cos(w0) - 2.f*sqrt(AA)*alpha;

        B[0] = b0/a0;
        B[1] = b1/a0;
        B[2] = b2/a0;
        A[0] = 1.f;
        A[1] = a1/a0;
        A[2] = a2/a0;
}

void ZamDynamicEQPlugin::highshelfeq(double, double G, double, double w0, double, double q, double B[], double A[])
{
        double alpha,b0,b1,b2,a0,a1,a2;
        G = powf(10.f,G/20.f);
        double AA  = sqrt(G);

        alpha = sin(w0)/2.f * sqrt( (AA + 1.f/AA)*(1.f/q - 1.f) + 2.f );
        b0 =    AA*( (AA+1.f) + (AA-1.f)*cos(w0) + 2.f*sqrt(AA)*alpha );
        b1 =  -2.f*AA*( (AA-1.f) + (AA+1.f)*cos(w0)                   );
        b2 =    AA*( (AA+1.f) + (AA-1.f)*cos(w0) - 2.f*sqrt(AA)*alpha );
        a0 =        (AA+1.f) - (AA-1.f)*cos(w0) + 2.f*sqrt(AA)*alpha;
        a1 =   2.f*( (AA-1.f) - (AA+1.f)*cos(w0)                   );
        a2 =        (AA+1.f) - (AA-1.f)*cos(w0) - 2.f*sqrt(AA)*alpha;

        B[0] = b0/a0;
        B[1] = b1/a0;
        B[2] = b2/a0;
        A[0] = 1.f;
        A[1] = a1/a0;
        A[2] = a2/a0;
}

void ZamDynamicEQPlugin::run_lowshelf(double input, double* output)
{
        double in = input;
        zln1 = sanitize_denormal(zln1);
        zln2 = sanitize_denormal(zln2);
        zld1 = sanitize_denormal(zld1);
        zld2 = sanitize_denormal(zld2);
        in = sanitize_denormal(in);

        *output = in * Bl[0] +
                zln1 * Bl[1] +
                zln2 * Bl[2] -
                zld1 * Al[1] -
                zld2 * Al[2] + 1e-20;

        *output = sanitize_denormal(*output);
        zln2 = zln1;
        zld2 = zld1;
        zln1 = in;
        zld1 = *output;
}

void ZamDynamicEQPlugin::run_highshelf(double input, double* output)
{
        double in = input;
        zhn1 = sanitize_denormal(zhn1);
        zhn2 = sanitize_denormal(zhn2);
        zhd1 = sanitize_denormal(zhd1);
        zhd2 = sanitize_denormal(zhd2);
        in = sanitize_denormal(in);

        *output = in   * Bh[0] +
                  zhn1 * Bh[1] +
                  zhn2 * Bh[2] -
                  zhd1 * Ah[1] -
                  zhd2 * Ah[2] + 1e-20;

        *output = sanitize_denormal(*output);
        zhn2 = zhn1;
        zhd2 = zhd1;
        zhn1 = in;
        zhd1 = *output;
}

void ZamDynamicEQPlugin::run_peq2(double input, double* output)
{
        double in = input;
        x1a = sanitize_denormal(x1a);
        x2a = sanitize_denormal(x2a);
        y1a = sanitize_denormal(y1a);
        y2a = sanitize_denormal(y2a);
        in = sanitize_denormal(in);

        *output = in * b0y + x1a * b1y + x2a * b2y - y1a * a1y - y2a * a2y
                + 1e-20;

        *output = sanitize_denormal(*output);
        x2a = x1a;
        y2a = y1a;
        x1a = in;
        y1a = *output;
}

void ZamDynamicEQPlugin::run_comp(double input, double* output)
{
	float srate = getSampleRate();
	float width = (6.f * knee) + 0.01;
	float slewwidth = 1.8f;
        float cdb=0.f;
        float attack_coeff = exp(-1000.f/(attack * srate));
        float release_coeff = exp(-1000.f/(release * srate));

        int attslew = 0;
	float lgaininp = 0.f;
	float Lgain = 1.f;
        float Lxg, Lxl, Lyg, Lyl, Ly1;
        float checkwidth = 0.f;
	float in;

        in = input;
        attslew = 0;
	Lyg = 0.f;
	Lxg = (in==0.f) ? -160.f : to_dB(fabs(in));
        Lxg = sanitize_denormal(Lxg);

        Lyg = Lxg + (1.f/ratio-1.f)*(Lxg-thresdb+width/2.f)*(Lxg-thresdb+width/2.f)/(2.f*width);

	checkwidth = 2.f*fabs(Lxg-thresdb);
        if (2.f*(Lxg-thresdb) < -width) {
                Lyg = Lxg;
        } else if (checkwidth <= width) {
		Lyg = thresdb + (Lxg-thresdb)/ratio;
		Lyg = sanitize_denormal(Lyg);
		if (checkwidth <= slewwidth) {
			if (Lyg >= oldL_yg) {
				attslew = 1;
			}
		}
        } else if (2.f*(Lxg-thresdb) > width) {
                Lyg = thresdb + (Lxg-thresdb)/ratio;
                Lyg = sanitize_denormal(Lyg);
        }

        attack_coeff = attslew ? exp(-1000.f/((attack + 2.0*(slewfactor - 1)) * srate)) : attack_coeff;

        Lxl = Lxg - Lyg;

        oldL_y1 = sanitize_denormal(oldL_y1);
        oldL_yl = sanitize_denormal(oldL_yl);
        Ly1 = fmaxf(Lxl, release_coeff * oldL_y1+(1.f-release_coeff)*Lxl);
        Lyl = attack_coeff * oldL_yl+(1.f-attack_coeff)*Ly1;
        Ly1 = sanitize_denormal(Ly1);
        Lyl = sanitize_denormal(Lyl);

        cdb = -Lyl;
        Lgain = from_dB(cdb);

        //gainred = Lyl;

        lgaininp = Lgain * in;
        *output = lgaininp;

        oldL_yl = Lyl;
        oldL_y1 = Ly1;
        oldL_yg = Lyg;
}

void ZamDynamicEQPlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
        float srate = getSampleRate();
        double dcgain = 1.f;
        double qq2, boost2, fc2, w02, bwgain2, bw2;
        double boostl, All, bwl, bwgaindbl;
        double boosth, Ahh, bwh, bwgaindbh;

	int choose = (sidechain < 0.5) ? 0 : 1;
	float gaincontrol = 0.f;

	linear_svf_set_params(&bandpass, srate, detectfreq, 4.);

	uint32_t i;
	for (i = 0; i < frames; i++) {
		double tmp, filtered, out;
		double in = inputs[0][i];
		in = sanitize_denormal(in);
		tmp = sanitize_denormal(inputs[choose][i]);
		filtered = tmp;
		out = in;

		filtered = run_linear_svf(&bandpass, 0,  tmp);
		filtered = run_linear_svf(&bandpass, 1,  filtered);
		run_comp(filtered, &tmp);

		gaincontrol = tmp / filtered;
		controlgain = sanitize_denormal(to_dB(gaincontrol));
		if (boostcut > 0.5) {
			controlgain = -controlgain;
			if (controlgain < 0.f)
				controlgain = 0.f;
			else if (controlgain > max)
				controlgain = max;
		} else {
			if (controlgain > 0.f)
				controlgain = 0.f;
			else if (controlgain < -max)
				controlgain = -max;
		}

		if (controlgain != 0.f) {
			if (toglow) {
				bwl = 2.f*M_PI*targetfreq / srate;
				boostl = from_dB(controlgain);
				All = sqrt(boostl);
				bwgaindbl = to_dB(All);
				lowshelfeq(0.f,controlgain,bwgaindbl,bwl,bwl,0.707f,Bl,Al);
				run_lowshelf(in, &out);
			} else if (togpeak) {
				qq2 = pow(2.0, 1.0/targetwidth)/(pow(2.0, targetwidth) - 1.0); //q from octave bw
				boost2 = from_dB(controlgain);
				fc2 = targetfreq / srate;
				w02 = fc2*2.f*M_PI;
				bwgain2 = sqrt(boost2);
				bw2 = fc2 / qq2;
				peq(dcgain,boost2,bwgain2,w02,bw2,&a0y,&a1y,&a2y,&b0y,&b1y,&b2y,&gainy);
				run_peq2(in, &out);
			} else {
				bwh = 2.f*M_PI*targetfreq / srate;
				boosth = from_dB(controlgain);
				Ahh = sqrt(boosth);
				bwgaindbh = to_dB(Ahh);
				highshelfeq(0.f,controlgain,bwgaindbh,bwh,bwh,0.707f,Bh,Ah);
				run_highshelf(in, &out);
			}
		}
		outputs[0][i] = (float) out;
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamDynamicEQPlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
