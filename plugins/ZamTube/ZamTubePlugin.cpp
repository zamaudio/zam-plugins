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
#include "tonestacks.hpp"
#include "triode.h"
#include <stdio.h>

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamTubePlugin::ZamTubePlugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
	int i, j;

	const float* tstacks[25] = {
		Tonestacks::bassman,
		Tonestacks::mesa,
		Tonestacks::twin,
		Tonestacks::princeton,
		Tonestacks::jcm800,
		Tonestacks::jcm2000,
		Tonestacks::jtm45,
		Tonestacks::mlead,
		Tonestacks::m2199,
		Tonestacks::ac30,
		Tonestacks::ac15,
		Tonestacks::soldano,
		Tonestacks::sovtek,
		Tonestacks::peavey,
		Tonestacks::ibanez,
		Tonestacks::roland,
		Tonestacks::ampeg,
		Tonestacks::ampegrev,
		Tonestacks::bogner,
		Tonestacks::groove,
		Tonestacks::crunch,
		Tonestacks::fenderblues,
		Tonestacks::fender,
		Tonestacks::fenderdeville,
		Tonestacks::gibson
	};

	for (i = 0; i < 25; i++) {
		for (j = 0; j < 7; j++) {
			ts[i][j] = tstacks[i][j];
		}
	}

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
        parameter.ranges.def = 0.1f;
        parameter.ranges.min = 0.1f;
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
        parameter.enumValues.count = 25;
        parameter.enumValues.restrictedMode = true;
        {
            ParameterEnumerationValue* const values = new ParameterEnumerationValue[25];
            parameter.enumValues.values = values;

            values[0].label = "1959 Bassman 5F6-A";
            values[0].value = 0;
            values[1].label = "Mesa Boogie Mark";
            values[1].value = 1;
            values[2].label = "1969 Twin Reverb AA270";
            values[2].value = 2;
            values[3].label = "1964 Princeton AA1164";
            values[3].value = 3;
            values[4].label = "1959/81 JCM-800 Lead 100";
            values[4].value = 4;
            values[5].label = "1981 JCM-2000 Lead";
            values[5].value = 5;
            values[6].label = "JTM 45";
            values[6].value = 6;
            values[7].label = "1967 Major Lead 200";
            values[7].value = 7;
            values[8].label = "M2199 30W";
            values[8].value = 8;
            values[9].label = "1959/86 AC-30";
            values[9].value = 9;
            values[10].label = "VOX AC-15";
            values[10].value = 10;
            values[11].label = "Soldano SLO 100";
            values[11].value = 11;
            values[12].label = "Sovtek MIG 100 H";
            values[12].value = 12;
            values[13].label = "Peavey C20";
            values[13].value = 13;
            values[14].label = "Ibanez GX20";
            values[14].value = 14;
            values[15].label = "Roland Cube 60";
            values[15].value = 15;
            values[16].label = "Ampeg VL 501";
            values[16].value = 16;
            values[17].label = "Ampeg Reverb Rocket";
            values[17].value = 17;
            values[18].label = "Bogner Triple Giant Preamp";
            values[18].value = 18;
            values[19].label = "Groove Trio Preamp";
            values[19].value = 19;
            values[20].label = "Hughes&Kettner";
            values[20].value = 20;
            values[21].label = "Fender Blues Junior";
            values[21].value = 21;
            values[22].label = "Fender";
            values[22].value = 22;
            values[23].label = "Fender Hot Rod";
            values[23].value = 23;
            values[24].label = "Gibsen GS12 Reverb Rocket";
            values[24].value = 24;
        }
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
        parameter.name       = "Insane Boost";
        parameter.symbol     = "insane";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
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
        insane = value > 0.5 ? 1.0 : 0.0;
        break;
    }
}

void ZamTubePlugin::loadProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    tubedrive = 0.1f;
    bass = 5.f;
    middle = 5.f;
    treble = 5.f;
    tonestack = 0.0f;
    mastergain = 0.0f;
    insane = 0.0f;
    insaneold = 0.0f;

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
	/* Original WDF preamp
	rg[0] = 20e+3;
	rk[0] = 1e+3;
	ck[0] = 10e-6;
	e[0] = 500.0;
	er[0] = 100e+3;
	co[0] = 10e-9;
	ro[0] = 1e+6;
	*/
	
	/* Matt's preamp */
	rg[0] = 1e-3;
	rk[0] = 1200.; // 820 originally
	ck[0] = 50e-6;
	e[0] = 300.0;
	er[0] = 120e+3;
	co[0] = 4.7e-9;
	ro[0] = 470e+3;

	/* CLA's preamp
	rg[0] = 5.6e+3;
	rk[0] = 1.5e+3;
	ck[0] = 4.7e-6;
	e[0] = 340.0;
	er[0] = 230e+3;
	co[0] = 4.72e-9;
	ro[0] = 100e+3;
	*/

	int pre = 0;
	float volumepot = 800e+3; // 100 good at low gain, 1000 good at high gain 
	ckt.on = false;
	ckt.updateRValues(ck[pre], co[pre], e[pre], er[pre], rg[pre], volumepot, rk[pre], 1e+3, ro[pre], Fs);
	ckt.warmup_tubes();

        fSamplingFreq = Fs;
	
	fConst0 = (2 * float(MIN(192000, MAX(1, fSamplingFreq))));
	fConst1 = faustpower<2>(fConst0);
	fConst2 = (3 * fConst0);
	fRec0[3] = 0.f;
	fRec0[2] = 0.f;
	fRec0[1] = 0.f;
	fRec0[0] = 0.f;
}

void ZamTubePlugin::deactivate()
{
	ckt.warmup_tubes();
	fRec0[3] = 0.f;
	fRec0[2] = 0.f;
	fRec0[1] = 0.f;
	fRec0[0] = 0.f;
}
		
void ZamTubePlugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	const uint8_t stack = (uint8_t)tonestack > 24 ? 24 : (uint8_t)tonestack;

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

	float toneout = 0.f;
	
	float cut = insane ? 0. : 15.;
	float pregain = from_dB(tubedrive*3.6364 - cut + mastergain);
	float postgain = from_dB(cut + 42. * (1. - log1p(tubedrive/11.)));

	for (uint32_t i = 0; i < frames; ++i) {

		//Step 1: read input sample as voltage for the source
		float in = inputs[0][i] * pregain;

		//Tone Stack (post tube)
		fRec0[0] = (in - (fSlow31 * (((fSlow30 * fRec0[1]) + (fSlow29 * fRec0[2])) + (fSlow27 * fRec0[3])))) + 1e-20f;
		toneout = sanitize_denormal((float)(fSlow31 * ((((fSlow46 * fRec0[0]) + (fSlow45 * fRec0[1])) + (fSlow43 * fRec0[2])) + (fSlow41 * fRec0[3]))));

		outputs[0][i] = ckt.advanc(toneout) * postgain / 10000.;

		// update filter states
		fRec0[3] = fRec0[2];
		fRec0[2] = fRec0[1];
		fRec0[1] = fRec0[0];
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamTubePlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
