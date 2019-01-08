/*
 * ZamTube triode WDF distortion model
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
 *
 * Tone Stacks from David Yeh's faust implementation.
 *
 */

#include "ZamTubePlugin.hpp"
#include "triode.h"
#include <stdio.h>

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamTubePlugin::ZamTubePlugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamTubePlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramTubedrive:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Tube Drive";
        parameter.symbol     = "tubedrive";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 11.0f;
        break;
    case paramBass:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Bass";
        parameter.symbol     = "bass";
        parameter.unit       = " ";
        parameter.ranges.def = 5.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 10.0f;
        break;
    case paramMiddle:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Mids";
        parameter.symbol     = "mids";
        parameter.unit       = " ";
        parameter.ranges.def = 5.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 10.0f;
        break;
    case paramTreble:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Treble";
        parameter.symbol     = "treb";
        parameter.unit       = " ";
        parameter.ranges.def = 5.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 10.0f;
        break;
    case paramToneStack:
        parameter.hints      = kParameterIsAutomable | kParameterIsInteger;
        parameter.name       = "Tone Stack Model";
        parameter.symbol     = "tonestack";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 24.0f;
        break;
    case paramGain:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Output level";
        parameter.symbol     = "gain";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -30.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramInsane:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Quality Insane";
        parameter.symbol     = "insane";
        parameter.unit       = " ";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    }
}

void ZamTubePlugin::initProgramName(uint32_t index, String& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamTubePlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramTubedrive:
        return tubedrive;
        break;
    case paramBass:
        return bass;
        break;
    case paramMiddle:
        return middle;
        break;
    case paramTreble:
        return treble;
        break;
    case paramToneStack:
        return tonestack;
        break;
    case paramGain:
        return mastergain;
        break;
    case paramInsane:
        return insane;
        break;
    default:
        return 0.0f;
    }
}

void ZamTubePlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramTubedrive:
        tubedrive = value;
        break;
    case paramBass:
        bass = value;
        break;
    case paramMiddle:
        middle = value;
        break;
    case paramTreble:
        treble = value;
        break;
    case paramToneStack:
        tonestack = value;
        break;
    case paramGain:
        mastergain = value;
        break;
    case paramInsane:
        insane = value;
        break;
    }
}

void ZamTubePlugin::loadProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    tubedrive = 0.0f;
    bass = 5.f;
    middle = 5.f;
    treble = 5.f;
    tonestack = 0.0f;
    mastergain = 0.0f;
    insane = 1.0f;

    /* Default variable values */

    /* reset filter values */
    activate();
}

// -----------------------------------------------------------------------
// Process

void ZamTubePlugin::activate()
{

	T Fs = getSampleRate();
	
	// Passive components
	ci = 1.0e-7;
	ri = 1.0e+6;
	rg = 2.0e+4;
	rk = 1.0e+3;
	ck = 1.0e-5;
	e = 500.0;
	rp = 1.0e+5;
	er = rp;
	co = 1.0e-8;
	ro = 1.0e+6;

	// 12AX7 triode model RSD-1
	v.g = 2.242e-3;
	v.mu = 103.2;
	v.gamma = 1.26;
	v.c = 3.4;
	v.gg = 6.177e-4;
	v.e = 1.314;
	v.cg = 9.901;
	v.ig0 = 8.025e-8;

	// 12AX7 triode model EHX-1
	v.g2 = 1.371e-3;
	v.mu2 = 86.9;
	v.gamma2 = 1.349;
	v.c2 = 4.56;
	v.gg2 = 3.263e-4;
	v.e2 = 1.156;
	v.cg2 = 11.99;
	v.ig02 = 3.917e-8;

	ckt.updateRValues(ci, ck, co, e, er, rg, ri, rk, 1., ro, Fs, v);
	ckt.warmup_tubes();

        fSamplingFreq = Fs;
	
	fConst0 = (2 * float(MIN(192000, MAX(1, fSamplingFreq))));
	fConst1 = faustpower<2>(fConst0);
	fConst2 = (3 * fConst0);
	for (int i=0; i<4; i++) fRec0[i] = 0;
}		
		
		
void ZamTubePlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint8_t stack = (uint8_t)tonestack > 24 ? 24 : (uint8_t)tonestack;

	float 	fSlow0 = float(ts[stack][R4]);
	float 	fSlow1 = float(ts[stack][R3]);
	float 	fSlow2 = float(ts[stack][C3]);
	float 	fSlow3 = (fSlow2 * fSlow1);
	float 	fSlow4 = float(ts[stack][C2]);
	float 	fSlow5 = (fSlow4 + fSlow2);
	float 	fSlow6 = float(ts[stack][R1]);
	float 	fSlow7 = (fSlow4 * fSlow1);
	float 	fSlow8 = float(ts[stack][C1]);
	float 	fSlow9 = (fSlow6 + fSlow0);
	float 	fSlow10 = ((fSlow2 * fSlow0) + (fSlow4 * fSlow9));
	float 	fSlow11 = expf((3.4f * (float(bass/10.) - 1)));
	float 	fSlow12 = float(ts[stack][R2]);
	float 	fSlow13 = (fSlow12 * fSlow11);
	float 	fSlow14 = (fSlow8 + fSlow4);
	float 	fSlow15 = float(middle/10.);
	float 	fSlow16 = (fSlow15 * fSlow1);
	float 	fSlow17 = (fSlow16 * fSlow14);
	float 	fSlow18 = (fSlow4 * fSlow0);
	float 	fSlow19 = (fSlow13 * fSlow14);
	float 	fSlow20 = ((fSlow3 * (fSlow18 + (fSlow15 * ((fSlow19 + (fSlow7 + ((fSlow8 * fSlow1) + ((fSlow8 * fSlow6) - fSlow18)))) - fSlow17)))) + ((fSlow13 * (((fSlow4 * fSlow2) * fSlow0) + (fSlow8 * fSlow10))) + (fSlow8 * ((((fSlow4 * fSlow6) * fSlow1) + (fSlow0 * (fSlow7 + (fSlow6 * fSlow5)))) + (fSlow3 * fSlow0)))));
	float 	fSlow21 = (fSlow13 - fSlow16);
	float 	fSlow22 = ((fSlow8 * fSlow4) * fSlow2);
	float 	fSlow23 = (fSlow22 * (((fSlow16 * (((fSlow1 * fSlow9) - (fSlow6 * fSlow0)) + (fSlow9 * fSlow21))) + (((fSlow6 * fSlow12) * fSlow0) * fSlow11)) + ((fSlow6 * fSlow1) * fSlow0)));
	float 	fSlow24 = (fConst0 * fSlow23);
	float 	fSlow25 = (fSlow8 * (fSlow6 + fSlow1));
	float 	fSlow26 = (fConst0 * (((fSlow25 + (fSlow4 * (fSlow1 + fSlow0))) + (fSlow2 * (fSlow0 + fSlow16))) + fSlow19));
	float 	fSlow27 = ((fSlow26 + (fConst1 * (fSlow24 - fSlow20))) - 1);
	float 	fSlow28 = (fConst2 * fSlow23);
	float 	fSlow29 = ((fSlow26 + (fConst1 * (fSlow20 - fSlow28))) - 3);
	float 	fSlow30 = ((fConst1 * (fSlow20 + fSlow28)) - (3 + fSlow26));
	float 	fSlow31 = (1.0f / (0 - (1 + (fSlow26 + (fConst1 * (fSlow20 + fSlow24))))));
	float 	fSlow32 = float(treble/10.);
	float 	fSlow33 = (fSlow32 * fSlow6);
	float 	fSlow34 = (fSlow33 * fSlow0);
	float 	fSlow35 = (fSlow15 * fSlow2);
	float 	fSlow36 = (((fSlow35 * fSlow1) * (fSlow19 + ((fSlow7 + fSlow25) - fSlow17))) + (fSlow8 * (((fSlow34 * fSlow5) + (fSlow13 * fSlow10)) + (fSlow1 * fSlow10))));
	float 	fSlow37 = (fSlow22 * ((fSlow1 * ((fSlow34 + ((fSlow15 * fSlow9) * (fSlow1 + fSlow21))) - (((fSlow32 * fSlow15) * fSlow6) * fSlow0))) + (((fSlow33 * fSlow12) * fSlow0) * fSlow11)));
	float 	fSlow38 = (fConst0 * fSlow37);
	float 	fSlow39 = ((fSlow1 * (fSlow35 + fSlow14)) + (((fSlow32 * fSlow8) * fSlow6) + fSlow19));
	float 	fSlow40 = (fConst0 * fSlow39);
	float 	fSlow41 = (fSlow40 + (fConst1 * (fSlow38 - fSlow36)));
	float 	fSlow42 = (fConst2 * fSlow37);
	float 	fSlow43 = (fSlow40 + (fConst1 * (fSlow36 - fSlow42)));
	float 	fSlow44 = (fConst0 * (0 - fSlow39));
	float 	fSlow45 = (fSlow44 + (fConst1 * (fSlow36 + fSlow42)));
	float 	fSlow46 = (fSlow44 - (fConst1 * (fSlow36 + fSlow38)));

	float tubeout = 0.f;
	ckt.t.insane = insane;
	
	for (uint32_t i = 0; i < frames; ++i) {

		//Step 1: read input sample as voltage for the source
		double in = sanitize_denormal(inputs[0][i]);

		// protect against overflowing circuit
		in = fabs(in) < DANGER ? in : 0.f;

		double pregain = from_dB(tubedrive*3.6364); // 0 to +40dB
		double ViE = in * pregain * from_dB(15.); // assume in -15dBFS
		double postgain = from_dB(mastergain*2. - 8.);
		tubeout = ckt.advanc(ViE) * postgain * from_dB(tubedrive) / (2. * e);
		//Tone Stack sim
		fRec0[0] = ((float)tubeout - (fSlow31 * (((fSlow30 * fRec0[1]) + (fSlow29 * fRec0[2])) + (fSlow27 * fRec0[3]))));
		outputs[0][i] = sanitize_denormal((float)(fSlow31 * ((((fSlow46 * fRec0[0]) + (fSlow45 * fRec0[1])) + (fSlow43 * fRec0[2])) + (fSlow41 * fRec0[3]))));

		// post processing
		for (int i=3; i>0; i--) fRec0[i] = fRec0[i-1];
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamTubePlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
