/*
 * ZamGEQ31 31 band graphic equaliser
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

#include "ZamGEQ31X2Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamGEQ31X2Plugin::ZamGEQ31X2Plugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    // set default values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void ZamGEQ31X2Plugin::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramMaster:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Master Gain";
        parameter.symbol     = "master";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.f;
        parameter.ranges.max = 0.f;
        break;
    case paramLink:
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Link";
        parameter.symbol     = "link";
        parameter.unit       = "";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramGain1L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "20Hz";
        parameter.symbol     = "band1L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain2L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "25Hz";
        parameter.symbol     = "band2L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain3L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "31.5Hz";
        parameter.symbol     = "band3L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain4L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "40Hz";
        parameter.symbol     = "band4L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain5L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "50Hz";
        parameter.symbol     = "band5L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain6L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "63Hz";
        parameter.symbol     = "band6L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain7L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "80Hz";
        parameter.symbol     = "band7L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain8L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "100Hz";
        parameter.symbol     = "band8L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain9L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "125Hz";
        parameter.symbol     = "band9L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain10L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "160Hz";
        parameter.symbol     = "band10L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain11L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "200Hz";
        parameter.symbol     = "band11L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain12L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "250Hz";
        parameter.symbol     = "band12L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain13L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "315Hz";
        parameter.symbol     = "band13L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain14L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "400Hz";
        parameter.symbol     = "band14L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain15L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "500Hz";
        parameter.symbol     = "band15L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain16L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "630Hz";
        parameter.symbol     = "band16L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain17L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "800Hz";
        parameter.symbol     = "band17L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain18L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "1000Hz";
        parameter.symbol     = "band18L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain19L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "1250Hz";
        parameter.symbol     = "band19L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain20L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "1600Hz";
        parameter.symbol     = "band20L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain21L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "2000Hz";
        parameter.symbol     = "band21L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain22L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "2500Hz";
        parameter.symbol     = "band22L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain23L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "3150Hz";
        parameter.symbol     = "band23L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain24L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "4000Hz";
        parameter.symbol     = "band24L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain25L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "5000Hz";
        parameter.symbol     = "band25L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain26L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "6300Hz";
        parameter.symbol     = "band26L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain27L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "8000Hz";
        parameter.symbol     = "band27L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain28L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "10kHz";
        parameter.symbol     = "band28L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain29L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "12.5kHz";
        parameter.symbol     = "band29L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain30L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "16kHz";
        parameter.symbol     = "band30L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain31L:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "20kHz";
        parameter.symbol     = "band31L";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain1R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "20Hz";
        parameter.symbol     = "band1R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain2R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "25Hz";
        parameter.symbol     = "band2R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain3R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "31.5Hz";
        parameter.symbol     = "band3R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain4R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "40Hz";
        parameter.symbol     = "band4R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain5R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "50Hz";
        parameter.symbol     = "band5R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain6R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "63Hz";
        parameter.symbol     = "band6R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain7R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "80Hz";
        parameter.symbol     = "band7R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain8R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "100Hz";
        parameter.symbol     = "band8R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain9R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "125Hz";
        parameter.symbol     = "band9R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain10R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "160Hz";
        parameter.symbol     = "band10R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain11R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "200Hz";
        parameter.symbol     = "band11R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain12R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "250Hz";
        parameter.symbol     = "band12R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain13R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "315Hz";
        parameter.symbol     = "band13R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain14R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "400Hz";
        parameter.symbol     = "band14R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain15R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "500Hz";
        parameter.symbol     = "band15R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain16R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "630Hz";
        parameter.symbol     = "band16R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain17R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "800Hz";
        parameter.symbol     = "band17R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain18R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "1000Hz";
        parameter.symbol     = "band18R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain19R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "1250Hz";
        parameter.symbol     = "band19R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain20R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "1600Hz";
        parameter.symbol     = "band20R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain21R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "2000Hz";
        parameter.symbol     = "band21R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain22R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "2500Hz";
        parameter.symbol     = "band22R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain23R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "3150Hz";
        parameter.symbol     = "band23R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain24R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "4000Hz";
        parameter.symbol     = "band24R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain25R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "5000Hz";
        parameter.symbol     = "band25R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain26R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "6300Hz";
        parameter.symbol     = "band26R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain27R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "8000Hz";
        parameter.symbol     = "band27R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain28R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "10kHz";
        parameter.symbol     = "band28R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain29R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "12.5kHz";
        parameter.symbol     = "band29R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain30R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "16kHz";
        parameter.symbol     = "band30R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    case paramGain31R:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "20kHz";
        parameter.symbol     = "band31R";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -12.0f;
        parameter.ranges.max = 12.0f;
        break;
    }
}

void ZamGEQ31X2Plugin::initProgramName(uint32_t index, String& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamGEQ31X2Plugin::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramMaster:
        return master;
        break;
    case paramLink:
        return link;
        break;
    case paramGain1L:
        return gain[0][0];
        break;
    case paramGain2L:
        return gain[1][0];
        break;
    case paramGain3L:
        return gain[2][0];
        break;
    case paramGain4L:
        return gain[3][0];
        break;
    case paramGain5L:
        return gain[4][0];
        break;
    case paramGain6L:
        return gain[5][0];
        break;
    case paramGain7L:
        return gain[6][0];
        break;
    case paramGain8L:
        return gain[7][0];
        break;
    case paramGain9L:
        return gain[8][0];
        break;
    case paramGain10L:
        return gain[9][0];
        break;
    case paramGain11L:
        return gain[10][0];
        break;
    case paramGain12L:
        return gain[11][0];
        break;
    case paramGain13L:
        return gain[12][0];
        break;
    case paramGain14L:
        return gain[13][0];
        break;
    case paramGain15L:
        return gain[14][0];
        break;
    case paramGain16L:
        return gain[15][0];
        break;
    case paramGain17L:
        return gain[16][0];
        break;
    case paramGain18L:
        return gain[17][0];
        break;
    case paramGain19L:
        return gain[18][0];
        break;
    case paramGain20L:
        return gain[19][0];
        break;
    case paramGain21L:
        return gain[20][0];
        break;
    case paramGain22L:
        return gain[21][0];
        break;
    case paramGain23L:
        return gain[22][0];
        break;
    case paramGain24L:
        return gain[23][0];
        break;
    case paramGain25L:
        return gain[24][0];
        break;
    case paramGain26L:
        return gain[25][0];
        break;
    case paramGain27L:
        return gain[26][0];
        break;
    case paramGain28L:
        return gain[27][0];
        break;
    case paramGain29L:
        return gain[28][0];
        break;
    case paramGain30L:
        return gain[29][0];
        break;
    case paramGain31L:
        return gain[30][0];
        break;
    case paramGain1R:
        return gain[0][1];
        break;
    case paramGain2R:
        return gain[1][1];
        break;
    case paramGain3R:
        return gain[2][1];
        break;
    case paramGain4R:
        return gain[3][1];
        break;
    case paramGain5R:
        return gain[4][1];
        break;
    case paramGain6R:
        return gain[5][1];
        break;
    case paramGain7R:
        return gain[6][1];
        break;
    case paramGain8R:
        return gain[7][1];
        break;
    case paramGain9R:
        return gain[8][1];
        break;
    case paramGain10R:
        return gain[9][1];
        break;
    case paramGain11R:
        return gain[10][1];
        break;
    case paramGain12R:
        return gain[11][1];
        break;
    case paramGain13R:
        return gain[12][1];
        break;
    case paramGain14R:
        return gain[13][1];
        break;
    case paramGain15R:
        return gain[14][1];
        break;
    case paramGain16R:
        return gain[15][1];
        break;
    case paramGain17R:
        return gain[16][1];
        break;
    case paramGain18R:
        return gain[17][1];
        break;
    case paramGain19R:
        return gain[18][1];
        break;
    case paramGain20R:
        return gain[19][1];
        break;
    case paramGain21R:
        return gain[20][1];
        break;
    case paramGain22R:
        return gain[21][1];
        break;
    case paramGain23R:
        return gain[22][1];
        break;
    case paramGain24R:
        return gain[23][1];
        break;
    case paramGain25R:
        return gain[24][1];
        break;
    case paramGain26R:
        return gain[25][1];
        break;
    case paramGain27R:
        return gain[26][1];
        break;
    case paramGain28R:
        return gain[27][1];
        break;
    case paramGain29R:
        return gain[28][1];
        break;
    case paramGain30R:
        return gain[29][1];
        break;
    case paramGain31R:
        return gain[30][1];
        break;
    default:
        return 0.0f;
    }
}

void ZamGEQ31X2Plugin::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramMaster:
        master = value;
        break;
    case paramLink:
        link = value;
        break;
    case paramGain1L:
        gain[0][0] = value;
        break;
    case paramGain2L:
        gain[1][0] = value;
        break;
    case paramGain3L:
        gain[2][0] = value;
        break;
    case paramGain4L:
        gain[3][0] = value;
        break;
    case paramGain5L:
        gain[4][0] = value;
        break;
    case paramGain6L:
        gain[5][0] = value;
        break;
    case paramGain7L:
        gain[6][0] = value;
        break;
    case paramGain8L:
        gain[7][0] = value;
        break;
    case paramGain9L:
        gain[8][0] = value;
        break;
    case paramGain10L:
        gain[9][0] = value;
        break;
    case paramGain11L:
        gain[10][0] = value;
        break;
    case paramGain12L:
        gain[11][0] = value;
        break;
    case paramGain13L:
        gain[12][0] = value;
        break;
    case paramGain14L:
        gain[13][0] = value;
        break;
    case paramGain15L:
        gain[14][0] = value;
        break;
    case paramGain16L:
        gain[15][0] = value;
        break;
    case paramGain17L:
        gain[16][0] = value;
        break;
    case paramGain18L:
        gain[17][0] = value;
        break;
    case paramGain19L:
        gain[18][0] = value;
        break;
    case paramGain20L:
        gain[19][0] = value;
        break;
    case paramGain21L:
        gain[20][0] = value;
        break;
    case paramGain22L:
        gain[21][0] = value;
        break;
    case paramGain23L:
        gain[22][0] = value;
        break;
    case paramGain24L:
        gain[23][0] = value;
        break;
    case paramGain25L:
        gain[24][0] = value;
        break;
    case paramGain26L:
        gain[25][0] = value;
        break;
    case paramGain27L:
        gain[26][0] = value;
        break;
    case paramGain28L:
        gain[27][0] = value;
        break;
    case paramGain29L:
        gain[28][0] = value;
        break;
    case paramGain30L:
        gain[29][0] = value;
        break;
    case paramGain31L:
        gain[30][0] = value;
        break;
    case paramGain1R:
        gain[0][1] = value;
        break;
    case paramGain2R:
        gain[1][1] = value;
        break;
    case paramGain3R:
        gain[2][1] = value;
        break;
    case paramGain4R:
        gain[3][1] = value;
        break;
    case paramGain5R:
        gain[4][1] = value;
        break;
    case paramGain6R:
        gain[5][1] = value;
        break;
    case paramGain7R:
        gain[6][1] = value;
        break;
    case paramGain8R:
        gain[7][1] = value;
        break;
    case paramGain9R:
        gain[8][1] = value;
        break;
    case paramGain10R:
        gain[9][1] = value;
        break;
    case paramGain11R:
        gain[10][1] = value;
        break;
    case paramGain12R:
        gain[11][1] = value;
        break;
    case paramGain13R:
        gain[12][1] = value;
        break;
    case paramGain14R:
        gain[13][1] = value;
        break;
    case paramGain15R:
        gain[14][1] = value;
        break;
    case paramGain16R:
        gain[15][1] = value;
        break;
    case paramGain17R:
        gain[16][1] = value;
        break;
    case paramGain18R:
        gain[17][1] = value;
        break;
    case paramGain19R:
        gain[18][1] = value;
        break;
    case paramGain20R:
        gain[19][1] = value;
        break;
    case paramGain21R:
        gain[20][1] = value;
        break;
    case paramGain22R:
        gain[21][1] = value;
        break;
    case paramGain23R:
        gain[22][1] = value;
        break;
    case paramGain24R:
        gain[23][1] = value;
        break;
    case paramGain25R:
        gain[24][1] = value;
        break;
    case paramGain26R:
        gain[25][1] = value;
        break;
    case paramGain27R:
        gain[26][1] = value;
        break;
    case paramGain28R:
        gain[27][1] = value;
        break;
    case paramGain29R:
        gain[28][1] = value;
        break;
    case paramGain30R:
        gain[29][1] = value;
        break;
    case paramGain31R:
        gain[30][1] = value;
        break;
    }
}

void ZamGEQ31X2Plugin::peq(int i, int ch, float srate, float fc, float g, float)
{
        float k, v0, q;

        k = tanf(M_PI * fc / srate);
        v0 = powf(10., g / 20.); 
        q = 4.318473;
	//q = powf(2., bw/2.)/(powf(2., bw) - 1.); //q from octave bw

        if (g < 0.f) {
                // cut
                float denom = 1. + k/(v0*q) + k*k;
                b0[ch][i] = (1. + k/q + k*k) / denom;
                b1[ch][i] = 2. * (k*k - 1.) / denom;
                b2[ch][i] = (1. - k/q + k*k) / denom;
                a1[ch][i] = b1[ch][i];
                a2[ch][i] = (1. - k/(v0*q) + k*k) / denom;
        } else {
                // boost
                float denom = 1. + k/q + k*k;
                b0[ch][i] = (1. + k*v0/q + k*k) / denom;
                b1[ch][i] = 2. * (k*k - 1.) / denom;
                b2[ch][i] = (1. - k*v0/q + k*k) / denom;
                a1[ch][i] = b1[ch][i];
                a2[ch][i] = (1. - k/q + k*k) / denom;
        }
}

double ZamGEQ31X2Plugin::run_filter(int i, int ch, double in)
{
        double out;
        in = sanitize_denormal(in);
        out = in * b0[ch][i]    + x1[ch][i] * b1[ch][i]
                                + x2[ch][i] * b2[ch][i]
                                - y1[ch][i] * a1[ch][i]
                                - y2[ch][i] * a2[ch][i] + 1e-20f;
        out = sanitize_denormal(out);
        x2[ch][i] = sanitize_denormal(x1[ch][i]);
        y2[ch][i] = sanitize_denormal(y1[ch][i]);
        x1[ch][i] = in;
        y1[ch][i] = out;

        return out;
}

void ZamGEQ31X2Plugin::loadProgram(uint32_t index)
{
    if (index != 0)
        return;

    int i;
    /* Default parameter values */
    for (i = 0; i < MAX_FILT; i++) {
    	gain[i][0] = 0.0f;
    	gain[i][1] = 0.0f;
    }

    freq[0] = 20.;
    freq[1] = 25.;
    freq[2] = 31.5;
    freq[3] = 40.;
    freq[4] = 50.;
    freq[5] = 63.;
    freq[6] = 80.;
    freq[7] = 100.;
    freq[8] = 125.;
    freq[9] = 160.;
    freq[10] = 200.;
    freq[11] = 250.;
    freq[12] = 315.;
    freq[13] = 400.;
    freq[14] = 500.;
    freq[15] = 630.;
    freq[16] = 800.;
    freq[17] = 1000.;
    freq[18] = 1250.;
    freq[19] = 1600.;
    freq[20] = 2000.;
    freq[21] = 2500.;
    freq[22] = 3150.;
    freq[23] = 4000.;
    freq[24] = 5000.;
    freq[25] = 6300.;
    freq[26] = 8000.;
    freq[27] = 10000.;
    freq[28] = 12500.;
    freq[29] = 16000.;
    freq[30] = 20000.;
    master = 0.f;
    link = 0;
    float srate = getSampleRate();
    for (int i=0; i < MAX_FILT; ++i) {
    	peq(i, 0, srate, freq[i], 0., 1./3.);
    	peq(i, 1, srate, freq[i], 0., 1./3.);
    }

    /* reset filter values */
    activate();
}

// -----------------------------------------------------------------------
// Process

void ZamGEQ31X2Plugin::activate()
{
        int i;
	for (i = 0; i < MAX_FILT; ++i) {
		x1[0][i] = x2[0][i] = 0.f;
		y1[0][i] = y2[0][i] = 0.f;
		a1[0][i] = a2[0][i] = 0.f;
		b0[0][i] = b1[0][i] = b2[0][i] = 0.f;
		x1[1][i] = x2[1][i] = 0.f;
		y1[1][i] = y2[1][i] = 0.f;
		a1[1][i] = a2[1][i] = 0.f;
		b0[1][i] = b1[1][i] = b2[1][i] = 0.f;
        }
}

/*
void ZamGEQ31X2Plugin::geq31(int i, int ch, float srate, float fc, float q)
{
	double t0;

	t0 = 2.*M_PI*fc/srate;
	b[ch][i] = (q - t0*0.5)/(2.*q+t0);
	a[ch][i] = (0.5 - b[ch][i])*0.5;
	g[ch][i] = (0.5 + b[ch][i])*cos(t0);
}

double ZamGEQ31X2Plugin::run_filter(int i, int ch, double in)
{
	double out;
	in = sanitize_denormal(in);
	out = 2.*(a[ch][i]*(in - x2[ch][i]) + g[ch][i]*y1[ch][i] - b[ch][i]*y2[ch][i]) + 1e-20f;
	out = sanitize_denormal(out);
	x2[ch][i] = sanitize_denormal(x1[ch][i]);
	y2[ch][i] = sanitize_denormal(y1[ch][i]);
	x1[ch][i] = in;
	y1[ch][i] = out;

	return out;
}
*/

void ZamGEQ31X2Plugin::run(const float** inputs, float** outputs, uint32_t frames)
{
	float srate = getSampleRate();
	
	uint32_t i, j;
	for (i = 0; i < MAX_FILT; i++) {
		peq(i, 0, srate, freq[i], gain[i][0], 1./3.);
		peq(i, 1, srate, freq[i], gain[i][1], 1./3.);
	}

	for (i = 0; i < frames; i++) {
		double tmpl, tmpr, filteredL, filteredR;
		double inL = inputs[0][i];
		double inR = inputs[1][i];
		inL = sanitize_denormal(inL);
		inR = sanitize_denormal(inR);
		tmpl = inL;
		tmpr = inR;

		for (j = 0; j < 31; j++) {
			if (gain[j][0] != 0.f) {
				filteredL = run_filter(j, 0, tmpl);
				tmpl = filteredL;
			}
			if (gain[j][1] != 0.f) {
				filteredR = run_filter(j, 1, tmpr);
				tmpr = filteredR;
			}
		}

		outputs[0][i] = inputs[0][i];
		outputs[0][i] = (float) (filteredL * from_dB(master));
		outputs[1][i] = inputs[1][i];
		outputs[1][i] = (float) (filteredR * from_dB(master));
	}
}

Plugin* createPlugin()
{
    return new ZamGEQ31X2Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
