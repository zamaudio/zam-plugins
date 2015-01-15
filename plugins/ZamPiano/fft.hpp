#ifndef FFT_H 
#define FFT_H

#include <fftw3.h>

class fft {
public:

  int nfft;        // size of FFT
  int numfreqs;    // number of frequencies represented (nfft/2 + 1)
  float* ffttime;
  fftwf_complex* cmplex;

  fftwf_plan forwardplan;
  fftwf_plan reverseplan;

  fft();
  fft(int nfft);
  ~fft();

  void fft_forward();
  void fft_inverse();
};
#endif
