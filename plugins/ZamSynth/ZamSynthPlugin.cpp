/*
 * ZamSynth polyphonic synthesiser
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

#include "ZamSynthPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamSynthPlugin::ZamSynthPlugin()
    : Plugin(paramCount, 0, 2) // 0 programs, 2 states
{
    /* Default parameter values */
    gain = 0.0f;
    graph = 0.0f;
    speed = 10.0f;

    /* Default variable values */
    for (int i = 0; i < MAX_VOICES; i++) {
        voice[i].playing = false;
	voice[i].notenum = -1;
	voice[i].envpos = 0;
	voice[i].slowcount = 0;
	voice[i].curamp = 0.f;
	voice[i].vi = 0.f;
        voice[i].rampstate = 0.f;
    }

    curvoice = voice; //ptr to first voice

    for (int i = 0; i < AREAHEIGHT; i++) {
        wave_y[i] = sin(i*2.*M_PI/getSampleRate());//*1000
    }

    for (int i = 0; i < MAX_ENV; i++) {
        env_y[i] = (sin(i*2.*M_PI/getSampleRate()*1000./2.)) > 0.f ? sin(i*2.*M_PI/getSampleRate()*1000./2.) : 0.f;
    }
}

// -----------------------------------------------------------------------
// Init

void ZamSynthPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramGain:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Gain";
        parameter.symbol     = "gain";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -30.0f;
        parameter.ranges.max = 30.0f;
        break;
    case paramSpeed:
        parameter.hints      = kParameterIsAutomatable | kParameterIsInteger;
        parameter.name       = "Speed";
        parameter.symbol     = "speed";
        parameter.unit       = " ";
        parameter.ranges.def = 10.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 20.0f;
        break;
    case paramGraph:
        parameter.hints      = kParameterIsAutomatable | kParameterIsBoolean;
        parameter.name       = "Graph toggle";
        parameter.symbol     = "graph";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
	break;
    }
}

// -----------------------------------------------------------------------
// Internal data

float ZamSynthPlugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramGain:
        return gain;
        break;
    case paramSpeed:
        return speed;
        break;
    case paramGraph:
        return graph;
        break;
    default:
        return 0.0f;
    }
}

void ZamSynthPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramGain:
        gain = value;
        break;
    case paramSpeed:
        speed = value;
        break;
    case paramGraph:
        graph = value;
        break;
    }
}

void ZamSynthPlugin::setState(const char* key, const char* value)
{
	if (strcmp(key, "waveform") == 0) {
		char* tmp;
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
	} else if (strcmp(key, "envelope") == 0) {
		char* tmp;
		int i = 0;
		char tmpbuf[4*MAX_ENV+1] = {0};
		snprintf(tmpbuf, 4*MAX_ENV, "%s", value);
		tmp = strtok(tmpbuf, " ");
		while ((tmp != NULL) && (i < MAX_ENV)) {
			env_y[i] = ((float) atoi(tmp))/MAX_ENV - 0.5;
			i++;
			//printf("dsp env_y[%d]=%.2f ", i, env_y[i]);
			tmp = strtok(NULL, " ");
		}
	}
}

void ZamSynthPlugin::initState(unsigned int index, String& key, String& defvalue)
{
	if (index == 0) key = "waveform";
	if (index == 1) key = "envelope";
	defvalue = "";
}

// -----------------------------------------------------------------------
// Process

void ZamSynthPlugin::activate()
{
}

float ZamSynthPlugin::wavetable(float in)
{
	int index = (int) ((in / (2.0 * M_PI)) * (AREAHEIGHT-1.0));
	return (wave_y[index]);
	//return (sin(in));
}

void ZamSynthPlugin::run(const float**, float** outputs, uint32_t frames,
				const MidiEvent* midievent, uint32_t midicount)
{
	float srate = getSampleRate();
	int slowfactor = (int) srate / (speed * 2400); // 1-20 ~ 20-1
	uint32_t i;
	float RD_0;

	for (i = 0; i < midicount; i++) {
		int type = midievent[i].data[0] & 0xF0;
		int chan = midievent[i].data[0] & 0x0F;
		int num = midievent[i].data[1];
		int vel = midievent[i].data[2];
		if (type == 0x90 && chan == 0x0) {
			// NOTE ON
			int newvoice = -1;
			//printf("ON: Note\n");
			//printf("ON: begin attack\n");
			for (int k = 0; k < 128; k++)
				if (!voice[k].playing)
					newvoice = k;
			if (newvoice == -1)
				newvoice = 0; //steal #0
			curvoice = &voice[newvoice];
			curvoice->envpos = 1; // begin attack
			curvoice->playing = true;
			curvoice->notenum = num;
			curvoice->vi = vel / 127.f;
			curvoice->curamp = curvoice->vi;
			curvoice->rampstate = 0;
		}
		else if (type == 0x80 && chan == 0x0) {
			// NOTE OFF
			//printf("OFF: Note\n");
			//find voice with current notenum
			nvoices = 0;
			for (int k = 0; k < 128; k++) {
				if (voice[k].playing && voice[k].notenum == num) {
					voice[k].envpos = MAX_ENV / 2 + 1; // begin release;
				}
				if (!voice[k].playing && voice[k].notenum == num) {
					voice[k].notenum = -1;
				}
			}
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
		int k;
		Voice* j;
		// process envelope positions per sample
		for (k = 0; k < 128; k++) {
			j = &voice[k];
			if (j->playing) {
				if (j->envpos <= 0) {
					//silence
					j->playing = false;
					j->notenum = -1;
					j->envpos = 0;
					j->slowcount = 0;
					j->curamp = 0.f;
					j->vi = 0.f;
					j->rampstate = 0.f;
				} else if (j->envpos > 0 && (int) j->envpos < MAX_ENV / 2) {
					//attack
					j->curamp = j->vi * env_y[j->envpos];
					//printf("att: %d %d curamp=%.2f\n",k,j->envpos, j->curamp);
					j->slowcount++;
					j->envpos += ((j->slowcount % slowfactor) == 0) ? 1 : 0;
				} else if (j->envpos > MAX_ENV / 2) {
					//release
					j->curamp = j->vi * env_y[j->envpos];
					//printf("rel: %d %d curamp=%.2f\n",k,j->envpos, j->curamp);
					j->slowcount++;
					j->envpos += ((j->slowcount % slowfactor) == 0) ? 1 : 0;
					if (j->envpos == MAX_ENV) {
						//end of release
						j->playing = false;
						j->notenum = -1;
						j->envpos = 0;
						j->slowcount = 0;
						j->curamp = 0.f;
						j->vi = 0.f;
						j->rampstate = 0.f;
						//printf("killed, n=%d\n",k);
					}
				} else {
					//sustain
					j->curamp = j->vi * env_y[MAX_ENV/2];
					//printf("sustain...\n");
				}
			}
		}
		for (k = 0; k < 128; k++) {
			float rampfreq;
			if (voice[k].curamp < 0.f && voice[k].playing) printf("WTF NEG\n");
			if (!voice[k].playing) continue;
			signal = true;
			rampfreq = 440.0*powf(2.0, (voice[k].notenum-48.0-36)/12.);

			// ramp sawtooth
			RD_0 = 1.4247585730565955E-4*srate/44100.*rampfreq + voice[k].rampstate;
			if (RD_0>6.283185307179586) {RD_0 -= 6.283185307179586;}
			if (RD_0<-6.283185307179586) {RD_0 += 6.283185307179586;}
			voice[k].rampstate = RD_0;

			// wavetable
			wave = wavetable(voice[k].rampstate);
			power += sqrt(voice[k].curamp);

			outl += wave*voice[k].curamp/5.;
			outr += wave*voice[k].curamp/5.;
		}
		if (signal) {
			//outl;
			//outr;
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
