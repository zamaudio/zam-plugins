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

#ifndef CONVOLUTION_H_
#define CONVOLUTION_H_

#include "../../lib/zita-convolver-3.1.0/zita-convolver.h"

#define MAX_CHANNEL_MAPS (4)
#define VERBOSE_printf(x, ...)

class LV2convolv {
public:
	Convproc *convproc;

	char *ir_fn;
	int ir_preset;
	int ir_presetx;
	int ir_presety;
	unsigned int chn_inp[MAX_CHANNEL_MAPS];
	unsigned int chn_out[MAX_CHANNEL_MAPS];
	unsigned int ir_chan[MAX_CHANNEL_MAPS];
	unsigned int ir_delay[MAX_CHANNEL_MAPS];
	float ir_gain[MAX_CHANNEL_MAPS];

	/* convolution settings*/
	unsigned int size;
	float density;

	/* process settings */
	unsigned int fragment_size;

	/* static methods */
	static int resample_read_presets (const float *in, unsigned int in_frames, const int sample_rate, float **buf, unsigned int *n_ch, unsigned int *n_sp);
	static void silent_output(float * const * outbuf, size_t n_channels, size_t n_samples);

	/* convolution specific methods */
	LV2convolv();
	~LV2convolv();
	void clv_alloc(void);
	void clv_release (void);
	void clv_clone_settings(LV2convolv *clv_new);
	void clv_free (void);
	int clv_configure (const char *key, const char *value);
	int clv_configure (const char *key, const char *elev, const char* azim);
	int clv_initialize (
		const unsigned int sample_rate,
		const unsigned int in_channel_cnt,
		const unsigned int out_channel_cnt,
		const unsigned int buffersize
	);
	int clv_is_active (void);
	int clv_convolve (
		const float * const * inbuf,
		float * const * outbuf,
		const unsigned int in_channel_cnt,
		const unsigned int out_channel_cnt,
		const unsigned int n_samples,
		const float output_gain
	);
};

#endif
