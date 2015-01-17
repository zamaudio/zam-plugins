/*
 * ZamPiano physically modelled piano synth
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

#include "ZamPianoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamPianoPlugin::ZamPianoPlugin()
    : Plugin(paramCount, 1, 0), // 1 program, 0 states
	fftvars(MAX_SAMP_FRAME)
{
    // set default values
    d_setProgram(0);
}

ZamPianoPlugin::~ZamPianoPlugin()
{
}

// -----------------------------------------------------------------------
// Init

void ZamPianoPlugin::d_initParameter(uint32_t index, Parameter& parameter)
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

void ZamPianoPlugin::d_initProgramName(uint32_t index, d_string& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamPianoPlugin::d_getParameterValue(uint32_t index) const
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

void ZamPianoPlugin::d_setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramGain:
        gain = value;
        break;
    }
}

void ZamPianoPlugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    gain = 0.0f;

    /* reset filter values */
    d_activate();
}

/*
void ZamPianoPlugin::d_setState(const char*, const char*)
{
}

void ZamPianoPlugin::d_initStateKey(unsigned int, d_string&)
{
}
*/

// -----------------------------------------------------------------------
// Process

double ZamPianoPlugin::f0(int n)
{
	return 15.5 * exp(0.059 * n);
}

void ZamPianoPlugin::d_activate()
{
	int i;
	for (i = 0; i < 128; i++) {
		ff0[i] = f0(i);
		ff1[i] = 0.;
		integrala[i] = 0.;
		integralb[i] = 0.;
		timepos[i] = 0.;
		note[i].state = SILENT;
		note[i].vel = 0.;
	}	
	for (i = 0; i < MAX_SAMP_FRAME; i++) {
		oldbuf[0][i] = oldbuf[1][i] = 0.;
	}

	dumpvals();
}

double ZamPianoPlugin::q0(int n)
{
	return 0.183 * exp(0.045 * n);
}

double ZamPianoPlugin::p(int n)
{
	return 3.7 + 0.015 * n;
}

double ZamPianoPlugin::alpha(int n)
{
	return (259.5 - 0.58*n + 6.6e-2*n*n - 1.25e-3*n*n*n + 1.172e-5*n*n*n*n)/1000.;
}

double ZamPianoPlugin::mhammer(int n)
{
	return 11.074 - 0.074*n + 1e-4*n*n;
}

double ZamPianoPlugin::lstring(int n)
{
	if (n <= 26) {
		return 1239.2 - (1239.2-831.2)*(n-1) / 25.;
	}
	if (n == 27) return 1031.;
	if (n == 28) return 1007.4;
	if (n == 29) return 985.3;
	if (n == 30) return 964.1;
	if (n > 30 && n <= 49) {
		return 964.1 - (964.1-399.3)*(n-30) / 19;
	}
	if (n > 49 && n <= 61) {
		return 399.3 - (399.3-208.3)*(n-49) / 12;
	}
	return 208.3;
}

double ZamPianoPlugin::mstring(int n)
{
	return lstring(n) / 1000. * mu(n);
}

double ZamPianoPlugin::fk1(double fk, double dt, double t, int n)
{
	return fk + dt*(q0(n)*(pow(t, p(n)) + alpha(n)*p(n)*pow(t, p(n)-1.)));
}

double ZamPianoPlugin::c(int n)
{
	float f = 440. * pow(2., (n - 48.) / 12.);
	return 2*f*lstring(n);
}

double ZamPianoPlugin::mu(int n)
{
	if (n <= 26) {
		return 290.6 - (290.6-16.7)*(n-1) / 25.;
	}
	if (n == 27) return 12.2;
	if (n == 28) return 10.8;
	if (n == 29) return 10.4;
	if (n == 30) return 7.8;
	if (n > 30 && n <= 49) {
		return 7.8 - (7.8-5.6)*(n-30) / 19;
	}
	if (n > 49 && n <= 61) {
		return 5.6 - (5.6-5.0)*(n-49) / 12;
	}
	return 5.0;
}

double ZamPianoPlugin::a(int i, int n)
{
	float strike = 0.105;
	if (i == 0.) return 1.;
	return sin(strike*i*M_PI)/(i*M_PI*c(n)*mu(n));
}

double ZamPianoPlugin::w(int i, double f)
{
	return M_PI * 2. * f; // *i;
}

void ZamPianoPlugin::e(int n, double vhammer, int *state, float *out, uint32_t frames)
{
	double sr = d_getSampleRate();
	double t0 = 0.008; //1000  0.8ms contact time
	//int N = t0 * frames;
	double dt = 10./sr;
	double t;
	uint32_t i;
	vhammer *= 1000.;
	memcpy(fftvars.cmplex[0], oldbuf[0], 4096*sizeof(float));
	memcpy(fftvars.cmplex[1], oldbuf[1], 4096*sizeof(float));
	
	ff0[n] = (*state == STRIKE) ? f0(n) : ff0[n];
	timepos[n] = (*state == STRIKE) ? 0. : timepos[n];
	if (*state == STRIKE) *state = ATTACK;
	
	for (i = 0; i < frames; i++) {
		if (timepos[n] >= t0*frames) {
			*state = SUSTAIN;
		}
		if (timepos[n] > 0.1*frames) {
			*state = SILENT;
			timepos[n] = 0;
			integrala[n] = 0.;
			integralb[n] = 0.;
			printf("SILENT\n");
		}
		if (*state == ATTACK) {
			double ii = i / (double) (frames + 1.);
			ff1[n] = fk1(ff0[n], dt, timepos[n], n);
			integrala[n] += ff1[n]*a(i, n) * cos(w(i, ii) * timepos[n]);
			integralb[n] -= ff1[n]*a(i, n) * sin(w(i, ii) * timepos[n]);
	
			ff0[n] = ff1[n];
			timepos[n] += dt;
			printf("%f, %f, %f\n", integrala[n], integralb[n], ff0[n]);
			fftvars.cmplex[0][i] = ( 2.*mstring(n)
				* w(i, ii)*w(i, ii)
				/ (mhammer(n)*vhammer*vhammer)
				* (integrala[n]*integrala[n]) );
			fftvars.cmplex[1][i] = ( 2.*mstring(n)
				* w(i, ii)*w(i, ii)
				/ (mhammer(n)*vhammer*vhammer)
				* (integralb[n]*integralb[n]) );
			//printf("%f %f\n", fftvars.cmplex[0][i], fftvars.cmplex[1][i]);
		} else {
			timepos[n] += frames;
			for (i = 0; i < frames; i++) {
				fftvars.cmplex[0][i] = 0.2;//oldbuf[0][i]*0.2; 
				fftvars.cmplex[1][i] = 0.2;//oldbuf[1][i]*0.2;
			}
		}
	}
/*
	for (i = 0; i < frames; i++) {
		out[i] += fftvars.cmplex[0][i];
	}
*/
	fftvars.cmplex[0][0] = 0.;
	fftvars.cmplex[1][0] = 0.;
	memcpy(oldbuf[0], fftvars.cmplex[0], 4096*sizeof(float));	
	memcpy(oldbuf[1], fftvars.cmplex[1], 4096*sizeof(float));

	fftvars.fft_inverse();
	
	float scale = 1. / fftvars.ffttime[0];
	for (i = 1; i < frames; i++) {
		out[i] += fftvars.ffttime[i] * scale;
		//printf("%e\n", out[i]);
	}
	out[0] += 1.;
}

void ZamPianoPlugin::dumpvals(void)
{
	int i;
	printf("i f0 q0 p alpha mhammer mstring lstring mstring\n");
	for (i = 0; i < 88; i++) {
		printf("%d %f %f %f %f %f %f %f %f\n", i, f0(i), q0(i), p(i), alpha(i), mhammer(i), mstring(i), lstring(i), mstring(i));
	}
}

void ZamPianoPlugin::d_run(const float**, float** outputs, uint32_t frames,
				const MidiEvent* midievent, uint32_t midicount)
{
	uint32_t i;
	bool signal;

	for (i = 0; i < midicount; i++) {
		int type = midievent[i].data[0] & 0xF0;
		int chan = midievent[i].data[0] & 0x0F;
		int n = midievent[i].data[1];
		int v = midievent[i].data[2];
		if (type == 0x90 && chan == 0x0) {
			// NOTE ON
			note[n].state = STRIKE;
			note[n].vel = v / 127.f;
		}
		else if (type == 0x80 && chan == 0x0) {
			// NOTE OFF
			if (note[n].state != SILENT)
				note[n].state = RELEASE;
		}
	}

	for (i = 0; i < frames; i++) {
		outputs[0][i] = 0.f;
		outputs[1][i] = 0.f;
	}
	signal = false;
	int k;
	for (k = 0; k < 128; k++) {
		if (k > 87) continue;
		if (note[k].state == SILENT) {
			continue;
		}
		signal = true;
		if (note[k].state == STRIKE || note[k].state == ATTACK) {
			printf("ATT: %d\n", k);
			e(k, 5.*note[k].vel, &note[k].state, outputs[0], frames);
		} else if (note[k].state == SUSTAIN) {
			printf("SUS: %d\n", k);
			e(k, 0., &note[k].state, outputs[0], frames);
		} else if (note[k].state == RELEASE) {
			printf("REL: %d\n", k);
			e(k, 0., &note[k].state, outputs[0], frames);
		}
	}
	for (i = 0; i < frames; i++) {
		if (signal) {
			sanitize_denormal(outputs[0][i]);
			sanitize_denormal(outputs[1][i]);
			outputs[1][i] = outputs[0][i]*from_dB(gain);
			outputs[0][i] *= from_dB(gain);
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
