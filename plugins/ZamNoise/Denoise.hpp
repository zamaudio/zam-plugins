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

#include <float.h>
#include <math.h>
#include <fftw3.h>
#include <inttypes.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

#define FFTW(func) fftwf_ ## func
#define DENOISE_MAX_FFT 8192
#define DENOISE_WINDOW_BLACKMAN 0
#define DENOISE_WINDOW_BLACKMAN_HYBRID 1
#define DENOISE_WINDOW_HANNING_OVERLAP_ADD 2
#define bin2freq(r,s,k) ((double)r / 2.0 /(double)(s/2)*(double)k)

typedef float fftw_real;

class Denoise {
public:
	Denoise(float srate);
	~Denoise();
	void process(const float* ins, float* outs, float* noisebuffer, uint32_t frames, int noisetoggle, float amount);
	double hypergeom(double theta);
	double gain_em(double Rprio, double Rpost);
	double blackman(int k, int N);
	double hanning(int k, int N);
	double blackman_hybrid(int k, int n_flat, int N);
	double fft_window(int k, int N, int window_type);
	void fft_remove_noise(const float* ins, float* outs, uint32_t frames, fftw_real noise_min2[], fftw_real noise_max2[], float amount, FFTW(plan) *pFor, FFTW(plan) *pBak);
	int denoise(long noise_start, long noise_end, long first_sample, long last_sample);
	void get_noise_sample(float* noisebuffer, fftw_real *noise_min, fftw_real *noise_max);

private:
	int window_type;
	int FFT_SIZE;
	int n_noise_samples;
	double dn_gamma;
	double min_sample_freq;
	double max_sample_freq;
	int rate;
	double window_coef[DENOISE_MAX_FFT];
	int noisebufpos;

	int prev_sample;

	fftw_real windowed[DENOISE_MAX_FFT];
	fftw_real out[DENOISE_MAX_FFT];

    fftw_real noise[DENOISE_MAX_FFT];
    fftw_real sample[DENOISE_MAX_FFT];
    fftw_real noise_max[DENOISE_MAX_FFT];
    fftw_real noise_min[DENOISE_MAX_FFT];
    fftw_real noisefft[DENOISE_MAX_FFT];
    fftw_real prev_frame[DENOISE_MAX_FFT];
    FFTW(plan) pForNoise;
    FFTW(plan) pFor, pBak;
    double chbevl( double x, double array[], int n );
    double i0(double x);
    double i1(double x);

};
