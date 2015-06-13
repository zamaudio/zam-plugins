/*
 * ZamSFZ polyphonic SFZ player
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

#include "ZamSFZPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamSFZPlugin::ZamSFZPlugin()
    : Plugin(paramCount, 1, 1) // 1 program, 1 state
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamSFZPlugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramGain:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Gain";
        parameter.symbol     = "gain";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -30.0f;
        parameter.ranges.max = 30.0f;
        break;
    }
}

void ZamSFZPlugin::initProgramName(uint32_t index, String& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamSFZPlugin::getParameterValue(uint32_t index) const
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

void ZamSFZPlugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramGain:
        gain = value;
        break;
    }
}

void ZamSFZPlugin::setState(const char* key, const char* value)
{
	if (strcmp(key, "filepath") == 0) {
		char *tmp;
		char file[1024] = {0};
		snprintf(file, 1024, "%s", value);
		tmp = strrchr(file, '/');
		filename.assign(tmp, 1, strlen(tmp) - 1);
		path.assign(value, 0, strlen(value) - strlen(tmp));
		loading = true;
		printf("Path: %s\nFile: %s\n", path.c_str(), filename.c_str());
    		sfz.clearsamples();
		sfz.loadsamples(path, filename);
		sfz.pitchshiftsamples((int)getSampleRate());
		loading = false;
	}
}

void ZamSFZPlugin::initState(unsigned int key, String& val, d_string&)
{
	if (key == 0)
		val = "filepath";	
}

void ZamSFZPlugin::loadProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    gain = 0.0f;
    speed = 10.0f;

    /* Default variable values */
    for (int i = 0; i < MAX_VOICES; i++) {
        voice[i].playing = false;
	voice[i].notenum = -1;
	voice[i].envpos = 0;
	voice[i].slowcount = 0;
	voice[i].curamp = 0.f;
	voice[i].vi = 0.f;
        voice[i].rampstate = 0;
        voice[i].maxsamples = MAX_SAMPLES;
    }

    curvoice = voice; //ptr to first voice

    for (int i = 0; i < MAX_ENV/2; i++) {
        env_y[i] = 1.f;
    }
    for (int i = MAX_ENV/2; i < MAX_ENV; i++) {
        env_y[i] = (MAX_ENV-i)/((float)(MAX_ENV/2.));
    }
    env_y[0] = 0.f;
    env_y[MAX_ENV-1] = 0.f;
    /* reset filter values */
    activate();
}

// -----------------------------------------------------------------------
// Process

void ZamSFZPlugin::activate()
{
}


void ZamSFZPlugin::run(const float**, float** outputs, uint32_t frames,
				const MidiEvent* midievent, uint32_t midicount)
{
	float srate = getSampleRate();
	int slowfactor = (int) srate / (speed * 2400); // 1-20 ~ 20-1
	uint32_t i;

  if (loading == true) {
	for (i = 0; i < frames; i++) {
		outputs[0][i] = 0.f;
		outputs[1][i] = 0.f;
	}
  } else {
	for (i = 0; i < midicount; i++) {
		int type = midievent[i].data[0] & 0xF0;
		int chan = midievent[i].data[0] & 0x0F;
		int num = midievent[i].data[1];
		int vel = midievent[i].data[2];
		if (type == 0x90 && chan == 0x0) {
			// NOTE ON
			int newvoice = -1;
			//printf("ON: Note %d\n", num);
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

	bool signal;
	float wavel;
	float waver;
	float outl;
	float outr;
	for (i = 0; i < frames; i++) {
		signal = false;
		outl = outr = 0.f;
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
					j->rampstate = 0;
				} else if (j->envpos > 0 && (int) j->envpos < MAX_ENV / 2) {
					//attack
					j->curamp = j->vi * env_y[j->envpos];
					//printf("att: %d envpos=%d curamp=%.2f\n",k,j->envpos, j->curamp);
					j->slowcount++;
					j->envpos += ((j->slowcount % slowfactor) == 0) ? 1 : 0;
				} else if (j->envpos > MAX_ENV / 2) {
					//release
					j->curamp = j->vi * env_y[j->envpos];
					//printf("rel: %d envpos=%d curamp=%.2f\n",k,j->envpos, j->curamp);
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
						j->rampstate = 0;
						//printf("killed, n=%d\n",k);
					}
				} else if (j->envpos >= j->maxsamples || j->rampstate >= j->maxsamples) {
					j->playing = false;
					j->notenum = -1;
					j->envpos = 0;
					j->slowcount = 0;
					j->curamp = 0.f;
					j->vi = 0.f;
					j->rampstate = 0;
				} else {
					//sustain
					j->curamp = j->vi * env_y[MAX_ENV/2];
					//printf("sustain...\n");
				}
			}
		}
		for (k = 0; k < 128; k++) {
			if (voice[k].curamp < 0.f && voice[k].playing)
				printf("WTF NEG\n");
			if (!voice[k].playing) {
				voice[k].rampstate = 0;
				continue;
			}
			signal = true;
			voice[k].rampstate++;
			if (voice[k].rampstate >= voice[k].maxsamples) {
				wavel = waver = 0.f;
				voice[k].rampstate = 0;
				voice[k].playing = false;
				voice[k].notenum = -1;
				voice[k].envpos = 0;
				voice[k].slowcount = 0;
				voice[k].curamp = 0.f;
				voice[k].vi = 0.f;
			} else {
				int n = voice[k].notenum;
				int v = voice[k].vi * 127.;
				int l = 0;
				for (int k = 0; k < sfz.layers[n].max; k++) {
					if (sfz.layers[n].l[k].lovel <= v && v <= sfz.layers[n].l[k].hivel) {
						l = k;
						break;
					}
				}
				int pos = voice[k].rampstate;
				wavel = sfz.sample[n][l][0][pos];
				waver = sfz.sample[n][l][1][pos];
			}
			outl += wavel*voice[k].curamp/2.;
			outr += waver*voice[k].curamp/2.;
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
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamSFZPlugin();
}

// -----------------------------------------------------------------------
END_NAMESPACE_DISTRHO
