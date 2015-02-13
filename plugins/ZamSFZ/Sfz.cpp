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
	for (i = 0; i < 128; i++) {
		layers[i].max = 0;
		for (j = 0; j < MAX_LAYERS; j++) {
			layers[i].l[j].lovel = 0;
			layers[i].l[j].hivel = 0;
			layers[i].l[j].lokey = 0;
			layers[i].l[j].hikey = 0;
		}
	}
}

void Sfz::loadsamples(std::string path, std::string filename)
{
	int note, layer, i, key;
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
			key = 0;
			if (sfzinstrument->regions[i]->lokey == sfzinstrument->regions[i]->hikey) {
				key = sfzinstrument->regions[i]->lokey;
			} else {
				key = sfzinstrument->regions[i]->pitch_keycenter;
			}
			if (note == key) {
				infile = NULL;
				if ((infile = sf_open(sfzinstrument->regions[i]->sample.c_str(), SFM_READ, &sfinfo)) == NULL) {
						printf("Missing samples\n");
						puts (sf_strerror (NULL));
						printf("File: %s\n",sfzinstrument->regions[i]->sample.c_str());
				}
				readsamples (infile, sfinfo.channels, note, layers[note].max);
				int k = layers[note].max;
				layers[note].l[k].lovel = sfzinstrument->regions[i]->lovel;
				layers[note].l[k].hivel = sfzinstrument->regions[i]->hivel;
				layers[note].l[k].lokey = sfzinstrument->regions[i]->lokey;
				layers[note].l[k].hikey = sfzinstrument->regions[i]->hikey;
				sf_close (infile);
				printf("N-%d V-%d %s\n", note, k, sfzinstrument->regions[i]->sample.c_str());
				layers[note].max++;
				break;
			}
		}
	}
	printf("All samples loaded, Woot!\n");
  }
  delete sfzfile;
  for (i = 0; i < 128; i++) {
	if (layers[i].max) {
		layers[i].max--;
	}
  }
}
