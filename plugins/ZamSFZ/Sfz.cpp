#include "Sfz.hpp"
#include <iostream>
#include <fstream>
#include <string>

void Sfz::remapvelocityranges(::sfz::Instrument *inst)
{
	int i, k, r, key, oldkey, l, lowest, idx;
	::sfz::SFZRegion *reg;
	std::vector< ::sfz::SFZRegion > tmp;
	std::vector< ::sfz::SFZRegion* > remap;
	int layer[128] = {0};

	if (!inst)
		return;

	for (i = 0; i < inst->regions.size(); i++) {
		if (inst->regions[i]->lokey == inst->regions[i]->hikey) {
			key = inst->regions[i]->lokey;
		} else {
			key = inst->regions[i]->pitch_keycenter;
		}
		layer[key]++;
	}

	for (key = 0; key < 128; key++) {
		lowest = 2;
		idx = -1;
		for (i = 0; i < layer[key]; i++) {
			// find next region with correct key
			for (r = idx+1; r < inst->regions.size(); r++) {
				if (inst->regions[r]->lokey == inst->regions[r]->hikey) {
					k = inst->regions[r]->lokey;
				} else {
					k = inst->regions[r]->pitch_keycenter;
				}
				if (k != key)
					continue;

				if (inst->regions[r]->lovel <= lowest) {
					idx = r;
					lowest = inst->regions[r]->hivel + 2;
					break;
				}
			}
			if (idx >= 0) {
				// push found region to new vector
				tmp.push_back(*(inst->regions[idx]));
			}
		}
	}

	oldkey = -1;
	for (i = 0; i < tmp.size(); i++) {
		if (tmp[i].lokey == tmp[i].hikey) {
			key = tmp[i].lokey;
		} else {
			key = tmp[i].pitch_keycenter;
		}
		if (key != oldkey) {
			if (layer[key] > MAX_LAYERS) {
				// Remap velocities
				// Choose staggered layers
				int chosen, lo, hi;
				lo = 0;
				hi = 0;
				for (k = 1; k <= MAX_LAYERS; k++) {
					chosen = k * layer[key] / MAX_LAYERS;
					lo = hi + 1;
					hi = k * 127 / MAX_LAYERS;
					reg = new ::sfz::SFZRegion(inst);

					tmp[(chosen - 1) + i].lovel = lo;
					tmp[(chosen - 1) + i].hivel = hi;
					*reg = tmp[(chosen - 1) + i];
					remap.push_back(reg);
				}
			} else {
				// 1:1 mapping
				for (k = 0; k < layer[key]; k++) {
					reg = new ::sfz::SFZRegion(inst);
					*reg = tmp[k + i];
					remap.push_back(reg);
				}
			}
		}
		oldkey = key;
	}

	inst->regions.clear();
	for (i = 0; i < remap.size(); i++) {
		inst->regions.push_back(remap[i]);
		//printf("k:%d (%d, %d) %s\n", remap[i]->lokey, remap[i]->lovel, remap[i]->hivel, remap[i]->sample.c_str());
	}
}

void Sfz::readsamples_resample(SNDFILE *infile, SF_INFO *sfinfo, int note, int layer, int target_rate)
{
	float buf[sfinfo->channels * BLOCK_SIZE];
	float tmpbuf_pre[2][MAX_SAMPLES] = {0.f};
	float tmpbuf_post[2][MAX_SAMPLES] = {0.f};
	SRC_DATA src;
	int k, m, readcount, i;
	int maxch = std::min(sfinfo->channels, 2);

	// Read samples in blocks to a flat buffer
	i = 0;
	while (i < MAX_SAMPLES && (readcount = sf_readf_float (infile, buf, BLOCK_SIZE)) > 0) {
		for (k = 0 ; k < readcount ; k++) {
			for (m = 0; m < maxch; m++) {
				tmpbuf_pre[m][i+k] = buf[k*sfinfo->channels + m];
			}
		}
		i += readcount;
	}

	// Resample...
	src.data_in = &tmpbuf_pre[0][0];
	src.data_out = &tmpbuf_post[0][0];
	src.input_frames = MAX_SAMPLES;
	src.output_frames = MAX_SAMPLES;
	src.src_ratio = (double)target_rate / (double)sfinfo->samplerate;
	if (!src_simple(&src, CONVERTER_TYPE, 1)) {
		// Write left samples out to sample buffer
		for (i = 0; i < src.output_frames_gen; i++) {
			sample[note][layer][0][i] = tmpbuf_post[0][i];
		}
	}

	if (maxch == 2) {
		src.data_in = &tmpbuf_pre[1][0];
		src.data_out = &tmpbuf_post[1][0];
		src.input_frames = MAX_SAMPLES;
		src.output_frames = MAX_SAMPLES;
		src.src_ratio = (double)target_rate / (double)sfinfo->samplerate;
		if (!src_simple(&src, CONVERTER_TYPE, 1)) {
			// Write right samples out to sample buffer
			for (i = 0; i < src.output_frames_gen; i++) {
				sample[note][layer][1][i] = tmpbuf_post[1][i];
			}
		}
	}
}

void Sfz::readsamples(SNDFILE *infile, int channels, int note, int layer)
{
	float buf[channels*BLOCK_SIZE];
	int k, m, readcount, i;

	i = 0;
	while (i < MAX_SAMPLES && (readcount = sf_readf_float (infile, buf, BLOCK_SIZE)) > 0) {	
		for (k = 0 ; k < readcount ; k++) {	
			for (m = 0; m < std::min(channels, 2); m++) {
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

void Sfz::loadsamples(std::string path, std::string filename, int target_rate)
{
	int note, i, j, k, key;
	::sfz::SFZParser sfzfile;
	std::string fullsfzpath = path + std::string("/") + filename;
	if (sfzfile.readsfz(fullsfzpath) == -1) {
		printf("Can't open SFZ\n");
		return;
	}
	::sfz::Instrument* sfzinstrument = &sfzfile.instrument;

	remapvelocityranges(sfzinstrument);

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
				std::string fullsamplepath = path + std::string("/") + sfzinstrument->regions[i]->sample;
				if ((infile = sf_open(fullsamplepath.c_str(), SFM_READ, &sfinfo)) == NULL) {
						printf("Missing samples\n");
						puts (sf_strerror (NULL));
						printf("File: %s\n",fullsamplepath.c_str());
				}
				k = layers[note].max;
				if ((int)sfinfo.samplerate == target_rate) {
					readsamples (infile, sfinfo.channels, note, k);
				} else {
					readsamples_resample (infile, &sfinfo, note, k, target_rate);
				}
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
  for (i = 0; i < 128; i++) {
  	if (!(layers[i].keymiddle == i)) {
		k = layers[i].keymiddle;
		layers[i].max = layers[k].max;
		for (j = 0; j < layers[i].max; j++) {
			layers[i].l[j].lovel = layers[k].l[j].lovel;
			layers[i].l[j].hivel = layers[k].l[j].hivel;
		}
	}
  }
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
			printf("Pitch shifting... %d\n", layers[i].dsemitones);
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
