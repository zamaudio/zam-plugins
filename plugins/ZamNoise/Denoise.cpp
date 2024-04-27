/*****************************************************************************
*   Gnome Wave Cleaner Version 0.19
*   Copyright (C) 2001 Jeffrey J. Welty
*   (Modified by Damien Zammit for zam-plugins 2014)
*   
*   This program is free software; you can redistribute it and/or
*   modify it under the terms of the GNU General Public License
*   as published by the Free Software Foundation; either version 2
*   of the License, or (at your option) any later version.
*   
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*   
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*******************************************************************************/

#include "Denoise.hpp"
#include "bessel.inc"

double Denoise::hypergeom(double theta)
{
    if(theta < 7.389056)
	return exp(-theta/2.0)*(1.0+theta*i0(theta/2.0)+theta*i1(theta/2.0));
    else
	return exp(0.09379 + 0.50447*log(theta));
}

double Denoise::gain_em(double Rprio, double Rpost)
{
    /* Ephraim-Malah classic noise suppression, from 1984 paper */

    double gain = 0.886226925*sqrt(1.0/(1.0+Rpost)*(Rprio/(1.0+Rprio)));

    gain *= hypergeom((1.0+Rpost)*(Rprio/(1.0+Rprio)));

    return gain;
}

double Denoise::blackman(int k, int N)
{
    double p = ((double)(k))/(double)(N-1);
    return 0.42-0.5*cos(2.0*M_PI*p) + 0.08*cos(4.0*M_PI*p);
}

double Denoise::hanning(int k, int N)
{
    double p = ((double)(k))/(double)(N-1);
    return 0.5 - 0.5 * cos(2.0*M_PI*p);
}

double Denoise::blackman_hybrid(int k, int n_flat, int N)
{
    if(k >= (N-n_flat)/2 && k <= n_flat+(N-n_flat)/2-1) {
	return 1.0;
    } else {
	double p;
	if(k >= n_flat+(N-n_flat)/2-1) k -= n_flat;
	p = (double)(k)/(double)(N-n_flat-1);
	return 0.42-0.5*cos(2.0*M_PI*p) + 0.08*cos(4.0*M_PI*p);
    }
}

double Denoise::fft_window(int k, int N, int window_type)
{
    if(window_type == DENOISE_WINDOW_BLACKMAN) {
	return blackman(k, N);
    } else if(window_type == DENOISE_WINDOW_BLACKMAN_HYBRID) {
	return blackman_hybrid(k, N-N/4, N);
    } else if(window_type == DENOISE_WINDOW_HANNING_OVERLAP_ADD) {
	return hanning(k, N);
    }

    return 0.0;
}

void Denoise::fft_remove_noise(const float* ins, float* outs, uint32_t frames, fftw_real noise_min2[], fftw_real noise_max2[], float amount, FFTW(plan) *pFor, FFTW(plan) *pBak)
{
    int k;
    uint32_t i;
    fftw_real noise2[DENOISE_MAX_FFT/2+1] = {0};
    fftw_real Y2[DENOISE_MAX_FFT/2+1] = {0};
    static fftw_real bY2_prev[DENOISE_MAX_FFT/2+1] = {0};
    static fftw_real bgain_prev[DENOISE_MAX_FFT/2+1] = {0};
    fftw_real *Y2_prev, *gain_prev;

    Y2_prev = bY2_prev;
    gain_prev = bgain_prev;

    for (i = 0; i < frames; i++) {
	windowed[i] = ins[i];
    }
    for (i = frames; i < (uint32_t)FFT_SIZE; i++) {
    	windowed[i] = 0.f;
    }

    FFTW(execute)(*pFor);

    {
	double sum_log_p = 0.0;
	double sum_p = 0.0;

	for (k = 1; k <= FFT_SIZE/2 ; ++k) {
	    noise2[k] = noise_min2[k] + 0.5*(noise_max2[k] - noise_min2[k]);
	    if(k < FFT_SIZE/2) {
		Y2[k] = out[k]*out[k] + out[FFT_SIZE-k]*out[FFT_SIZE-k];
	    } else {
		Y2[k] = out[k]*out[k];
	    }
	    sum_log_p += log10(Y2[k]);
	    sum_p += Y2[k];
	}
    }

    for (k = 1; k <= FFT_SIZE/2 ; ++k) {
	if(noise2[k] > 0.0) {
	    double gain, Fk, Gk;

	    double Rpost = MAX(Y2[k]/noise2[k]-1.0, 0.0);
	    double alpha = dn_gamma;
	    double Rprio;

	    if(prev_sample == 1)
	        Rprio = (1.0-alpha)*Rpost+alpha*gain_prev[k]*gain_prev[k]*Y2_prev[k]/noise2[k];
	    else
	        Rprio = Rpost;

	    gain = gain_em(Rprio, Rpost);
	    gain_prev[k] = gain;
	    Y2_prev[k] = Y2[k];

	    Fk = amount * (1.0-gain);

	    if(Fk < 0.0)
	        Fk = 0.0;
	    if(Fk > 1.0)
	        Fk = 1.0;

	    Gk =  1.0 - Fk;

	    out[k] *= Gk;

	    if(k < FFT_SIZE/2)
	        out[FFT_SIZE-k] *= Gk;
	}
    }

    /* the inverse fft */
    FFTW(execute)(*pBak);

    for(k = 0 ; k < FFT_SIZE ; k++)
	windowed[k] /= (double)(FFT_SIZE);

    prev_sample = 1;

    for (i = 0; i < frames; i++) {
    	outs[i] = windowed[i];
    }
}

Denoise::Denoise(float srate) {
    int k;
    FFT_SIZE = 4096;
    dn_gamma = 0.95;
    n_noise_samples = FFT_SIZE;
    rate = (int) srate;
    noisebufpos = 0;
    prev_sample = 0;
    
    pFor = FFTW(plan_r2r_1d)(FFT_SIZE, windowed, out, FFTW_R2HC, FFTW_ESTIMATE);
    pBak = FFTW(plan_r2r_1d)(FFT_SIZE, out, windowed, FFTW_HC2R, FFTW_ESTIMATE);

    pForNoise = FFTW(plan_r2r_1d)(FFT_SIZE, noise, noisefft, FFTW_R2HC, FFTW_ESTIMATE);

    window_type = DENOISE_WINDOW_BLACKMAN;

    for(k = 0; k < FFT_SIZE; k++) {
	window_coef[k] = fft_window(k,FFT_SIZE, window_type);
	prev_frame[k] = 0.0;
	noise[k] = 0.0;
	noise_max[k] = 0.0;
	noise_min[k] = 0.0;
    }
}


void Denoise::process(const float* ins, float* outs, float* noisebuffer, uint32_t frames, int togglenoise, float amount) {

	if (togglenoise == 1) {
		uint32_t i;
		for (i = 0; i < frames; i++) {
			noisebuffer[noisebufpos] = ins[i];
			noisebufpos++;
			if (noisebufpos >= n_noise_samples)
				noisebufpos = 0;

			if (noisebufpos % (n_noise_samples/2) == 0) {
				get_noise_sample(noisebuffer, noise_min, noise_max);
			}
			outs[i] = ins[i];
		}
	} else {
		fft_remove_noise(ins, outs, frames, noise_min, noise_max, amount, &pFor, &pBak);
	}
}

Denoise::~Denoise() {
    FFTW(destroy_plan)(pForNoise);
    FFTW(destroy_plan)(pBak);
    FFTW(destroy_plan)(pFor);
    FFTW(cleanup)();
}

void Denoise::get_noise_sample(float* noisebuffer, fftw_real *noise_min, fftw_real *noise_max)
{
    int k;

    for(k = 0 ; k < FFT_SIZE ; k++) {
	noise_max[k] = 0.0;
	noise_min[k] = DBL_MAX;
    }

    for(k = 0 ; k < FFT_SIZE ; k++) {
        noise[k] = noisebuffer[k] * window_coef[k];
    }
    
    FFTW(execute)(pForNoise);
    
    /* convert noise sample to power spectrum */
    for(k = 1 ; k <= FFT_SIZE/2 ; k++) {
        double p2;
        if(k < FFT_SIZE/2) {
    	    p2 = noisefft[k] * noisefft[k] + noisefft[FFT_SIZE-k]*noisefft[FFT_SIZE-k];
        } else {
    	    /* Nyquist Frequency */
    	    p2 = noisefft[k] * noisefft[k];
        }
        noise_min[k] = MIN(noise_min[k], p2);
        noise_max[k] = MAX(noise_max[k], p2);
    }
}
