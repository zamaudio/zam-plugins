/*
 * ZamHeadX2 Stereo widener
 * Copyright (C) 2014  Damien Zammit <damien@zamaudio.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the doc/GPL.txt file.
 */

#include "ZamHeadX2Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamHeadX2Plugin::ZamHeadX2Plugin()
    : Plugin(paramCount, 1, 0)
{
    // set default values
    loadProgram(0);
}

ZamHeadX2Plugin::~ZamHeadX2Plugin()
{
}

// -----------------------------------------------------------------------
// Init

void ZamHeadX2Plugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramAzimuth:
        parameter.hints      = kParameterIsAutomable | kParameterIsInteger;
        parameter.name       = "Azimuth";
        parameter.symbol     = "az";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -90.0f;
        parameter.ranges.max = 270.0f;
        break;
    case paramElevation:
        parameter.hints      = kParameterIsAutomable | kParameterIsInteger;
        parameter.name       = "Elevation";
        parameter.symbol     = "elev";
        parameter.unit       = " ";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 90.0f;
        break;
    case paramWidth:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Width";
        parameter.symbol     = "width";
        parameter.unit       = " ";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 2.5f;
        break;
    }
}


void ZamHeadX2Plugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "Zero";
		break;
	}
}

void ZamHeadX2Plugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		azimuth = 0.0;
		elevation = 0.0;
		width = 1.0;
		break;
	}

	activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZamHeadX2Plugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramAzimuth:
        return azimuth;
        break;
    case paramElevation:
        return elevation;
        break;
    case paramWidth:
        return width;
        break;
    }
    return 0.f;
}

void ZamHeadX2Plugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramAzimuth:
        azimuth = value;
        break;
    case paramElevation:
        elevation = value;
        break;
    case paramWidth:
        width = value;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

static const float fir_left[50][25][200] = {
	{
	#include "l1"
	},
	{
	#include "l2"
	},
	{
	#include "l3"
	},
	{
	#include "l4"
	},
	{
	#include "l5"
	},
	{
	#include "l6"
	},
	{
	#include "l7"
	},
	{
	#include "l8"
	},
	{
	#include "l9"
	},
	{
	#include "l10"
	},
	{
	#include "l11"
	},
	{
	#include "l12"
	},
	{
	#include "l13"
	},
	{
	#include "l14"
	},
	{
	#include "l15"
	},
	{
	#include "l16"
	},
	{
	#include "l17"
	},
	{
	#include "l18"
	},
	{
	#include "l19"
	},
	{
	#include "l20"
	},
	{
	#include "l21"
	},
	{
	#include "l22"
	},
	{
	#include "l23"
	},
	{
	#include "l24"
	},
	{
	#include "l25"
	},
	{
	#include "l26"
	},
	{
	#include "l27"
	},
	{
	#include "l28"
	},
	{
	#include "l29"
	},
	{
	#include "l30"
	},
	{
	#include "l31"
	},
	{
	#include "l32"
	},
	{
	#include "l33"
	},
	{
	#include "l34"
	},
	{
	#include "l35"
	},
	{
	#include "l36"
	},
	{
	#include "l37"
	},
	{
	#include "l38"
	},
	{
	#include "l39"
	},
	{
	#include "l40"
	},
	{
	#include "l41"
	},
	{
	#include "l42"
	},
	{
	#include "l43"
	},
	{
	#include "l44"
	},
	{
	#include "l45"
	},
	{
	#include "l46"
	},
	{
	#include "l47"
	},
	{
	#include "l48"
	},
	{
	#include "l49"
	},
	{
	#include "l50"
	}
};

static const float fir_right[50][25][200] = {
	{
	#include "r1"
	},
	{
	#include "r2"
	},
	{
	#include "r3"
	},
	{
	#include "r4"
	},
	{
	#include "r5"
	},
	{
	#include "r6"
	},
	{
	#include "r7"
	},
	{
	#include "r8"
	},
	{
	#include "r9"
	},
	{
	#include "r10"
	},
	{
	#include "r11"
	},
	{
	#include "r12"
	},
	{
	#include "r13"
	},
	{
	#include "r14"
	},
	{
	#include "r15"
	},
	{
	#include "r16"
	},
	{
	#include "r17"
	},
	{
	#include "r18"
	},
	{
	#include "r19"
	},
	{
	#include "r20"
	},
	{
	#include "r21"
	},
	{
	#include "r22"
	},
	{
	#include "r23"
	},
	{
	#include "r24"
	},
	{
	#include "r25"
	},
	{
	#include "r26"
	},
	{
	#include "r27"
	},
	{
	#include "r28"
	},
	{
	#include "r29"
	},
	{
	#include "r30"
	},
	{
	#include "r31"
	},
	{
	#include "r32"
	},
	{
	#include "r33"
	},
	{
	#include "r34"
	},
	{
	#include "r35"
	},
	{
	#include "r36"
	},
	{
	#include "r37"
	},
	{
	#include "r38"
	},
	{
	#include "r39"
	},
	{
	#include "r40"
	},
	{
	#include "r41"
	},
	{
	#include "r42"
	},
	{
	#include "r43"
	},
	{
	#include "r44"
	},
	{
	#include "r45"
	},
	{
	#include "r46"
	},
	{
	#include "r47"
	},
	{
	#include "r48"
	},
	{
	#include "r49"
	},
	{
	#include "r50"
	}
};

void ZamHeadX2Plugin::activate()
{
	int i;
	for (i = 0; i < 6; i++) {
		pos[i] = 0;
	}
	for (i = 0; i < 4096+200; i++) {
		inbuf[0][i] = 0.f;
		inbuf[1][i] = 0.f;
		outbuf[0][i] = 0.f;
		outbuf[1][i] = 0.f;
	}

}

void ZamHeadX2Plugin::pushsample(float* buf, float val, int i, uint32_t maxframes)
{
	buf[pos[i]] = val;
	pos[i]--;
	if (pos[i] < 0) {
		pos[i] = (int)maxframes;
	}
}

float ZamHeadX2Plugin::getsample(float *buf, int i, uint32_t maxframes)
{
	float val = buf[pos[i]];
	pos[i]++;
	if (pos[i] >= (int)maxframes) {
		pos[i] = 0;
	}
	return val;
}

void ZamHeadX2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	uint32_t i;
	int az, el;
	
	el = (int)((elevation + 45.) / 135. * 24.);
	if (el >= 24) el = 24;
	if (el < 0) el = 0;
	az = (int)((azimuth + 90.) / 360. * 49.);
	if (az >= 49) az = 49;
	if (az < 0) az = 0;
	if (az > 24) az = 49 - az;

	float ltmp = 0.f;
	float rtmp = 0.f;
	int k;
	float m, s;

	for (i = 0; i < frames; i++) {
		m = (inputs[0][i] + inputs[1][i]) * 0.5;
		s = (inputs[0][i] - inputs[1][i]) * 0.5 * width;
		pushsample(&inbuf[0][0], m - s, 0, 200);
		pushsample(&inbuf[1][0], m + s, 1, 200);
		ltmp = 0.f;
		rtmp = 0.f;
		for (k = 0; k < 200; k++) {
			ltmp += getsample(&inbuf[0][0], 0, 200) *
					fir_left[el][az][(200-k+200)%200];
			rtmp += getsample(&inbuf[1][0], 1, 200) *
					fir_right[el][az][(200-k+200)%200];
		}
		outputs[0][i] = ltmp;
		outputs[1][i] = rtmp;
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamHeadX2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
