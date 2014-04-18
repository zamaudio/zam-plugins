/*
 * ZamEQ2 2 band parametric equaliser
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

#include "ZamEQ2Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamEQ2Plugin::ZamEQ2Plugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    // set default values
    d_setProgram(0);

    // reset
    d_deactivate();
}

ZamEQ2Plugin::~ZamEQ2Plugin()
{
}

// -----------------------------------------------------------------------
// Init

void ZamEQ2Plugin::d_initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramGain1:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Boost/Cut 1";
        parameter.symbol     = "boost1";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -50.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramQ1:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_LOGARITHMIC;
        parameter.name       = "Bandwidth 1";
        parameter.symbol     = "bw1";
        parameter.unit       = " ";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.1f;
        parameter.ranges.max = 7.0f;
        break;
    case paramFreq1:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_LOGARITHMIC;
        parameter.name       = "Frequency 1";
        parameter.symbol     = "f1";
        parameter.unit       = "Hz";
        parameter.ranges.def = 500.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 20000.0f;
        break;
    case paramGain2:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Boost/Cut 2";
        parameter.symbol     = "boost2";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -50.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramQ2:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_LOGARITHMIC;
        parameter.name       = "Bandwidth 2";
        parameter.symbol     = "bw2";
        parameter.unit       = " ";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.1f;
        parameter.ranges.max = 7.0f;
        break;
    case paramFreq2:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_LOGARITHMIC;
        parameter.name       = "Frequency 2";
        parameter.symbol     = "f2";
        parameter.unit       = "Hz";
        parameter.ranges.def = 3000.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 20000.0f;
        break;
    case paramGainL:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Boost/Cut L";
        parameter.symbol     = "boostl";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -50.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramFreqL:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_LOGARITHMIC;
        parameter.name       = "Frequency L";
        parameter.symbol     = "fl";
        parameter.unit       = "Hz";
        parameter.ranges.def = 250.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 20000.0f;
        break;
    case paramGainH:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Boost/Cut H";
        parameter.symbol     = "boosth";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -50.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramFreqH:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_LOGARITHMIC;
        parameter.name       = "Frequency H";
        parameter.symbol     = "fh";
        parameter.unit       = "Hz";
        parameter.ranges.def = 8000.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 20000.0f;
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
    case paramTogglePeaks:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "Peaks ON";
        parameter.symbol     = "peaks";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    }
}

void ZamEQ2Plugin::d_initProgramName(uint32_t index, d_string& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamEQ2Plugin::d_getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramGain1:
        return gain1;
        break;
    case paramQ1:
        return q1;
        break;
    case paramFreq1:
        return freq1;
        break;
    case paramGain2:
        return gain2;
        break;
    case paramQ2:
        return q2;
        break;
    case paramFreq2:
        return freq2;
        break;
    case paramGainL:
        return gainl;
        break;
    case paramFreqL:
        return freql;
        break;
    case paramGainH:
        return gainh;
        break;
    case paramFreqH:
        return freqh;
        break;
    case paramMaster:
        return master;
        break;
    case paramTogglePeaks:
        return togglepeaks;
        break;
    default:
        return 0.0f;
    }
}

void ZamEQ2Plugin::d_setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramGain1:
        gain1 = value;
        break;
    case paramQ1:
        q1 = value;
        break;
    case paramFreq1:
        freq1 = value;
        break;
    case paramGain2:
        gain2 = value;
        break;
    case paramQ2:
        q2 = value;
        break;
    case paramFreq2:
        freq2 = value;
        break;
    case paramGainL:
        gainl = value;
	break;
    case paramFreqL:
        freql = value;
        break;
    case paramGainH:
        gainh = value;
        break;
    case paramFreqH:
        freqh = value;
        break;
    case paramMaster:
        master = value;
        break;
    case paramTogglePeaks:
        togglepeaks = value;
        break;
    }
}

void ZamEQ2Plugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    gain1 = 0.0f;
    q1 = 1.0f;
    freq1 = 500.0f;
    gain2 = 0.0f;
    q2 = 1.0f;
    freq2 = 3000.0f;
    gainl = 0.0f;
    freql = 250.0f;
    gainh = 0.0f;
    freqh = 8000.0f;
    master = 0.f;
    togglepeaks = 0.f;

    /* Default variable values */

    /* reset filter values */
    d_activate();
}

// -----------------------------------------------------------------------
// Process

void ZamEQ2Plugin::d_activate()
{
        x1=x2=y1=y2=0.f;
        x1a=x2a=y1a=y2a=0.f;
        a0x=a1x=a2x=b0x=b1x=b2x=gainx=0.f;
        a0y=a1y=a2y=b0y=b1y=b2y=gainy=0.f;
        zln1=zln2=zld1=zld2=0.f;
        zhn1=zhn2=zhd1=zhd2=0.f;

        int i;
	for (i = 0; i < 3; ++i) {
                Bl[i] = Al[i] = Bh[i] = Ah[i] = 0.f;
                Bl[i] = Al[i] = Bh[i] = Ah[i] = 0.f;
                Bl[i] = Al[i] = Bh[i] = Ah[i] = 0.f;
        }
}

void ZamEQ2Plugin::d_deactivate()
{
    // all values to zero
}

void ZamEQ2Plugin::peq(double G0, double G, double GB, double w0, double Dw,
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

void ZamEQ2Plugin::lowshelfeq(double G0, double G, double GB, double w0, double Dw, double q, double B[], double A[]) {
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

void ZamEQ2Plugin::highshelfeq(double G0, double G, double GB, double w0, double Dw, double q, double B[], double A[]) {
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

void ZamEQ2Plugin::d_run(float** inputs, float** outputs, uint32_t frames)
{
	float srate = d_getSampleRate();
        double dcgain = 1.f;

        double qq1 = pow(2.0, 1.0/q1)/(pow(2.0, q1) - 1.0); //q from octave bw
        double boost1 = from_dB(gain1);
        double fc1 = freq1 / srate;
        double w01 = fc1*2.f*M_PI;
        double bwgain1 = sqrt(boost1);
        double bw1 = fc1 / qq1;

        double qq2 = pow(2.0, 1.0/q2)/(pow(2.0, q2) - 1.0); //q from octave bw
        double boost2 = from_dB(gain2);
        double fc2 = freq2 / srate;
        double w02 = fc2*2.f*M_PI;
        double bwgain2 = sqrt(boost2);
        double bw2 = fc2 / qq2;

        double boostl = from_dB(gainl);
        double All = sqrt(boostl);
        double bwl = 2.f*M_PI*freql/ srate;
        double bwgaindbl = to_dB(All);

        double boosth = from_dB(gainh);
        double Ahh = sqrt(boosth);
        double bwh = 2.f*M_PI*freqh/ srate;
        double bwgaindbh = to_dB(Ahh);

        peq(dcgain,boost1,bwgain1,w01,bw1,&a0x,&a1x,&a2x,&b0x,&b1x,&b2x,&gainx);
        peq(dcgain,boost2,bwgain2,w02,bw2,&a0y,&a1y,&a2y,&b0y,&b1y,&b2y,&gainy);
        lowshelfeq(0.f,gainl,bwgaindbl,2.f*M_PI*freql/srate,bwl,1.0f,Bl,Al);
        highshelfeq(0.f,gainh,bwgaindbh,2.f*M_PI*freqh/srate,bwh,1.0f,Bh,Ah);

        for (uint32_t i = 0; i < frames; i++) {
                double tmp,tmpl, tmph;
                double in = inputs[0][i];
                x1 = sanitize_denormal(x1);
                x2 = sanitize_denormal(x2);
                y1 = sanitize_denormal(y1);
                y2 = sanitize_denormal(y2);
                x1a = sanitize_denormal(x1a);
                x2a = sanitize_denormal(x2a);
                y1a = sanitize_denormal(y1a);
                y2a = sanitize_denormal(y2a);
                zln1 = sanitize_denormal(zln1);
                zln2 = sanitize_denormal(zln2);
                zld1 = sanitize_denormal(zld1);
                zld2 = sanitize_denormal(zld2);
                zhn1 = sanitize_denormal(zhn1);
                zhn2 = sanitize_denormal(zhn2);
                zhd1 = sanitize_denormal(zhd1);
                zhd2 = sanitize_denormal(zhd2);
                in = sanitize_denormal(in);

                //lowshelf
                tmpl = in * Bl[0] +
                        zln1 * Bl[1] +
                        zln2 * Bl[2] -
                        zld1 * Al[1] -
                        zld2 * Al[2];
                zln2 = zln1;
                zld2 = zld1;
                zln1 = in;
                zld1 = tmpl;

                //highshelf
                tmph = tmpl * Bh[0] +
                        zhn1 * Bh[1] +
                        zhn2 * Bh[2] -
                        zhd1 * Ah[1] -
                        zhd2 * Ah[2];
                zhn2 = zhn1;
                zhd2 = zhd1;
                zhn1 = tmpl; 
                zhd1 = tmph; 
                        
                //parametric1
                tmp = tmph * b0x + x1 * b1x + x2 * b2x - y1 * a1x - y2 * a2x;
                x2 = x1;
                y2 = y1;
                x1 = tmph;
                y1 = tmp;
                 
                //parametric2
                outputs[0][i] = tmp * b0y + x1a * b1y + x2a * b2y - y1a * a1y - y2a * a2y;
                x2a = x1a;
                y2a = y1a;
                x1a = tmp;
                y1a = outputs[0][i];        
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamEQ2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
