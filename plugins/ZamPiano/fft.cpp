#include <stdlib.h>
#include "fft.hpp"

// Constructor for FFT routine
fft::fft()
{
	fft(4096);
}

fft::fft(int nfft) {
	nfft = nfft;
	numfreqs = nfft/2 + 1;

	ffttime = (float*) fftwf_malloc(nfft* sizeof(float));
	cmplex = (fftwf_complex*) fftwf_malloc(numfreqs* sizeof(fftwf_complex));
	forwardplan=fftwf_plan_dft_r2c_1d(nfft,ffttime, cmplex, FFTW_ESTIMATE);
	reverseplan=fftwf_plan_dft_c2r_1d(nfft,cmplex,ffttime, FFTW_ESTIMATE);
}

// Destructor for FFT routine
fft::~fft()
{
	fftwf_destroy_plan(forwardplan);
	fftwf_destroy_plan(reverseplan);
	fftwf_free(ffttime);
	fftwf_free(cmplex);
}

// Perform forward FFT of real data
// Accepts:
//   fftvars - pointer to struct of FFT variables
//   input - pointer to an array of (real) input values, size nfft
//   output_re - pointer to an array of the real part of the output,
//     size nfft/2 + 1
//   output_im - pointer to an array of the imaginary part of the output,
//     size nfft/2 + 1
void fft::fft_forward()
{
	fftwf_execute(forwardplan);
}

// Perform inverse FFT, returning real data
// Accepts:
//   fftvars - pointer to struct of FFT variables
//   input_re - pointer to an array of the real part of the output,
//     size nfft/2 + 1
//   input_im - pointer to an array of the imaginary part of the output,
//     size nfft/2 + 1
//   output - pointer to an array of (real) input values, size nfft
void fft::fft_inverse()
{
	fftwf_execute(reverseplan);
}
