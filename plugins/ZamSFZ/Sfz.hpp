#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libsfz/sfz.h"
#include <rubberband/RubberBandStretcher.h>
#include <sndfile.h>
#include <math.h>
#define MAX_LAYERS 25
#define MAX_SAMPLES 64000

class Sfz {
	
public:
	Sfz();
	float sample[128][MAX_LAYERS][2][MAX_SAMPLES];
	typedef struct {
		uint8_t lovel;
		uint8_t hivel;
		uint8_t lokey;
		uint8_t hikey;
	} hilo_t;

	typedef struct {
		int max;
		int keymiddle;
		int dsemitones;
		hilo_t l[MAX_LAYERS];
	} layer_t;

	layer_t layers[128];
	void clearsamples();
	void loadsamples(std::string path, std::string filename);
	void readsamples (SNDFILE *infile, int channels, int note, int layer);
	void pitchshiftsamples(int srate);
};
