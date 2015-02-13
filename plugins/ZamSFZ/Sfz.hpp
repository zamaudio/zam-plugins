#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libsfz/sfz.h"
#include <sndfile.h>
#define MAX_LAYERS 8
#define MAX_SAMPLES 64000

class Sfz {
	
public:
	Sfz();
	float sample[128][MAX_LAYERS][2][MAX_SAMPLES];
	int maxlayers[128];
	void clearsamples();
	void loadsamples(std::string path, std::string filename);
	void readsamples (SNDFILE *infile, int channels, int note, int layer);
};
