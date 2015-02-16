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
	int note, i, key;
	::sfz::File* sfzfile = NULL;
	::sfz::Instrument* sfzinstrument = NULL;
	sfzfile = new ::sfz::File(filename, path);
	sfzinstrument = sfzfile->GetInstrument();

	SNDFILE *infile = NULL;
	SF_INFO sfinfo;

	int maxregions = sfzinstrument->regions.size();
  if (maxregions == 0) {
	printf("No samples found\n");
  } else {
	for (i = 0; i < maxregions; i++) {
		for (note = 0; note < 128; note++) {
			if (sfzinstrument->regions[i]->lokey == sfzinstrument->regions[i]->hikey) {
				key = sfzinstrument->regions[i]->lokey;
			} else {
				key = sfzinstrument->regions[i]->pitch_keycenter;
			}
			if (note == key) {
				layers[note].keymiddle = key;
				layers[note].dsemitones = 0;
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
				continue;
			} else if (sfzinstrument->regions[i]->lokey <= note && sfzinstrument->regions[i]->hikey >= note) {
				layers[note].keymiddle = key;
				layers[note].dsemitones = note - key;
				//printf("MainKey=%d NoteShiftTo=%d\n", key, note);
			}
		}
	}
	printf("All samples loaded, Woot!\n");
  }
  delete sfzfile;
}

void Sfz::pitchshiftsamples(int srate)
{
	::RubberBand::RubberBandStretcher* rbl = NULL;
	::RubberBand::RubberBandStretcher* rbr = NULL;
	int i,j;
	for (i = 0; i < 128; i++) {
		//printf("i=%d ds=%d\n", i, layers[i].dsemitones);
		if (!(layers[i].dsemitones == 0)) {
			int ii = layers[i].keymiddle;
			//printf("Pitch shifting... %d\n", layers[i].dsemitones);
			for (j = 0; j < layers[ii].max; j++) {
				float const * const inl[] = {sample[ii][j][0]};
				float const * const inr[] = {sample[ii][j][1]};
				float * const outl[] = {sample[i][j][0]};
				float * const outr[] = {sample[i][j][1]};
				rbl = new ::RubberBand::RubberBandStretcher(srate, 1, 0, 1.0, pow(2.0, layers[i].dsemitones / 12.));
				rbr = new ::RubberBand::RubberBandStretcher(srate, 1, 0, 1.0, pow(2.0, layers[i].dsemitones / 12.));

				rbl->setMaxProcessSize(MAX_SAMPLES);
				rbr->setMaxProcessSize(MAX_SAMPLES);
				rbl->setExpectedInputDuration(MAX_SAMPLES);
				rbr->setExpectedInputDuration(MAX_SAMPLES);
				rbl->process(inl, MAX_SAMPLES, true);
				rbr->process(inr, MAX_SAMPLES, true);
				rbl->retrieve(outl, MAX_SAMPLES);
				rbr->retrieve(outr, MAX_SAMPLES);
				delete rbl;
				delete rbr;
			}
		}
	}
}
