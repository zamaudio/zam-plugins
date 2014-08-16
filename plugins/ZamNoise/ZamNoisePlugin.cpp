/*
 * ZamNoise  Noise detection and removal plugin
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

#include "ZamNoisePlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

// Constructor for FFT routine
fft_vars* ZamNoisePlugin::fft_con(int nfft) {
	fft_vars* fftvars = (fft_vars*) malloc(sizeof(fft_vars));

	fftvars->nfft = nfft;
	fftvars->numfreqs = nfft/2 + 1;

	fftvars->ffttime = (float*) fftwf_malloc(nfft* sizeof(float));
	fftvars->cmplex = (fftwf_complex*) fftwf_malloc(fftvars->numfreqs* sizeof(fftwf_complex));
	fftvars->noisespectra = (fftwf_complex*) fftwf_malloc(fftvars->numfreqs* sizeof(fftwf_complex));
	fftvars->flag = (int*) malloc(nfft* sizeof(int));
	fftvars->forwardplan=fftwf_plan_dft_r2c_1d(nfft,fftvars->ffttime, fftvars->cmplex, FFTW_ESTIMATE);
	fftvars->reverseplan=fftwf_plan_dft_c2r_1d(nfft,fftvars->cmplex,fftvars->ffttime, FFTW_ESTIMATE);
	return fftvars;
}

// Destructor for FFT routine
void ZamNoisePlugin::fft_des(fft_vars* fftvars)
{
	fftwf_destroy_plan(fftvars->forwardplan);
	fftwf_destroy_plan(fftvars->reverseplan);
	fftwf_free(fftvars->ffttime);
	fftwf_free(fftvars->cmplex);
	fftwf_free(fftvars->noisespectra);
	free(fftvars->flag);
	free(fftvars);
}

void ZamNoisePlugin::fft_forward(fft_vars* fftvars)
{
	fftwf_execute(fftvars->forwardplan);
}

void ZamNoisePlugin::fft_inverse(fft_vars* fftvars)
{
	fftwf_execute(fftvars->reverseplan);
}

ZamNoisePlugin::ZamNoisePlugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{

    ZamNoisePlugin::init(d_getSampleRate());
    
    // set default values
    d_setProgram(0);

    // reset
    d_deactivate();
}

ZamNoisePlugin::~ZamNoisePlugin()
{
        fft_des(fmembvars);
       	free(buffer.cbi);
       	free(buffer.cbf);
}

// -----------------------------------------------------------------------
// Init

void ZamNoisePlugin::d_initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramNoiseToggle:
        parameter.hints      = PARAMETER_IS_AUTOMABLE | PARAMETER_IS_BOOLEAN;
        parameter.name       = "Noise Capture";
        parameter.symbol     = "noisecapture";
        parameter.unit       = "";
        parameter.ranges.def = 0.f;
        parameter.ranges.min = 0.f;
        parameter.ranges.max = 1.f;
        break;
    default:
	break;
    }
}

void ZamNoisePlugin::d_initProgramName(uint32_t index, d_string& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamNoisePlugin::d_getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramNoiseToggle:
        return noisetoggle;
        break;
    default:
	return 0.0f;
        break;
    }
}

void ZamNoisePlugin::d_setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramNoiseToggle:
        if (value == 1.f) {
		memset(buffer.cbi, 0, buffer.cbsize*sizeof(float));
	}
	noisetoggle = value;
        break;
    }
}

void ZamNoisePlugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    noisetoggle = 0.f;

    d_activate();
}


void ZamNoisePlugin::InstantiateCircularBuffer(CircularBuffer* buffer, unsigned long SampleRate) {
	if (SampleRate>=88200) {
		buffer->cbsize = 32768;
	}
	else {
		buffer->cbsize = 16384;
	}
	buffer->corrsize = buffer->cbsize / 2 + 1;

	buffer->cbi = (float*) calloc(buffer->cbsize, sizeof(float));
	buffer->cbf = (float*) calloc(buffer->cbsize, sizeof(float));
	buffer->cbiwr = 0;
}

inline void ZamNoisePlugin::IncrementPointer(CircularBuffer& buffer) {
        // Input write pointer logic
        CircularBuffer *buf = &buffer;
	buf->cbiwr++;
        if (buf->cbiwr >= buf->cbsize) {
                buf->cbiwr = 0;
        }
}


void ZamNoisePlugin::fft_thresh(fft_vars* fftvars, CircularBuffer* buffer, long int N) {

	// Window and fill FFT buffer
	long int i;
	float window[N];
	for (i=0; i<N; i++) {
        	window[i] = (float) (-0.5*cos(4*M_PI*i/(float)(2*N - 1)) + 0.5);
		float inputbuffer=buffer->cbi[(buffer->cbiwr-i+N)%N];
		fftvars->ffttime[i] = inputbuffer*window[i];
	}
	// Calculate FFT
	fft_forward(fftvars);
	memcpy(fftvars->noisespectra, fftvars->cmplex, (N/2+1)*sizeof(fftwf_complex));

	// Remove DC
	fftvars->cmplex[0][0] = 0;
	fftvars->cmplex[0][1] = 0;

	// Find max spectra energy spike
	float energy;
	float max = 0.f;
	for (i=0; i<N/2; i++) {
		energy = fftvars->cmplex[i][0]*fftvars->cmplex[i][0] + fftvars->cmplex[i][1]*fftvars->cmplex[i][1];
		if (energy > max) max = energy;
		fftvars->flag[i] = 0;
	}
	// Flag soft thresholds
	for (i=0; i<N/2; i++) {
		//if ((fftvars->cmplex[i][0]*fftvars->cmplex[i][0] + fftvars->cmplex[i][1]*fftvars->cmplex[i][1]) > max * 0.05) {
			fftvars->flag[i] = 1;
		//}
	}
}

void ZamNoisePlugin::fft_apply(const float *ins, float *outs, uint32_t frames, fft_vars* fftvars, long int N)
{
	// Fill FFT buffer
	uint32_t i;
	for (i=0; i<frames; i++) {
		fftvars->ffttime[i] = ins[i];
	}
	for (i=frames; i<N; i++) {
		fftvars->ffttime[i] = 0.f;
	}
        // Calculate FFT
        fft_forward(fftvars);

        // Remove DC
        fftvars->cmplex[0][0] = 0;
        fftvars->cmplex[0][1] = 0;
        long int Nf=N/2 -1;

        // Subtract noise spectra in complex domain
        for (i=1; i<Nf; i++) {
	 if (fftvars->flag[i] == 1) {
		float mag = sqrt(fftvars->cmplex[i][0]*fftvars->cmplex[i][0]+fftvars->cmplex[i][1]*fftvars->cmplex[i][1]);
		float ang = atan2(fftvars->cmplex[i][1], fftvars->cmplex[i][0]);
		float noise = sqrt(fftvars->noisespectra[i][0]*fftvars->noisespectra[i][0]+fftvars->noisespectra[i][1]*fftvars->noisespectra[i][1]);
		if (mag - noise > 0.f) {
			if (noise > 0.9)
				mag -= noise;
		} else
			mag = 0.f;
			
		fftvars->cmplex[i][0] = mag*cos(ang);
		fftvars->cmplex[i][1] = mag*sin(ang);
	 }
        }

        // Calculate IFFT
        fft_inverse(fftvars);

        // Normalize
        for (i=0; i<N; i++) {
                fftvars->ffttime[i] /= N;
        }

	for (i=0; i<frames; i++) {
		outs[i] = ins[i];
		outs[i] = fftvars->ffttime[i];
	}
}

void ZamNoisePlugin::init (float fsamp)
{
        InstantiateCircularBuffer(&buffer,fsamp);
        unsigned long N=buffer.cbsize;
        fmembvars = fft_con(N);
        noverlap = 5;
}


// -----------------------------------------------------------------------
// Process

void ZamNoisePlugin::d_activate()
{
}

void ZamNoisePlugin::d_deactivate()
{
}

void ZamNoisePlugin::d_run(const float** inputs, float** outputs, uint32_t frames)
{
	unsigned long N = ZamNoisePlugin::buffer.cbsize;
	uint32_t i;
	for (i = 0; i < frames; i++) {
		// load data into circular buffer when toggled
		if (noisetoggle == 1.f) {
			buffer.cbi[buffer.cbiwr] = inputs[0][i];
                	IncrementPointer(buffer);
			// Every N/noverlap samples, redetect noise
		        if ((buffer.cbiwr)%(N/noverlap) == 0) {
				fft_thresh(fmembvars, &buffer, N);
			}
		}
	}
	fft_apply(inputs[0], outputs[0], frames, fmembvars, N);
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
	return new ZamNoisePlugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
