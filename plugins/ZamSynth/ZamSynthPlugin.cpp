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
    case paramGraph:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "Graph toggle";
        parameter.symbol     = "graph";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
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
    case paramGraph:
        return graph;
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
    case paramGraph:
        graph = value;
        break;
    }
}

void ZamSynthPlugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    gain = 0.0f;
    graph = 0.0f;

    /* Default variable values */
    for (int i = 0; i < 127; i++) {
        voice[i].playing = false;
	voice[i].notenum = 0;
	voice[i].envpos = 0.f;
	voice[i].curamp = 0.f;
	voice[i].vi = 0.f;
        voice[i].rampstate = 0.f;
    }
    
    curvoice = voice; //ptr to first voice

    for (int i = 0; i < AREAHEIGHT; i++) {
        wave_y[i] = sin(i*2.*M_PI/d_getSampleRate()*1000.);
    }

    for (int i = 0; i < MAX_ENV; i++) {
        env_y[i] = sin(i*2.*M_PI/d_getSampleRate()*1000./2.);
    }
    /* reset filter values */
    d_activate();
}

void ZamSynthPlugin::d_setState(const char* key, const char* value)
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
			//printf("dsp wave_y[%d]=%.2f ", i, wave_y[i]);
			tmp = strtok(NULL, " ");
		}
	}
}

void ZamSynthPlugin::d_initStateKey(unsigned int index, d_string& key)
{
	if (index == 0) key = "waveform";
	if (index == 1) key = "envelope";
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
	int index = (int) ((in / (2.0 * M_PI)) * (AREAHEIGHT-1.0));
	return (wave_y[index]);
	//return (sin(in));
}

void ZamSynthPlugin::d_run(float**, float** outputs, uint32_t frames,
				const MidiEvent* midievent, uint32_t midicount)
{
	float srate = d_getSampleRate();
	int slowdown = (int) srate / 2000; 
	uint32_t i,j;
	float RD_0;
	int vn;
	
	for (i = 0; i < midicount; i++) {
		int type = midievent[i].buf[0] & 0xF0;
		int chan = midievent[i].buf[0] & 0x0F;
		int num = midievent[i].buf[1];
		int vel = midievent[i].buf[2];
		if (type == 0x90 && chan == 0x0) {
			// NOTE ON
			//printf("ON: Note\n");
			//find voice with current notenum
			vn = -1;
			nvoices = 0;
			for (int k = 0; k < 128; k++) {
				if (voice[k].playing) {
					nvoices++;
					if (voice[k].notenum == num) {
						vn = k;
					}
				}
			}
			if (vn != -1) {
				//printf("note already playing\n");
				//begin attack
				voice[vn].playing = true;
				voice[vn].envpos = 1;
				voice[vn].notenum = num;
				voice[vn].vi = vel / 127.f;
				voice[vn].curamp = vel / 127.f;
				voice[vn].rampstate = 0;
				continue; // restart note
			}
			nvoices++;
			curvoice = &voice[nvoices];
			if (nvoices > MAX_VOICES) {
				//printf("steal first voice\n");
				curvoice = voice; // steal first voice
				nvoices--;
			}
			//printf("ON: nvoices = %d\n", nvoices);
			//printf("ON: begin attack\n");
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
			vn = -1;
			int v2 = -1;
			nvoices = 0;
			for (int k = 0; k < 128; k++) {
				if (voice[k].playing) {
					nvoices++;
					if (voice[k].notenum == num) {
						vn = k;
					}
				}
				if (!voice[k].playing && voice[k].notenum == num) {
					v2 = k;
				}
			}
			if (vn != -1) {
				voice[vn].envpos = MAX_ENV / 2 + 1; // begin release;
				//printf("begin release\n");
				continue;
			}
			if (v2 != -1) {
				//printf("note already off, do nothing\n");
				voice[v2].envpos = 0;
				voice[v2].curamp = 0.f;
				voice[v2].vi = 0.f;
				voice[v2].playing = false;
				//printf("OFF: nvoices = %d\n", nvoices);
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
				if ((int) j->envpos <= 0) {
					//silence
					j->curamp = 0.f;
					j->playing = false;
					j->envpos = 0;
				} else if ((int) j->envpos > 0 && (int) j->envpos < MAX_ENV / 2) {
					//attack
					j->curamp = j->vi * env_y[(int)(j->envpos)];
					//printf("att: %d %d curamp=%.2f\n",k,j->envpos, j->curamp);
					j->envpos += 1. / slowdown;
				} else if ((int) j->envpos > MAX_ENV / 2) {
					//release
					j->curamp = j->vi * env_y[(int)(j->envpos)];
					//printf("rel: %d %d curamp=%.2f\n",k,j->envpos, j->curamp);
					j->envpos += 1. / slowdown;
					if ((int) j->envpos == MAX_ENV) {
						//end of release
						j->envpos = 0.f;
						j->curamp = 0.f;
						j->vi = 0.f;
						j->playing = false;
						curvoice--;
						nvoices--;
						if (nvoices < 0) {
							curvoice = voice + MAX_VOICES-1;
							nvoices++;
						}
						//printf("killed, nvoices=%d\n",nvoices);
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
			if (voice[k].curamp < 0.01f) continue;
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

			outl += wave*voice[k].curamp;
			outr += wave*voice[k].curamp;
		}
		if (signal) {
			outl *= (nvoices)/(10. * power);
			outr *= (nvoices)/(10. * power);
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
