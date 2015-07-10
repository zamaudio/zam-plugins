/*
 * ZaMaximX2 Stereo maximiser
 * Copyright (C) 2015  Damien Zammit <damien@zamaudio.com>
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

#include "ZaMaximX2Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZaMaximX2Plugin::ZaMaximX2Plugin()
    : Plugin(paramCount, 1, 0)
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZaMaximX2Plugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramRelease:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Release";
        parameter.symbol     = "rel";
        parameter.unit       = "ms";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.1f;
        parameter.ranges.max = 100.0f;
        break;
    case paramCeiling:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Output Ceiling";
        parameter.symbol     = "ceil";
        parameter.unit       = "dB";
        parameter.ranges.def = -6.0f;
        parameter.ranges.min = -40.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramThresh:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Threshold";
        parameter.symbol     = "thresh";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -60.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramGainRed:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Gain Reduction";
        parameter.symbol     = "gr";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 40.0f;
        break;
    case paramOutputLevel:
        parameter.hints      = kParameterIsOutput;
        parameter.name       = "Output Level";
        parameter.symbol     = "outlevel";
        parameter.unit       = "dB";
        parameter.ranges.def = -45.0f;
        parameter.ranges.min = -45.0f;
        parameter.ranges.max = 0.0f;
        break;
    }
}


void ZaMaximX2Plugin::initProgramName(uint32_t index, String& programName)
{
	switch(index) {
	case 0:
		programName = "Zero";
		break;
	}
}

void ZaMaximX2Plugin::loadProgram(uint32_t index)
{
	switch(index) {
	case 0:
		release = 1.0;
		ceiling = -6.0;
		thresdb = 0.0;
		gainred = 0.0;
		outlevel = -45.0;
		break;
	}

	activate();
}

// -----------------------------------------------------------------------
// Internal data

float ZaMaximX2Plugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramRelease:
        return release;
        break;
    case paramThresh:
        return thresdb;
        break;
    case paramCeiling:
        return ceiling;
        break;
    case paramGainRed:
        return gainred;
        break;
    case paramOutputLevel:
        return outlevel;
        break;
    default:
        return 0.0f;
    }
}

void ZaMaximX2Plugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramRelease:
        release = value;
        break;
    case paramThresh:
        thresdb = value;
        break;
    case paramCeiling:
        ceiling = value;
        break;
    case paramGainRed:
        gainred = value;
        break;
    case paramOutputLevel:
        outlevel = value;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void ZaMaximX2Plugin::activate()
{
    gainred = 0.0f;
    outlevel = -45.0f;
    oldL_yl = oldL_y1 = oldR_yl = oldR_y1 = oldL_yg = oldR_yg = 0.f;
}

float ZaMaximX2Plugin::normalise(float in)
{
	if (ceiling < thresdb) {
		ceiling = thresdb;
		return in;
	}
	return from_dB(ceiling - thresdb) * in;
}

void ZaMaximX2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	float srate = getSampleRate();
	float width = 0.01;
        float cdb=0.f;
        float attack_coeff = exp(-1000.f/(1.0 * srate));
        float release_coeff = exp(-1000.f/(release * srate));

	float max = 0.f;
	float lgaininp = 0.f;
	float rgaininp = 0.f;
	float Lgain = 1.f;
        float Rgain = 1.f;
        float Lxg, Lxl, Lyg, Lyl, Ly1;
        float Rxg, Rxl, Ryg, Ryl, Ry1;
        float checkwidth = 0.f;
	uint32_t i;

        for (i = 0; i < frames; i++) {
		Lyg = Ryg = 0.f;
                Lxg = (inputs[0][i]==0.f) ? -160.f : to_dB(fabs(inputs[0][i]));
                Rxg = (inputs[1][i]==0.f) ? -160.f : to_dB(fabs(inputs[1][i]));
                Lxg = sanitize_denormal(Lxg);
                Rxg = sanitize_denormal(Rxg);

                Lyg = Lxg - (Lxg-thresdb+width/2.f)*(Lxg-thresdb+width/2.f)/(2.f*width);
                Ryg = Rxg - (Rxg-thresdb+width/2.f)*(Rxg-thresdb+width/2.f)/(2.f*width);

		checkwidth = 2.f*fabs(Lxg-thresdb);
                if (2.f*(Lxg-thresdb) < -width) {
                        Lyg = Lxg;
                } else if (checkwidth <= width) {
			Lyg = thresdb;
                } else if (2.f*(Lxg-thresdb) > width) {
                        Lyg = thresdb;
                }

		checkwidth = 2.f*fabs(Rxg-thresdb);
                if (2.f*(Rxg-thresdb) < -width) {
                        Ryg = Rxg;
                } else if (checkwidth <= width) {
			Ryg = thresdb;
                } else if (2.f*(Rxg-thresdb) > width) {
                        Ryg = thresdb;
                }

                Lxl = Rxl = fmaxf(Lxg - Lyg, Rxg - Ryg);

                oldL_y1 = sanitize_denormal(oldL_y1);
                oldR_y1 = sanitize_denormal(oldR_y1);
                oldL_yl = sanitize_denormal(oldL_yl);
                oldR_yl = sanitize_denormal(oldR_yl);
                Ly1 = fmaxf(Lxl, release_coeff * oldL_y1+(1.f-release_coeff)*Lxl);
		Lyl = attack_coeff * oldL_yl+(1.f-attack_coeff)*Ly1;
                Ly1 = sanitize_denormal(Ly1);
                Lyl = sanitize_denormal(Lyl);

                cdb = -Lyl;
                Lgain = from_dB(cdb);

                gainred = Lyl;

                Ry1 = fmaxf(Rxl, release_coeff * oldR_y1+(1.f-release_coeff)*Rxl);
                Ryl = attack_coeff * oldR_yl+(1.f-attack_coeff)*Ry1;
                Ry1 = sanitize_denormal(Ry1);
                Ryl = sanitize_denormal(Ryl);

                cdb = -Ryl;
                Rgain = from_dB(cdb);

		lgaininp = inputs[0][i] * Lgain;
		rgaininp = inputs[1][i] * Rgain;

		outputs[0][i] = normalise(lgaininp);
		outputs[1][i] = normalise(rgaininp);

		max = (fabsf(lgaininp) > max) ? fabsf(lgaininp) : sanitize_denormal(max);
		max = (fabsf(rgaininp) > max) ? fabsf(rgaininp) : sanitize_denormal(max);

                oldL_yl = Lyl;
                oldR_yl = Ryl;
                oldL_y1 = Ly1;
                oldR_y1 = Ry1;
                oldL_yg = Lyg;
                oldR_yg = Ryg;
        }
	outlevel = (max == 0.f) ? -45.f : to_dB(max) - thresdb;
    }

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZaMaximX2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
