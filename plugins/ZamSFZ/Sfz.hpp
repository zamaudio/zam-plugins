#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libsfz/sfz.h"
#include <rubberband/RubberBandStretcher.h>
#include <sndfile.h>
#include <samplerate.h>
#include <math.h>
#define MAX_LAYERS 12
#define MAX_SAMPLES 130000
#define	BLOCK_SIZE 512
#define CONVERTER_TYPE SRC_SINC_BEST_QUALITY

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
	void loadsamples(std::string path, std::string filename, int target_rate);
	void readsamples (SNDFILE *infile, int channels, int note, int layer);
	void readsamples_resample (SNDFILE *infile, SF_INFO *sfinfo, int note, int layer, int target_rate);
	void pitchshiftsamples(int srate);
	void remapvelocityranges(::sfz::Instrument *inst);
};
