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

void Denoise::compute_bark_z(int rate)
{
    int k;

    /* compute the bark z value for this frequency bin */
    for(k = 1 ; k <= FFT_SIZE/2 ; k++) {
	double freq = (double)rate / 2.0 /(double)(FFT_SIZE/2)*(double)k;
	bark_z[k] = 7.0*log(freq/650.0 + sqrt(1 + (freq/650.0)*(freq/650.0)));
    }
}

void Denoise::compute_johnston_gain(double tonality_factor)
{
    int k;

    for (k = 1; k <= FFT_SIZE/2 ; ++k) {
	int j;

	for(j = k-1 ; j > 0 ; j--) {
	    double bark_diff = bark_z[k] - bark_z[j];

	    double johnston = 15.81 + 7.5*(bark_diff+.474) - 17.5*sqrt(1.0+(bark_diff+0.474)*(bark_diff+0.474));
	    double johnston_masked = johnston - (tonality_factor*(14.5+bark_z[j])+5.5*(1.0 - tonality_factor));
	    double gain = pow(10.0, johnston_masked/10.0);

	    jg_lower[k][k-j] = gain;

	    if(k - j > 10) break;
	}

	for(j = k ; j <= FFT_SIZE/2 ; j++) {
	    double bark_diff = bark_z[j] - bark_z[k];

	    double johnston = 15.81 + 7.5*(bark_diff+.474) - 17.5*sqrt(1.0+(bark_diff+0.474)*(bark_diff+0.474));
	    double johnston_masked = johnston - (tonality_factor*(14.5+bark_z[j])+5.5*(1.0 - tonality_factor));
	    double gain = pow(10.0, johnston_masked/10.0);

	    jg_upper[k][j-k] = gain;

	    if(j - k > 10) break;

	}
    }
}

int Denoise::get_window_delta()
{

    if(window_type == DENOISE_WINDOW_HANNING_OVERLAP_ADD) {
	return FFT_SIZE/2;
    } else {
	if(window_type == DENOISE_WINDOW_BLACKMAN)
	    return FFT_SIZE/smoothness;
	else
	    return 3*FFT_SIZE/4;
    }
}

void Denoise::compute_sum_window_wgts()
{
    int delta = get_window_delta();
    int i, k;

    for(i = 0 ; i < FFT_SIZE ; i++) {
	sum_window_wgts[i] = 0.0;
	for(k = i ; k < FFT_SIZE+i ; k += delta) {
	    sum_window_wgts[i] += window_coef[k%FFT_SIZE];
	}
    }
}

double Denoise::gain_weiner(double Yk2, double Dk2)
{
    double gain;
    double Xk2 = Yk2 - Dk2;

    if(Yk2 > Dk2)
	gain = (Xk2) / (Xk2+Dk2);
    else
	gain = 0.0;

    return gain;
}

double Denoise::gain_power_subtraction(double Yk2, double Dk2)
{
    double level = MAX(Yk2-Dk2, 0.0);

    if(Yk2 > DBL_MIN)
	return level/Yk2;
    else
	return 0.0;
}

double Denoise::alpha_lorber(double snr)
{
    double snr_db = 10.*log10(snr);
    double alpha;

    if(snr_db > 20) return 1.0;

    alpha = MIN((3.0 - 0.10*snr_db), 3.5);

    return alpha;
}

double Denoise::hypergeom(double theta)
{
    if(theta < 7.389056)
	return exp(-theta/2.0)*(1.0+theta*i0(theta/2.0)+theta*i1(theta/2.0));
    else
	return exp(0.09379 + 0.50447*log(theta));
}

double Denoise::gain_em(double Rprio, double Rpost, double alpha)
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

double Denoise::welty_alpha(double w, double x)
{
    double alpha = ( log(acos(-2.0*w+1)) - log(M_PI) ) / log(1.0 - x);
    return alpha;
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

double Denoise::db2w(double db)
{
    return pow(10,db/10);
}


void Denoise::fft_remove_noise(const float* ins, float* outs, uint32_t frames, fftw_real noise_min2[], fftw_real noise_max2[], fftw_real noise_avg2[], FFTW(plan) *pFor, FFTW(plan) *pBak)
{
    int k;
    uint32_t i;
    fftw_real noise2[DENOISE_MAX_FFT/2+1];
    fftw_real Y2[DENOISE_MAX_FFT/2+1];
    fftw_real masked[DENOISE_MAX_FFT/2+1];
    fftw_real gain_k[DENOISE_MAX_FFT];
    static fftw_real bsig_prev[DENOISE_MAX_FFT],bY2_prev[DENOISE_MAX_FFT/2+1],bgain_prev[DENOISE_MAX_FFT/2+1];
    fftw_real *sig_prev, *Y2_prev,*gain_prev;
    double SFM; 

    sig_prev = bsig_prev;
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
	double kinv = 1./(double)(FFT_SIZE/2.0);

	for (k = 1; k <= FFT_SIZE/2 ; ++k) {
	    //noise2[k] = noise_max2[k];
	    noise2[k] = noise_min2[k] + 0.5*(noise_max2[k] - noise_min2[k]);
	    //noise2[k] = noise_avg2[k];
	    if(k < FFT_SIZE/2) {
		Y2[k] = out[k]*out[k] + out[FFT_SIZE-k]*out[FFT_SIZE-k];
	    } else {
		Y2[k] = out[k]*out[k];
	    }
	    sum_log_p += log10(Y2[k]);
	    sum_p += Y2[k];
	}


	SFM = 10.0*( kinv*sum_log_p - log10(sum_p*kinv) );
	tonality_factor = MIN(SFM/-60.0, 1);
    }
/*
        for (k = 1; k <= FFT_SIZE/2 ; ++k) {
            int j ;

            masked[k] = 0.0 ;

            for(j = k-1 ; j > 0 ; j--) {
                double gain = jg_lower[k][k-j] ;
                if(k - j > 10) break ;

                masked[k] += MAX((Y2[j]-noise2[j]),0.0)*gain ;
            }

            for(j = k ; j <= FFT_SIZE/2 ; j++) {
                double gain = jg_upper[k][j-k] ;
                if(gain < 1.e-2) break ;
                if(j - k > 10) break ;
                masked[k] += MAX((Y2[j]-noise2[j]),0.0)*gain ;
            }
	}
*/

    for (k = 1; k <= FFT_SIZE/2 ; ++k) {
	if(noise2[k] > DBL_MIN) {
	    double gain, Fk, Gk;

		double Rpost = MAX(Y2[k]/noise2[k]-1.0, 0.0);
		double alpha = dn_gamma;
		double Rprio;

		if(prev_sample == 1)
		    Rprio = (1.0-alpha)*Rpost+alpha*gain_prev[k]*gain_prev[k]*Y2_prev[k]/noise2[k];
		else
		    Rprio = Rpost;

		gain = gain_em(Rprio, Rpost, alpha);
		gain_prev[k] = gain;
		Y2_prev[k] = Y2[k];

/*
                double Rpost = MAX(Y2[k]/noise2[k]-1.0, 0.0) ;
                double alpha = dn_gamma ;
                double Rprio ;

                if(prev_sample == 1)
                    Rprio = (1.0-alpha)*Rpost+alpha*gain_prev[k]*gain_prev[k]*Y2_prev[k]/noise2[k] ;
                else
                    Rprio = Rpost ;

                Y2_prev[k] = Y2[k] ;

                if(Y2[k] > masked[k]) {
                    gain = MAX(masked[k]/Y2[k], Rprio/(Rprio+1.0)) ;
                } else {
                    gain = 1.0 ;
                }

                gain_prev[k] = gain ;
*/
	    Fk = amount*(1.0-gain);

	    if(Fk < 0.0) Fk = 0.0;
	    if(Fk > 1.0) Fk = 1.0;

	    Gk =  1.0 - Fk;

	    out[k] *= Gk;
	    if(k < FFT_SIZE/2) out[FFT_SIZE-k] *= Gk;

	    gain_k[k] = Gk;
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
    amount = 0.3;
    smoothness = 11;
    randomness = 0.0;
    estimate_power_floor = 0;
    noisebufpos = 0;
    prev_sample = 0;
    
    pFor = FFTW(plan_r2r_1d)(FFT_SIZE, windowed, out, FFTW_R2HC, FFTW_ESTIMATE);
    pBak = FFTW(plan_r2r_1d)(FFT_SIZE, out, windowed, FFTW_HC2R, FFTW_ESTIMATE);

    pForLeft = FFTW(plan_r2r_1d)(FFT_SIZE, left, tmp, FFTW_R2HC, FFTW_ESTIMATE);

    window_type = DENOISE_WINDOW_BLACKMAN;

    for(k = 0; k < FFT_SIZE; k++) {
	window_coef[k] = fft_window(k,FFT_SIZE, window_type);
	left_prev_frame[k] = 0.0;
	left[k] = 0.0;
    }
}


void Denoise::process(const float* ins, float* outs, float* noisebuffer, uint32_t frames, int togglenoise) {

	if (togglenoise == 1) {
		uint32_t i;
		for (i = 0; i < frames; i++) {
			noisebuffer[noisebufpos] = ins[i];
			noisebufpos++;
			if (noisebufpos >= n_noise_samples)
				noisebufpos = 0;

			if (noisebufpos % (n_noise_samples/2) == 0) {
				get_noise_sample(noisebuffer, left_noise_min, left_noise_max, left_noise_avg);
			}
			outs[i] = ins[i];
		}
	} else {
		fft_remove_noise(ins, outs, frames, left_noise_min, left_noise_max, left_noise_avg, &pFor, &pBak);
	}
}

Denoise::~Denoise() {
    FFTW(destroy_plan)(pForLeft);
    FFTW(destroy_plan)(pBak);
    FFTW(destroy_plan)(pFor);
}

void Denoise::get_noise_sample(float* noisebuffer, fftw_real *left_noise_min, fftw_real *left_noise_max, fftw_real *left_noise_avg)
{
    int k;

    for(k = 0 ; k < FFT_SIZE ; k++) {
	window_coef[k] = 1.0;
	left_noise_max[k] = 0.0;
	left_noise_avg[k] = 0.0;
	left_noise_min[k] = DBL_MAX;
    }

	for(k = 0 ; k < FFT_SIZE ; k++) {
	    left[k] = noisebuffer[k]*window_coef[k];
	}

	FFTW(execute)(pForLeft);

	/* convert noise sample to power spectrum */
	for(k = 1 ; k <= FFT_SIZE/2 ; k++) {
	    double p2;
	    if(k < FFT_SIZE/2) {
		p2 = tmp[k] * tmp[k] + tmp[FFT_SIZE-k]*tmp[FFT_SIZE-k];
	    } else {
		/* Nyquist Frequency */
		p2 = tmp[k] * tmp[k];
	    }
	    left_noise_min[k] = MIN(left_noise_min[k], p2);
	    left_noise_max[k] = MAX(left_noise_max[k], p2);
	    left_noise_avg[k] += p2;
	}
    //}


    /* average out the power spectrum samples */
    for(k = 1 ; k <= FFT_SIZE/2 ; k++) {
	left_noise_avg[k] /= (double)n_noise_samples;
    }

    compute_bark_z(rate);
    compute_johnston_gain(tonality_factor);
/*
    if(freq_filter) {
	if(estimate_power_floor) {
	    double half_freq_w = (max_sample_freq - min_sample_freq)/2.0;
	    double sum_left = 0.0;
	    double n_left = 0.0;
	    for(k = 1 ; k <= FFT_SIZE/2 ; k++) {
		double freq = (double)rate / 2.0 /(double)(FFT_SIZE/2)*(double)k;
		if(freq < min_sample_freq && freq > min_sample_freq-half_freq_w) {
		    sum_left += left_noise_avg[k];
		    n_left += 1.0;
		}
		if(freq > max_sample_freq && freq < max_sample_freq+half_freq_w) {
		    sum_left += left_noise_avg[k];
		    n_left += 1.0;
		}
	    }
	    if(n_left > 1.e-30) sum_left /= n_left;
	    for(k = 1 ; k <= FFT_SIZE/2 ; k++) {
		double freq = (double)rate / 2.0 /(double)(FFT_SIZE/2)*(double)k;
		if(freq < min_sample_freq || freq > max_sample_freq) {
		    left_noise_avg[k] -= sum_left;
		}
	    }
	}
	for(k = 1 ; k <= FFT_SIZE/2 ; k++) {
	    double freq = (double)rate / 2.0 /(double)(FFT_SIZE/2)*(double)k;
	    if(freq < min_sample_freq || freq > max_sample_freq) {
		left_noise_avg[k] = 0.0;
	    }
	}
    }
//    audio_normalize(1);
*/
}
