/*
 * ZamPiano physically modelled piano synth
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
 */

#include "ZamPianoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamPianoPlugin::ZamPianoPlugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    // set default values
    loadProgram(0);
}

ZamPianoPlugin::~ZamPianoPlugin()
{
	int i;
	for (i = 0; i < 88; i++) {
		free(intermed[i][0]);
		free(intermed[i][1]);
	}
}

// -----------------------------------------------------------------------
// Init

void ZamPianoPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramFreq: // fentry0
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Tone Frequency";
        parameter.symbol     = "freq";
        parameter.unit       = "Hz";
        parameter.ranges.def = 440.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 20000.0f;
	break;
    case paramGain: // fentry1
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Gain";
        parameter.symbol     = "gain";
        parameter.unit       = "";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramGate: // fbutton0
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Gate";
        parameter.symbol     = "gate";
        parameter.unit       = "";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
	break;
    case paramBright: // fslider0
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Brightness";
        parameter.symbol     = "brightness";
        parameter.unit       = "";
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramDetuning: // fslider3
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Detuning";
        parameter.symbol     = "detuning";
        parameter.unit       = "";
        parameter.ranges.def = 0.1f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramHammerHard: // fslider1
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Hammer Hardness";
        parameter.symbol     = "hammerhard";
        parameter.unit       = "";
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramStiffness: // fslider2
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Stiffness";
        parameter.symbol     = "stiffness";
        parameter.unit       = "";
        parameter.ranges.def = 0.28f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramReverbGain: // fslider4
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Reverb Gain";
        parameter.symbol     = "reverbgain";
        parameter.unit       = "";
        parameter.ranges.def = 0.137f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramReverbRoom: // fslider6
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Reverb Room Size";
        parameter.symbol     = "reverbroom";
        parameter.unit       = "";
        parameter.ranges.def = 0.72f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 2.0f;
        break;
    case paramPanAngle: // fslider5
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Pan Angle";
        parameter.symbol     = "panangle";
        parameter.unit       = "";
        parameter.ranges.def = 0.6f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramSpatialWidth: // fslider7
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Spatial Width";
        parameter.symbol     = "spatialwidth";
        parameter.unit       = "";
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    }
}

void ZamPianoPlugin::initProgramName(uint32_t index, String& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamPianoPlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramFreq: // fentry0
        return pfreq;
        break;
    case paramGain: // fentry1
        return pgain;
        break;
    case paramGate: // fbutton0
        return pgate;
        break;
    case paramBright: // fslider0
        return pbright;
        break;
    case paramDetuning: // fslider3
        return pdetune;
        break;
    case paramHammerHard: // fslider1
        return phammer;
        break;
    case paramStiffness: // fslider2
        return pstiff;
        break;
    case paramReverbGain: // fslider4
        return prevgain;
        break;
    case paramReverbRoom: // fslider6
        return prevroom;
        break;
    case paramPanAngle: // fslider5
        return ppanangle;
        break;
    case paramSpatialWidth: // fslider7
        return pspatialwidth;
        break;
    default:
    	return 0;
	break;
    }
}

void ZamPianoPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramFreq: // fentry0
        pfreq = value;
        break;
    case paramGain: // fentry1
        pgain = value;
        break;
    case paramGate: // fbutton0
        pgate = value;
        break;
    case paramBright: // fslider0
        pbright = value;
        break;
    case paramDetuning: // fslider3
        pdetune = value;
        break;
    case paramHammerHard: // fslider1
        phammer = value;
        break;
    case paramStiffness: // fslider2
        pstiff = value;
        break;
    case paramReverbGain: // fslider4
        prevgain = value;
        break;
    case paramReverbRoom: // fslider6
        prevroom = value;
        break;
    case paramPanAngle: // fslider5
        ppanangle = value;
        break;
    case paramSpatialWidth: // fslider7
        pspatialwidth = value;
        break;
    }
}

void ZamPianoPlugin::loadProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    /* reset filter values */
    activate();
}

/*
void ZamPianoPlugin::setState(const char*, const char*)
{
}

void ZamPianoPlugin::initStateKey(unsigned int, String&)
{
}
*/

// -----------------------------------------------------------------------
// Process

void ZamPianoPlugin::activate()
{
	int i;
	for (i = 0; i < 88; i++) {
		note[i].state = SILENT;
		note[i].vel = 0.;
		note[i].gatestate = 1;
		N[i].init();
	}

	pbright = 0.5f;
	pdetune = 0.1f;
	phammer = 0.5f;
	pstiff = 0.28f;
	prevgain = 0.137f;
	prevroom = 0.72f;
	ppanangle = 0.6f;
	pspatialwidth = 0.5f;

	for (i = 0; i < 88; i++) {
		intermed[i][0] = (float*)malloc(4096*sizeof(float));
		intermed[i][1] = (float*)malloc(4096*sizeof(float));
	}
}

void ZamPianoPlugin::run(const float** inputs, float** outputs, uint32_t frames,
				const MidiEvent* midievent, uint32_t midicount)
{
	uint32_t i, j;
	bool signal;
	int gate = 1;
	
	for (i = 0; i < 88; i++) {
		N[i].pbright = pbright;
		N[i].phammer = phammer;
		N[i].pstiff = pstiff;
		N[i].pdetune = pdetune;
		N[i].prevgain = prevgain;
		N[i].prevroom = prevroom;
		N[i].ppanangle = ppanangle;
		N[i].pspatialwidth = pspatialwidth;
		for (j = 0; j < frames; j++) {
			intermed[i][0][j] = 0.f;
			intermed[i][1][j] = 0.f;
		}
	}

	for (i = 0; i < midicount; i++) {
		int type = midievent[i].data[0] & 0xF0;
		int chan = midievent[i].data[0] & 0x0F;
		int n = midievent[i].data[1];
		int v = midievent[i].data[2];
		if (type == 0x90 && chan == 0x0) {
			// NOTE ON
			note[n].state = STRIKE;
			note[n].vel = v / 127.f;
			gate = 0;
		}
		else if (type == 0x80 && chan == 0x0) {
			// NOTE OFF
			gate = 1;
			if (note[n].state != SILENT) {
				note[n].state = RELEASE;
			}
		}
	}

	for (i = 0; i < frames; i++) {
		outputs[0][i] = 0.f;
		outputs[1][i] = 0.f;
	}
	signal = false;
	int k;
	for (k = 0; k < 88; k++) {
		if (note[k].state == SILENT) {
			N[k].pgate = 1.;
			continue;
		}
		signal = true;
		if (note[k].state == STRIKE) {
			//printf("STRIKE: %d\n", k);
			N[k].pfreq = 220. * powf(2., (k-69)/12.);
			N[k].pgain = note[k].vel;
			N[k].pgate = 0.;
			N[k].compute(1, inputs, intermed[k]);
			N[k].pgate = 1.;
			note[k].state++;
		} else if (note[k].state < RELEASE) {
			//printf("SUSTAIN: %d %d\n", k, note[k].state);
			note[k].state++;
		} else if (note[k].state < DECAY) {
			//printf("RELEASE: %d %d\n", k, note[k].state);
			//N[k].pfreq = 440. * powf(2., (k-69)/12.);
			N[k].pgain = 0.;
			//N[k].pgate = 0.;
			//N[k].compute(1, inputs, intermed[k]);
			N[k].pgate = 1.;
			note[k].state++;
		} else if (note[k].state < SILENT) {
			//printf("DECAY: %d %d\n", k, note[k].state);
			note[k].state++;
		}
		N[k].compute(frames, inputs, intermed[k]);	
	}
	for (i = 0; i < 88; i++) {
		for (j = 0; j < frames; j++) {
			outputs[0][j] += intermed[i][0][j] * 0.1;
			outputs[1][j] += intermed[i][1][j] * 0.1;
		}
	}
	for (i = 0; i < frames; i++) {
		if (signal) {
			sanitize_denormal(outputs[0][i]);
			sanitize_denormal(outputs[1][i]);

		} else {
			outputs[0][i] = 0.f;
			outputs[1][i] = 0.f;
		}
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamPianoPlugin();
}

// -----------------------------------------------------------------------
END_NAMESPACE_DISTRHO
