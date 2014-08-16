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

#ifndef ZAMNOISEPLUGIN_HPP_INCLUDED
#define ZAMNOISEPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include <fftw3.h>
#include <math.h>

typedef struct
{
  int nfft;        // size of FFT
  int numfreqs;    // number of frequencies represented (nfft/2 + 1)
  float* ffttime;
  int* flag;
  fftwf_complex* cmplex;
  fftwf_complex* noisespectra;

  fftwf_plan forwardplan;
  fftwf_plan reverseplan;
} fft_vars;

typedef struct {
	unsigned long cbsize; // size of circular buffer
	unsigned long corrsize; // cbsize/2 + 1
	unsigned long cbiwr;  //write pointer;
	float* cbi;
	float* cbf; // circular formant correction buffer	
} CircularBuffer;


START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamNoisePlugin : public Plugin
{
public:
    enum Parameters
    {
        paramNoiseToggle,
        paramCount
    };

    ZamNoisePlugin();
    ~ZamNoisePlugin() override;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamNoise";
    }

    const char* d_getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* d_getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t d_getVersion() const noexcept override
    {
        return 0x1000;
    }

    long d_getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'N', 'O', 'I');
    }

    // -------------------------------------------------------------------
    // Init

    void d_initParameter(uint32_t index, Parameter& parameter) ;
    void d_initProgramName(uint32_t index, d_string& programName) ;

    // -------------------------------------------------------------------
    // Internal data

    float d_getParameterValue(uint32_t index) const ;
    void  d_setParameterValue(uint32_t index, float value) ;
    void  d_setProgram(uint32_t index) ;

    // -------------------------------------------------------------------
    void d_activate() ;
    void d_deactivate() ;
    void d_run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------
	float noisetoggle;

	fft_vars* fft_con(int nfft);
	void fft_des(fft_vars* fftvars);
	void fft_forward(fft_vars* fftvars);
	void fft_inverse(fft_vars* fftvars);
	void fft_apply(const float *ins, float *outs, uint32_t frames, fft_vars* fftvars, long int N);
	void InstantiateCircularBuffer(CircularBuffer* buffer, unsigned long SampleRate);
	void fft_thresh(fft_vars* fftvars, CircularBuffer* buffer, long int N);
	inline void IncrementPointer(CircularBuffer& buffer);	
	void init(float srate);
	int noverlap;
	CircularBuffer buffer;
	fft_vars* fmembvars; // member variables for fft routine
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMNOISE_HPP_INCLUDED
