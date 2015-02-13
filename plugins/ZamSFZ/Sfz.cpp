#include "Sfz.hpp"
#include <iostream>
#include <fstream>
#include <string>
#define	BLOCK_SIZE 512

void Sfz::readsamples(SNDFILE *infile, 
				int channels, int note, int layer)
{
	float buf[channels*BLOCK_SIZE];
	int k, m, readcount, i;

	i = 0;
	while (i < MAX_SAMPLES && (readcount = sf_readf_float (infile, buf, BLOCK_SIZE)) > 0) {	
		for (k = 0 ; k < readcount ; k++) {	
			for (m = 0 ; m < channels ; m++) {
				sample[note][layer][m][i+k] = buf[k*channels + m];
			}
		}
		i += readcount;
	}
}

Sfz::Sfz()
{
}

void Sfz::clearsamples()
{
	int i,j,k;
	for (i = 0; i < 128; i++) {
		for (j = 0; j < MAX_LAYERS; j++) {
			for (k = 0; k < MAX_SAMPLES; k++) {
				sample[i][j][0][k] = 0.f;
				sample[i][j][1][k] = 0.f;
			}
		}
	}
}

void Sfz::loadsamples(std::string path, std::string filename)
{
	int note, layer, i;
	::sfz::File* sfzfile = NULL;
	::sfz::Instrument* sfzinstrument = NULL;
	sfzfile = new ::sfz::File(filename, path);
	sfzinstrument = sfzfile->GetInstrument();

	SNDFILE *infile = NULL;
	SF_INFO sfinfo;

	int maxregions = sfzinstrument->regions.size();
	layer = 0;
  if (maxregions == 0) {
	printf("No samples found\n");
  } else {
	for (i = 0; i < maxregions; i++) {
		for (note = 0; note < 128; note++) {
			if (note >= sfzinstrument->regions[i]->lokey &&
				note <= sfzinstrument->regions[i]->hikey) {
			
				infile = NULL;
				if ((infile = sf_open(sfzinstrument->regions[i]->sample.c_str(), SFM_READ, &sfinfo)) == NULL) {
						printf("Missing samples\n");
						puts (sf_strerror (NULL));
				}
				readsamples (infile, sfinfo.channels, note, layer);
				sf_close (infile);
				//printf("N-%d V-%d %s\n", note, layer, sfzinstrument->regions[i]->sample.c_str());
				layer++;
				if (sfzinstrument->regions[i]->hivel >= 127) {
					maxlayers[note] = layer;
					layer = 0;
				}
				break;
			}
		}
	}
	printf("All samples loaded, Woot!\n");
  }
  delete sfzfile;
}
