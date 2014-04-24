/*
 * ZamSynth polyphonic synthesiser
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
    : Plugin(paramCount, 1, 1) // 1 program, 1 state
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
    case paramGain:
        parameter.hints      = PARAMETER_IS_AUTOMABLE;
        parameter.name       = "Gain";
        parameter.symbol     = "gain";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -30.0f;
        parameter.ranges.max = 30.0f;
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
    case paramGain:
        return gain;
        break;
    default:
        return 0.0f;
    }
}

void ZamSynthPlugin::d_setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramGain:
        gain = value;
        break;
    }
}

void ZamSynthPlugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    gain = 0.0f;

    /* Default variable values */
    for (int i = 0; i < 127; i++) {
        amp[i] = 0.f;
        rampstate[i] = 0.f;
        rampfreq[i] = 0.f;
        voice[i] = 0;
    }

    for (int i = 0; i < AREAHEIGHT; i++) {
        wave_y[i] = sin(i*2.*M_PI/d_getSampleRate()*1000.);
    }

    /* reset filter values */
    d_activate();
}

void ZamSynthPlugin::d_setState(const char* key, const char* value)
{
	if (strcmp(key, "waveform") != 0) return;

	char* tmp;
	char* saveptr;
	int i = 0;
	char tmpbuf[4*AREAHEIGHT+1] = {0};
	snprintf(tmpbuf, 4*AREAHEIGHT, "%s", value);
	tmp = strtok(tmpbuf, " ");
	while ((tmp != NULL) && (i < AREAHEIGHT)) {
		wave_y[i] = ((float) atoi(tmp))/AREAHEIGHT - 0.5;
		i++;
		//printf("dsp wave_y[%d]=%.2f ", i, wave_y[i]);
		tmp = strtok(NULL, " ");
	}
}

void ZamSynthPlugin::d_initStateKey(unsigned int index, d_string& key)
{
	if (index != 0)
		return;
	key = "waveform";
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


float ZamSynthPlugin::wavetable(float in)
{
	//int index = (int) ((in / (2.0 * M_PI) + 1.0) / 2.0 * (AREAHEIGHT-1.0));
	int index = (int) ((in / (2.0 * M_PI)) * (AREAHEIGHT-1.0));
	return (wave_y[index]);
	//return (sin(in));
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

			//printf("Note ON: %d totalv=%d\n", num, totalvoices);
			rampfreq[*ptrvoice] = 440.0*powf(2.0, (num-48.0-24)/12.);
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

			//printf("Note OFF: %d totalv=%d\n", num, totalvoices);
			amp[*ptrvoice] = 0.f;
			rampfreq[*ptrvoice] = 1.f;
		}
	}
	
	float power;
	bool signal;
	float wave;
	float outl;
	float outr;
	for (i = 0; i < frames; i++) {
		signal = false;
		outl = outr = 0.f;
		power = 0.f;
		for (j = 0; j < 128; j++) {
			if (amp[j] < 0.01f) continue;
			signal = true;

			// ramp sawtooth
			RD_0 = 1.4247585730565955E-4*srate/44100.*rampfreq[j] + rampstate[j];
			if (RD_0>6.283185307179586) {RD_0 -= 6.283185307179586;}
			if (RD_0<-6.283185307179586) {RD_0 += 6.283185307179586;}
			rampstate[j] = RD_0;

			// wavetable
			wave = wavetable(rampstate[j]);
			power += sqrt(amp[j]);

			outl += wave*amp[j];
			outr += wave*amp[j];
		}
		if (signal) {
			outl *= (totalvoices)/(10. * power);
			outr *= (totalvoices)/(10. * power);
			outputs[0][i] = outl*from_dB(gain);
			outputs[1][i] = outr*from_dB(gain);
		} else {
			outputs[0][i] = 0.f;
			outputs[1][i] = 0.f;
		}
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamSynthPlugin();
}

// -----------------------------------------------------------------------
END_NAMESPACE_DISTRHO
