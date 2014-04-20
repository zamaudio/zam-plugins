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
    : Plugin(paramCount, 1, 1) // 1 program, 1 state
{
    // set default values
    d_setProgram(0);

    // reset
    d_deactivate();
}

ZaMultiCompX2Plugin::~ZaMultiCompX2Plugin()
{
}

// -----------------------------------------------------------------------
// Init

void ZaMultiCompX2Plugin::d_initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramAttack:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Attack";
        parameter.symbol     = "att";
        parameter.unit       = "ms";
        parameter.ranges.def = 10.0f;
        parameter.ranges.min = 0.1f;
        parameter.ranges.max = 100.0f;
        break;
    case paramRelease:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Release";
        parameter.symbol     = "rel";
        parameter.unit       = "ms";
        parameter.ranges.def = 80.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 500.0f;
        break;
    case paramKnee:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Knee";
        parameter.symbol     = "kn";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 9.0f;
        break;
    case paramRatio:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Ratio";
        parameter.symbol     = "rat";
        parameter.unit       = " ";
        parameter.ranges.def = 4.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramThresh:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Threshold";
        parameter.symbol     = "thr";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -80.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramMakeup1:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Makeup 1";
        parameter.symbol     = "mak1";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramMakeup2:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Makeup 2";
        parameter.symbol     = "mak2";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramMakeup3:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Makeup 3";
        parameter.symbol     = "mak3";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramGainR1:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_OUTPUT;
        parameter.name       = "Gain Reduction 1";
        parameter.symbol     = "gr1";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGainR2:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_OUTPUT;
        parameter.name       = "Gain Reduction 2";
        parameter.symbol     = "gr2";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGainR3:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_OUTPUT;
        parameter.name       = "Gain Reduction 3";
        parameter.symbol     = "gr3";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramXover1:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_LOGARITHMIC;
        parameter.name       = "Crossover freq 1";
        parameter.symbol     = "xover1";
        parameter.unit       = "Hz";
        parameter.ranges.def = 500.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 20000.0f;
        break;
    case paramXover2:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_LOGARITHMIC;
        parameter.name       = "Crossover freq 2";
        parameter.symbol     = "xover2";
        parameter.unit       = "Hz";
        parameter.ranges.def = 3000.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 20000.0f;
        break;
    case paramToggle1:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "ZamComp 1 ON";
        parameter.symbol     = "toggle1";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramToggle2:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "ZamComp 2 ON";
        parameter.symbol     = "toggle2";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramToggle3:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "ZamComp 3 ON";
        parameter.symbol     = "toggle3";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramListen1:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "Listen 1";
        parameter.symbol     = "listen1";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramListen2:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "Listen 2";
        parameter.symbol     = "listen2";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramListen3:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "Listen 3";
        parameter.symbol     = "listen3";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramGlobalGain:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Master Trim";
        parameter.symbol     = "globalgain";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramStereoDet:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "Detection (MAX/avg)";
        parameter.symbol     = "stereodet";
        parameter.unit       = " ";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramOutputLevelL:
        parameter.hints      = PARAMETER_IS_OUTPUT;
        parameter.name       = "Output Left";
        parameter.symbol     = "outl";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramOutputLevelR:
        parameter.hints      = PARAMETER_IS_OUTPUT;
        parameter.name       = "Output Right";
        parameter.symbol     = "outr";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 20.0f;
        break;
    }
}

void ZaMultiCompX2Plugin::d_initProgramName(uint32_t index, d_string& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZaMultiCompX2Plugin::d_getParameterValue(uint32_t index) const
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
    default:
        return 0.0f;
    }
}

void ZaMultiCompX2Plugin::d_setParameterValue(uint32_t index, float value)
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
    }
}

void ZaMultiCompX2Plugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    attack = 10.0f;
    release = 80.0f;
    knee = 0.0f;
    ratio = 4.0f;
    thresdb = 0.0f;
    makeup[0] = 0.0f;
    makeup[1] = 0.0f;
    makeup[2] = 0.0f;
    gainr[0] = 0.0f;
    gainr[1] = 0.0f;
    gainr[2] = 0.0f;
    xover1 = 500.0f;
    xover2 = 3000.0f;
    toggle[0] = 0.0f;
    toggle[1] = 0.0f;
    toggle[2] = 0.0f;
    listen[0] = 0.0f;
    listen[1] = 0.0f;
    listen[2] = 0.0f;
    globalgain = 0.0f;
    stereodet = 1.0f;
    outl = -45.f;
    outr = -45.f;
    maxL = 0.f;
    maxR = 0.f;

    /* Default variable values */

    /* reset filter values */
    d_activate();
}

void ZaMultiCompX2Plugin::d_setState(const char* key, const char* value)
{
    resetl = true;
    resetr = true;
}

void ZaMultiCompX2Plugin::d_initStateKey(unsigned int key, d_string& val)
{
}

// -----------------------------------------------------------------------
// Process

void ZaMultiCompX2Plugin::d_activate()
{
        int i,j;
        for (i = 0; i < MAX_COMP; i++)
        	for (j = 0; j < 2; j++)
                	old_yl[j][i]=old_y1[j][i]=0.f;

        for (i = 0; i < MAX_BANDS; i++) {
        	for (j = 0; j < 2; j++) {
                	F1[j][i] = Q1[j][i] = 0.f;
			w1[j][i] = w2[j][i] = 0.f;
                	w3[j][i] = w4[j][i] = 0.f;
                	w5[j][i] = w6[j][i] = 0.f;
		}
        }
	maxL = maxR = 0.f;
}

void ZaMultiCompX2Plugin::d_deactivate()
{
    // all values to zero
    d_activate();
}
void ZaMultiCompX2Plugin::run_filter(int i, int ch, float in, float *outl, float *outh)
{
	float x, yl1, yl2, yb1, yb2, yh1, yh2, z1, z2, z3, z4;

        z1 = w1[ch][i];
        z2 = w2[ch][i];
        z3 = w3[ch][i];
        z4 = w4[ch][i]; 

	x = sanitize_denormal(in);
	yh1 = x - z1 - Q1[ch][i] * z2;
	yb1 = yh1 * F1[ch][i] + z3;
	yl1 = F1[ch][i] * yb1 + z4;

	//yh1 = x - yl2 - Q1[ch][i] * yb2;
	//yb1 = (x - yl2 - Q1[ch][i] * yb2) * F1[ch][i] + yb2;
	//yl1 = F1[ch][i] * yb1 + yl2;

	*outl = sanitize_denormal(yl1);
	*outh = sanitize_denormal(yh1);
	
	z1 = yl1;
	z2 = yb1;
	z3 = yb1;
	z4 = yl1;

        w1[ch][i] = sanitize_denormal(z1);
        w2[ch][i] = sanitize_denormal(z2);
        w3[ch][i] = sanitize_denormal(z3);
        w4[ch][i] = sanitize_denormal(z4);
}




/*
void ZaMultiCompX2Plugin::run_filter(int i, int ch, float in, float *outl, float *outh)
{
	float x, y, glo, ghi, ww1, ww2, ww3, ww4, ww5, ww6;
        in = sanitize_denormal(in);

        ww1 = w1[ch][i];
        ww2 = w2[ch][i];
        ww3 = w3[ch][i];
        ww4 = w4[ch][i]; 
	ww5 = w5[ch][i];
	ww6 = w6[ch][i];
	
	glo = gl[ch][i];
	ghi = gh[ch][i];

	x = in;
	x -= c1[ch][i] * ww1 + c2[ch][i] * ww2 + 1e-20;
	y = x + 4. * (ww1 + ww2);
	ww2 += ww1;
	ww1 += x;
	x -= c3[ch][i] * ww3 + c4[ch][i] * ww4 - 1e-20;
	*outh = ghi * x;
	ww4 += ww3;
	ww3 += x;
	y -= c3[ch][i] * ww5 + c4[ch][i] * ww6 - 1e-20;
	x = y + 4. * (ww5 + ww6);
	*outl = glo * x;
	ww6 += ww5;
	ww5 += y;

	w1[ch][i] = ww1;
	w2[ch][i] = ww2;
	w3[ch][i] = ww3;
	w4[ch][i] = ww4;
	w5[ch][i] = ww5;
	w6[ch][i] = ww6;
}
*/
void ZaMultiCompX2Plugin::set_coeffs(float fc, int i, int ch)
{
	float sr;
	sr = d_getSampleRate();
	F1[ch][i] = 2. * sinf(M_PI * fc / sr);
	Q1[ch][i] = ROOT2;
}


/*
void ZaMultiCompX2Plugin::set_coeffs(float fc, int i, int ch)
{
    float a, b, d1, d2, r, p, s;
    s = powf(2, 0.465f);
    d1 = 2. * cosf((2.+s) * M_PI / 8.);
    d1 = 2. * cosf((2.-s) * M_PI / 8.);
    a = tanf ( M_PI * fc / d_getSampleRate());
    b = a * a;

    r = a/d1;
    p = r*b;
    s = p + r + b;
    c1[ch][i] = (4 * p + 2 * b) / s;
    c2[ch][i] = (4 * p) / s;
    gl[ch][i] = p/s;
    gh[ch][i] = r/s;

    r = a/d2;
    p = r*b;
    s = p + r + b;
    c3[ch][i] = (4 * p + 2 * b) / s;
    c4[ch][i] = (4 * p) / s;
    gl[ch][i] *= p/s;
    gh[ch][i] *= r/s;
}
*/
void ZaMultiCompX2Plugin::set_hp_coeffs(float fc, float q, float sr, int i, int ch, float gain=1.0)
{
/*
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
	
	float ff = fc / sr;

    const float ita = 1.0 / tan(M_PI*ff);
    a0[ch][i] = 1.0 / (1.0 + q*ita + ita*ita);
    a1[ch][i] = -2.0*a0[ch][i];
    a2[ch][i] = a0[ch][i];
    b1[ch][i] = 2.0 * (ita*ita - 1.0) * a0[ch][i];
    b2[ch][i] = -(1.0 - q*ita + ita*ita) * a0[ch][i];
*/
}

void ZaMultiCompX2Plugin::run_comp(int k, float inL, float inR, float *outL, float *outR)
{
	float srate = d_getSampleRate();
        float width=(knee-0.99f)*6.f;
        float attack_coeff = exp(-1000.f/(attack * srate));
        float release_coeff = exp(-1000.f/(release * srate));
	int stereolink = (stereodet > 0.5f) ? STEREOLINK_MAX : STEREOLINK_AVERAGE;

        float cdb=0.f;
        float Lgain = 1.f;
        float Rgain = 1.f;
        float Lxg, Lyg;
        float Rxg, Ryg;
        float Lxl, Lyl, Ly1;
        float Rxl, Ryl, Ry1;

        Lyg = Ryg = 0.f;
        Lxg = (inL==0.f) ? -160.f : to_dB(fabs(inL));
        Rxg = (inR==0.f) ? -160.f : to_dB(fabs(inR));
        Lxg = sanitize_denormal(Lxg);
        Rxg = sanitize_denormal(Rxg); 
        
        
        if (2.f*(Lxg-thresdb)<-width) {
                Lyg = Lxg;
        } else if (2.f*fabs(Lxg-thresdb)<=width) {
                Lyg = Lxg + (1.f/ratio-1.f)*(Lxg-thresdb+width/2.f)*(Lxg-thresdb+width/2.f)/(2.f*width);
        } else if (2.f*(Lxg-thresdb)>width) {
                Lyg = thresdb + (Lxg-thresdb)/ratio;
        }
        
        Lyg = sanitize_denormal(Lyg); 
        
        if (2.f*(Rxg-thresdb)<-width) {
                Ryg = Rxg;
        } else if (2.f*fabs(Rxg-thresdb)<=width) {
                Ryg = Rxg + (1.f/ratio-1.f)*(Rxg-thresdb+width/2.f)*(Rxg-thresdb+width/2.f)/(2.f*width);
        } else if (2.f*(Rxg-thresdb)>width) {
                Ryg = thresdb + (Rxg-thresdb)/ratio;
        }
        
        Ryg = sanitize_denormal(Ryg); 
        
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
}

void ZaMultiCompX2Plugin::d_run(float** inputs, float** outputs, uint32_t frames)
{
	float srate = d_getSampleRate();
	float maxxL = maxL;
	float maxxR = maxR;

        int tog1 = (toggle[0] > 0.5f) ? 1 : 0;
        int tog2 = (toggle[1] > 0.5f) ? 1 : 0;
        int tog3 = (toggle[2] > 0.5f) ? 1 : 0;

        int listen1 = (listen[0] > 0.5f) ? 1 : 0;
        int listen2 = (listen[1] > 0.5f) ? 1 : 0;
        int listen3 = (listen[2] > 0.5f) ? 1 : 0;

        set_coeffs(xover1, 0, 0);
        set_coeffs(xover2, 1, 0);
        set_coeffs(xover2, 2, 0);
        set_coeffs(xover1, 0, 1);
        set_coeffs(xover2, 1, 1);
        set_coeffs(xover2, 2, 1);

        for (uint32_t i = 0; i < frames; ++i) {
                float tmp1[2];
		float bnd1[2], bnd2[2], bnd3[2];
		float fil1[2], fil2[2], fil3[2], fil4[2];
		float outL[MAX_COMP] = {0.f};
		float outR[MAX_COMP] = {0.f};

		int listenmode = 0;

		// Interleaved channel processing
                run_filter(0, 0, inputs[0][i], &fil1[0], &fil2[0]);
                run_filter(0, 1, inputs[1][i], &fil1[1], &fil2[1]);
		if (tog1)
			run_comp(0, fil1[0], fil1[1], &outL[0], &outR[0]);

		bnd1[0] = tog1 ? outL[0] * from_dB(makeup[0]) : fil1[0];
                bnd1[1] = tog1 ? outR[0] * from_dB(makeup[0]) : fil1[1];

                run_filter(1, 0, fil2[0], &fil3[0], &tmp1[0]);
                run_filter(1, 1, fil2[1], &fil3[1], &tmp1[1]);
                //run_filter(1, 0, inputs[0][i], &fil3[0], &tmp1[0]);
                //run_filter(1, 1, inputs[1][i], &fil3[1], &tmp1[1]);

		if (tog2)
			run_comp(1, fil3[0], fil3[1], &outL[1], &outR[1]);

		bnd2[0] = tog2 ? outL[1] * from_dB(makeup[1]) : fil3[0];
                bnd2[1] = tog2 ? outR[1] * from_dB(makeup[1]) : fil3[1];

		run_filter(2, 0, inputs[0][i], &tmp1[0], &fil4[0]);
		run_filter(2, 1, inputs[1][i], &tmp1[1], &fil4[1]);

		if (tog3) 
			run_comp(2, fil4[0], fil4[1], &outL[2], &outR[2]);

                bnd3[0] = tog3 ? outL[2] * from_dB(makeup[2]) : fil4[0];
                bnd3[1] = tog3 ? outR[2] * from_dB(makeup[2]) : fil4[1];

		outputs[0][i] = outputs[1][i] = 0.f;	
		if (listen1) {
			listenmode = 1;
			outputs[0][i] += outL[0] * tog1*from_dB(makeup[0])
					+ (1.-tog1) * bnd1[0];
			outputs[1][i] += outR[0] * tog1*from_dB(makeup[0])
					+ (1.-tog1) * bnd1[1];
		}
		if (listen2) {
			listenmode = 1;
			outputs[0][i] += outL[1] * tog2*from_dB(makeup[1])
					+ (1.-tog2) * bnd2[0];
			outputs[1][i] += outR[1] * tog2*from_dB(makeup[1])
					+ (1.-tog2) * bnd2[1];
		}
		if (listen3) {
			listenmode = 1;
			outputs[0][i] += outL[2] * tog3*from_dB(makeup[2])
					+ (1.-tog3) * bnd3[0];
			outputs[1][i] += outR[2] * tog3*from_dB(makeup[2])
					+ (1.-tog3) * bnd3[1];
		}
		if (!listenmode) {
                	outputs[0][i] = bnd1[0];//bnd1[0] + bnd2[0] + bnd3[0];
                	outputs[1][i] = bnd1[1];//bnd1[1] + bnd2[1] + bnd3[1];
		}
                outputs[0][i] *= from_dB(globalgain);
                outputs[1][i] *= from_dB(globalgain);
		
                outputs[0][i] = sanitize_denormal(outputs[0][i]);
                outputs[1][i] = sanitize_denormal(outputs[1][i]);

		if (resetl) {
			maxL = fabsf(outputs[0][i]);
			resetl = false;
		} else {
			maxxL = (fabsf(outputs[0][i]) > maxxL) ? fabsf(outputs[0][i]) : maxxL;
		}
		if (resetr) {
			maxR = fabsf(outputs[1][i]);
			resetr = false;
		} else {
			maxxR = (fabsf(outputs[1][i]) > maxxR) ? fabsf(outputs[1][i]) : maxxR;
		}
        }
	outl = sanitize_denormal((maxxL <= 0.f) ? -160.f : to_dB(maxxL));
	outr = sanitize_denormal((maxxR <= 0.f) ? -160.f : to_dB(maxxR));
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZaMultiCompX2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
