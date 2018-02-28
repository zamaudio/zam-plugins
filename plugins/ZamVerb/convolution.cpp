/* LV2 convolution engine
 *
 * Copyright (C) 2012 Robin Gareus <robin@gareus.org>
 *
 * Modified for C++:
 * Copyright (C) 2017 Damien Zammit <damien@zamaudio.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/* Usage information:
 *
 * Non-realtime, initialization:
 *  1)  clv_alloc();
 *  2)  clv_configure(); // can be called multiple times
 *  3)  clv_initialize();   // fix settings
 *
 * Realtime process
 *  4)  convolve();
 *
 * Non-rt, cleanup
 *  5A) clv_release(); // -> goto (2) or (3)
 * OR
 *  5B) clv_free(); // -> The End
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>

#include <samplerate.h>
#include "convolution.hpp"
#include "ZamVerbImpulses.hpp"

#if ZITA_CONVOLVER_MAJOR_VERSION != 4
# error "This programs requires zita-convolver 4.x.x"
#endif

#ifndef SRC_QUALITY
# define SRC_QUALITY SRC_SINC_BEST_QUALITY
#endif

static pthread_mutex_t fftw_planner_lock = PTHREAD_MUTEX_INITIALIZER;

#define PRESETS_SAMPLERATE 48000
#define PRESETS_CH 4
int LV2convolv::resample_read_presets (const float *in, unsigned int in_frames, const int sample_rate, float **buf, unsigned int *n_ch, unsigned int *n_sp)
{
	float resample_ratio = 1.0;

	if (n_ch) *n_ch = PRESETS_CH;
	if (n_sp) *n_sp = in_frames;

	if (sample_rate != PRESETS_SAMPLERATE) {
		fprintf(stderr, "convolution: samplerate mismatch preset:%d host:%d\n", PRESETS_SAMPLERATE, sample_rate);
		resample_ratio = (float) sample_rate / (float) PRESETS_SAMPLERATE;
	}

	if (buf) {
		const size_t frames_in = PRESETS_CH * in_frames;
		const size_t frames_out = PRESETS_CH * ceil(in_frames * resample_ratio);
		*buf = (float*) malloc(frames_out*sizeof(float));
		
		float *iin;
		if (resample_ratio != 1.0) {
			iin = (float*)malloc(frames_in * sizeof(float));
			memcpy(iin, in, frames_in * sizeof(float));
		} else {
			memcpy(*buf, in, frames_in * sizeof(float));
		}

		if (!*buf) {
			fprintf (stderr, "convolution: memory allocation failed for IR audio-file buffer.\n");
			return (-2);
		}

		if (resample_ratio != 1.0) {
			VERBOSE_printf("convolution: resampling IR %ld -> %ld [frames * channels].\n",
					(long int) frames_in,
					(long int) frames_out);
			SRC_STATE* src_state = src_new(SRC_QUALITY, PRESETS_CH, NULL);
			SRC_DATA src_data;

			src_data.input_frames  = in_frames;
			src_data.output_frames = in_frames * resample_ratio;
			src_data.end_of_input  = 1;
			src_data.src_ratio     = resample_ratio;
			src_data.input_frames_used = 0;
			src_data.output_frames_gen = 0;
			src_data.data_in       = iin;
			src_data.data_out      = *buf;
			src_process(src_state, &src_data);
			VERBOSE_printf("convolution: resampled IR  %ld -> %ld [frames * channels].\n",
					src_data.input_frames_used * PRESETS_CH,
					src_data.output_frames_gen * PRESETS_CH);
			if (n_sp) *n_sp = (unsigned int) src_data.output_frames_gen;
			free(iin);
		}
	}
	return (0);
}

void LV2convolv::clv_alloc (void) {
	int i;
	convproc = NULL;
	for (i = 0; i < MAX_CHANNEL_MAPS; ++i) {
		ir_chan[i]  = i + 1;
		chn_inp[i]  = i + 1;
		chn_out[i]  = i + 1;
		ir_delay[i] = 0;
		ir_gain[i]  = 0.5f;
	}
	ir_fn = NULL;
	ir_preset = -1;
	density = 0.f;
	size = 0x00100000;
}

void LV2convolv::clv_release (void) {
	if (convproc) {
		convproc->stop_process ();
		delete convproc;
	}
	convproc = NULL;
}

void LV2convolv::clv_clone_settings(LV2convolv *clv_new) {
	memcpy (clv_new, this, sizeof(LV2convolv));
	clv_new->convproc = NULL;
	if (ir_fn) {
		clv_new->ir_fn = strdup (ir_fn);
	}
}

void LV2convolv::clv_free (void) {
	clv_release ();
	free (ir_fn);
}

int LV2convolv::clv_configure (const char *key, const char *value) {
	int n;
	if (strcasecmp (key, "convolution.ir.file") == 0) {
		free(ir_fn);
		ir_fn = strdup(value);
	} else if (strcasecmp (key, "convolution.ir.preset") == 0) {
		ir_preset = atoi(value);
	} else if (!strncasecmp (key, "convolution.out.source.", 23)) {
		if (sscanf (key, "convolution.source.%d", &n) == 1) {
			if ((0 < n) && (n <= MAX_CHANNEL_MAPS))
				chn_inp[n] = atoi(value);
		}
	} else if (!strncasecmp (key, "convolution.out.source.", 23)) {
		if (sscanf (key, "convolution.output.%d", &n) == 1) {
			if ((0 <= n) && (n < MAX_CHANNEL_MAPS))
				chn_out[n] = atoi(value);
		}
	} else if (!strncasecmp (key, "convolution.ir.channel.", 23)) {
		if (sscanf (key, "convolution.ir.channel.%d", &n) == 1) {
			if ((0 <= n) && (n < MAX_CHANNEL_MAPS))
				ir_chan[n] = atoi(value);
		}
	} else if (!strncasecmp (key, "convolution.ir.gain.", 20)) {
		if (sscanf (key, "convolution.ir.gain.%d", &n) == 1) {
			if ((0 <= n) && (n < MAX_CHANNEL_MAPS))
				ir_gain[n] = atof(value);
		}
	} else if (!strncasecmp (key, "convolution.ir.delay.", 21)) {
		if (sscanf (key, "convolution.ir.delay.%d", &n) == 1) {
			if ((0 <= n) && (n < MAX_CHANNEL_MAPS))
				ir_delay[n] = atoi(value);
		}
	} else if (strcasecmp (key, "convolution.maxsize") == 0) {
		size = atoi(value);
		if (size > 0x00400000) {
			size = 0x00400000;
		}
		if (size < 0x00001000) {
			size = 0x00001000;
		}
	} else {
		return 0;
	}
	return 1; // OK
}

char* LV2convolv::clv_dump_settings (void) {

#define MAX_CFG_SIZE ( MAX_CHANNEL_MAPS * 160 + 60 + (ir_fn ? strlen(ir_fn) : 0) )
	int i;
	size_t off = 0;
	char *rv = (char*) malloc (MAX_CFG_SIZE * sizeof (char));
#undef MAX_CFG_SIZE

	for (i = 0; i < MAX_CHANNEL_MAPS; ++i) {
		// f=12 ; d= 3 ; v=10
		off+= sprintf (rv + off, "convolution.ir.gain.%d=%e\n",    i, ir_gain[i]); // 22 + d + f
		off+= sprintf (rv + off, "convolution.ir.delay.%d=%d\n",   i, ir_delay[i]);// 23 + d + v
		off+= sprintf (rv + off, "convolution.ir.channel.%d=%d\n", i, ir_chan[i]); // 25 + d + d
		off+= sprintf (rv + off, "convolution.source.%d=%d\n",     i, chn_inp[i]); // 21 + d + d
		off+= sprintf (rv + off, "convolution.output.%d=%d\n",     i, chn_out[i]); // 21 + d + d
	}
	off+= sprintf(rv + off, "convolution.maxsize=%u\n", size);                         // 21 + v
	return rv;
}

int LV2convolv::clv_query_setting (const char *key, char *value, size_t val_max_len) {
	int rv = 0;
	if (!value || !key) {
		return -1;
	}

	if (strcasecmp (key, "convolution.ir.file") == 0) {
		if (ir_fn) {
			if (strlen(ir_fn) >= val_max_len) {
				rv = -1;
			}
			else {
				rv=snprintf(value, val_max_len, "%s", ir_fn);
			}
		}
	}
	// TODO allow querying other settings
	return rv;
}

int LV2convolv::clv_initialize (
		const unsigned int sample_rate,
		const unsigned int in_channel_cnt,
		const unsigned int out_channel_cnt,
		const unsigned int buffersize)
{
	unsigned int c;
	const unsigned int n_elem = in_channel_cnt * out_channel_cnt;

	/* zita-conv settings */
	const unsigned int options = 0;

	/* IR file */
	unsigned int n_chan = 0;
	unsigned int n_frames = 0;
	unsigned int max_size = 0;

	/* IR presets */
	struct pst {
		const float *data;
		unsigned int size;
	};

	const struct pst preset[7] = {
		{ ZamVerbImpulses::studioA, ZamVerbImpulses::studioAsamples },
		{ ZamVerbImpulses::studioB, ZamVerbImpulses::studioBsamples },
		{ ZamVerbImpulses::roomA, ZamVerbImpulses::roomAsamples },
		{ ZamVerbImpulses::roomB, ZamVerbImpulses::roomBsamples },
		{ ZamVerbImpulses::hallA, ZamVerbImpulses::hallAsamples },
		{ ZamVerbImpulses::hallB, ZamVerbImpulses::hallBsamples },
		{ ZamVerbImpulses::plate, ZamVerbImpulses::platesamples },
	};

	float *p = NULL;  /* temp. IR file buffer */
	float *gb = NULL; /* temp. gain-scaled IR file buffer */

	fragment_size = buffersize;

	if (zita_convolver_major_version () != ZITA_CONVOLVER_MAJOR_VERSION) {
		fprintf (stderr, "convolution: Zita-convolver version does not match.\n");
		return -1;
	}

	if (convproc) {
		fprintf (stderr, "convolution: already initialized.\n");
		return (-1);
	}

	if (!ir_fn && ir_preset < 0) {
		fprintf (stderr, "convolution: No IR file was configured.\n");
		return -1;
	}

	pthread_mutex_lock(&fftw_planner_lock);

	convproc = new Convproc();
	convproc->set_options (options);
/*
	if (audiofile_read (ir_fn, sample_rate, &p, &n_chan, &n_frames)) {
		fprintf(stderr, "convolution: failed to read IR.\n");
		goto errout;
	}
*/
	const float *impulse = preset[ir_preset].data;
	unsigned int impulse_frames = preset[ir_preset].size;

	if (resample_read_presets (impulse, impulse_frames, sample_rate, &p, &n_chan, &n_frames)) {
		fprintf(stderr, "convolution: failed to read IR preset.\n");
		goto errout;
	}

	if (n_frames == 0 || n_chan == 0) {
		fprintf(stderr, "convolution: invalid IR file.\n");
		goto errout;
	}

	for (c = 0; c < MAX_CHANNEL_MAPS; c++) {
		// TODO only relevant channels
		if (ir_delay[c] > max_size) {
			max_size = ir_delay[c];
		}
	}

	max_size += n_frames;

	if (max_size > size) {
		max_size = size;
	}

	VERBOSE_printf("convolution: max-convolution length %d samples (limit %d), period: %d samples\n", max_size, size, buffersize);


	if (convproc->configure (
			/*in*/  in_channel_cnt,
			/*out*/ out_channel_cnt,
			/*max-convolution length */ max_size,
			/*quantum*/  buffersize,
			/*min-part*/ buffersize,
			/*max-part*/ buffersize,
			density
			)) {
		fprintf (stderr, "convolution: Cannot initialize convolution engine.\n");
		goto errout;
	}

	gb = (float*) malloc (n_frames * sizeof(float));
	if (!gb) {
		fprintf (stderr, "convolution: memory allocation failed for convolution buffer.\n");
		goto errout;
	}

	VERBOSE_printf("convolution: Proc: in: %d, out: %d || IR-file: %d chn, %d samples\n",
			in_channel_cnt, out_channel_cnt, n_chan, n_frames);

	// TODO use pre-configured channel-map (from state), IFF set and valid for the current file

	// reset channel map
	for (c = 0; c < MAX_CHANNEL_MAPS; ++c) {
		ir_chan[c] = 0;
		chn_inp[c] = 0;
		chn_out[c] = 0;
	}

	// follow channel map conventions
	if (n_elem == n_chan) {
		// exact match: for every input-channel, iterate over all outputs
		// eg.  1: L -> L , 2: L -> R, 3: R -> L, 4: R -> R
		for (c = 0; c < n_chan && c < MAX_CHANNEL_MAPS; ++c) {
			ir_chan[c] = 1 + c;
			chn_inp[c] = 1 + ((c / out_channel_cnt) % in_channel_cnt);
			chn_out[c] = 1 +  (c % out_channel_cnt);
		}
	}
	else if (n_elem > n_chan) {
		VERBOSE_printf("convolution: IR file has too few channels for given processor config.\n");
		// missing some channels, first assign  in -> out, then x-over
		// eg.  1: L -> L , 2: R -> R,  3: L -> R,  4: R -> L
		// this allows to e.g load a 2-channel (stereo) IR into a
		// 2x2 true-stereo effect instance
		for (c = 0; c < n_chan && c < MAX_CHANNEL_MAPS; ++c) {
			ir_chan[c] = 1 + c;
			chn_inp[c] = 1 + (c % in_channel_cnt);
			chn_out[c] = 1 + (((c + c / in_channel_cnt) % in_channel_cnt) % out_channel_cnt);
		}
		// assign mono input to 1: L -> L , 2: R -> R,
		for (;n_chan == 1 && c < 2; ++c) {
			ir_chan[c] = 1;
			chn_inp[c] = 1 + (c % in_channel_cnt);
			chn_out[c] = 1 + (c % out_channel_cnt);
		}
	}
	else {
		assert (n_elem < n_chan);
		VERBOSE_printf("convolution: IR file has too many channels for given processor config.\n");
		// allow loading a quad file to a mono-in stereo-out
		// eg.  1: L -> L , 2: L -> R
		for (c = 0; c < n_elem && c < MAX_CHANNEL_MAPS; ++c) {
			ir_chan[c] = 1 + c;
			chn_inp[c] = 1 + ((c / out_channel_cnt) % in_channel_cnt);
			chn_out[c] = 1 +  (c % out_channel_cnt);
		}
	}

	// assign channel map to convolution engine
	for (c = 0; c < MAX_CHANNEL_MAPS; ++c) {
		unsigned int i;
		if (chn_inp[c] == 0 || chn_out[c] == 0 || ir_chan[c] == 0) {
			continue;
		}

		assert (ir_chan[c] <= n_chan);

		for (i = 0; i < n_frames; ++i) {
			// decode interleaved channels, apply gain scaling
			gb[i] = p[i * n_chan + ir_chan[c] - 1] * ir_gain[c];
		}

		VERBOSE_printf ("convolution: SET in %d -> out %d [IR chn:%d gain:%+.3f dly:%d]\n",
				chn_inp[c],
				chn_out[c],
				ir_chan[c],
				ir_gain[c],
				ir_delay[c]
			       );

		convproc->impdata_create (
				chn_inp[c] - 1,
				chn_out[c] - 1,
				1, gb, ir_delay[c], ir_delay[c] + n_frames);
	}

	free(gb); gb = NULL;
	free(p);  p  = NULL;

#if 0 // INFO
	convproc->print (stderr);
#endif

	if (convproc->start_process (0, 0)) {
		fprintf(stderr, "convolution: Cannot start processing.\n");
		goto errout;
	}

	pthread_mutex_unlock(&fftw_planner_lock);
	return 0;

errout:
	free(gb);
	free(p);
	delete(convproc);
	convproc = NULL;
	pthread_mutex_unlock(&fftw_planner_lock);
	return -1;
}

int LV2convolv::clv_is_active (void) {
	if (!convproc || !ir_fn) {
		return 0;
	}
	return 1;
}

void LV2convolv::silent_output(float * const * outbuf, size_t n_channels, size_t n_samples) {
	unsigned int c;
	for (c = 0; c < n_channels; ++c) {
		memset (outbuf[c], 0, n_samples * sizeof(float));
	}
}

int LV2convolv::clv_convolve (
		const float * const * inbuf,
		float * const * outbuf,
		const unsigned int in_channel_cnt,
		const unsigned int out_channel_cnt,
		const unsigned int n_samples,
		const float output_gain)
{
	unsigned int c;

	if (!convproc) {
		silent_output(outbuf, out_channel_cnt, n_samples);
		return (0);
	}

	if (convproc->state () == Convproc::ST_WAIT) {
		convproc->check_stop ();
	}

	if (fragment_size != n_samples) {
		silent_output(outbuf, out_channel_cnt, n_samples);
		return -1;
	}

#if 1
	if (convproc->state () != Convproc::ST_PROC) {
		/* This cannot happen in sync-mode, but... */
		assert (0);
		silent_output(outbuf, out_channel_cnt, n_samples);
		return (n_samples);
	}
#endif

	for (c = 0; c < in_channel_cnt; ++c)
#if 0 // no denormal protection
		memcpy (clv->convproc->inpdata (c), inbuf[c], n_samples * sizeof (float));
#else // prevent denormals
	{
		unsigned int i;
		float *id = convproc->inpdata(c);
		for (i = 0; i < n_samples; ++i) {
			id[i] = inbuf[c][i] + 1e-20f;
		}
	}
#endif

	int f = convproc->process (false);

	if (f /*&Convproc::FL_LOAD)*/ ) {
		/* Note this will actually never happen in sync-mode */
		assert (0);
		silent_output(outbuf, out_channel_cnt, n_samples);
		return (n_samples);
	}

	for (c = 0; c < out_channel_cnt; ++c) {
		if (output_gain == 1.0) {
			memcpy (outbuf[c], convproc->outdata (c), n_samples * sizeof (float));
		} else {
			unsigned int s;
			float const * const od = convproc->outdata (c);
			for (s = 0; s < n_samples; ++s) {
				outbuf[c][s] = od[s] * output_gain;
			}
		}
	}

	return (n_samples);
}

LV2convolv::LV2convolv()
{
	clv_alloc ();
}

LV2convolv::~LV2convolv()
{
	clv_free ();
}
