/*
 * ZamSynth polyphonic synth
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

#include "ZamSynthPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamSynthPlugin::ZamSynthPlugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    // set default values
    d_setProgram(0);

    // reset
    d_deactivate();
}

ZamSynthPlugin::~ZamSynthPlugin()
{
}

// -----------------------------------------------------------------------
// Init

void ZamSynthPlugin::d_initParameter(uint32_t index, Parameter& parameter)
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
        parameter.ranges.max = 200.0f;
        break;
    }
}

void ZamSynthPlugin::d_initProgramName(uint32_t index, d_string& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamSynthPlugin::d_getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramAttack:
        return attack;
        break;
    default:
        return 0.0f;
    }
}

void ZamSynthPlugin::d_setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramAttack:
        attack = value;
        break;
    }
}

void ZamSynthPlugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    attack = 10.0f;

    /* Default variable values */
    for (int i = 0; i < 127; i++) {
        amp[i] = 0.f;
        rampstate[i] = 0.f;
        rampfreq[i] = 0.f;
        voice[i] = 0;
    }

    /* reset filter values */
    d_activate();
}

// -----------------------------------------------------------------------
// Process

void ZamSynthPlugin::d_activate()
{
}

void ZamSynthPlugin::d_deactivate()
{
    // all values to zero
}

void ZamSynthPlugin::d_run(float** inputs, float** outputs, uint32_t frames,
				const MidiEvent* midievent, uint32_t midicount)
{
	float srate = d_getSampleRate();
	uint32_t i,j;
	float RD_0;

	for (i = 0; i < midicount; i++) {
		int *ptrvoice = voice;
		int type = midievent[i].buf[0] & 0xF0;
		int chan = midievent[i].buf[0] & 0x0F;
		int num = midievent[i].buf[1];
		int vel = midievent[i].buf[2];
		if (type == 0x90 && chan == 0x00) {
			// NOTE ON
			if (noteon[num] == true)
				continue;
			noteon[num] = true;
			noteoff[num] = false;
			ptrvoice++;
			if (totalvoices++ > MAX_VOICES) {
				ptrvoice = voice;
				totalvoices--;
			}
			*ptrvoice = num;

			printf("Note ON: %d totalv=%d\n", num, totalvoices);
			rampfreq[*ptrvoice] = 440.0*powf(2.0, (num-49.0)/12);
			amp[*ptrvoice] = vel / 127.f;
		}
		else if (type == 0x80 && chan == 0x00) {
			// NOTE OFF
			if (noteoff[num] == true)
				continue;
			noteoff[num] = true;
			noteon[num] = false;

			ptrvoice--;
			if (totalvoices-- < 0) {
				ptrvoice = voice + MAX_VOICES;
				totalvoices++;
			}
			*ptrvoice = num;

			printf("Note OFF: %d totalv=%d\n", num, totalvoices);
			amp[*ptrvoice] = 0.f;
			rampfreq[*ptrvoice] = 1.f;
		}
	}
	
	for (i = 0; i < frames; i++) {
		float totalamp = 1.f;
		outputs[0][i] = 0.f;
		float wave;
		for (j = 0; j < 128; j++) {
			if (amp[j] < 0.01f) continue;

			// ramp sawtooth
			RD_0 = 1.4247585730565955E-4*rampfreq[j] + rampstate[j];
			if (RD_0>6.283185307179586) {RD_0 -= 6.283185307179586;}
			if (RD_0<-6.283185307179586) {RD_0 += 6.283185307179586;}
			rampstate[j] = RD_0;

			// wavetable
			wave = sin(rampstate[j]);

			outputs[0][i] += wave*amp[j];
			totalamp += amp[j];
		}
		outputs[0][i] /= totalamp;
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamSynthPlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
