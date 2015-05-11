/*
 * ZamChild670 Stereo tube limiter
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

#include "ZamChild670Plugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamChild670Plugin::ZamChild670Plugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    // set default values
    inputLevel = from_dB(0.0);
    ACThreshold = from_dB(-6.f);
    timeConstantSelect = 2;
    DCThreshold = from_dB(-12.f);
    outputGain = from_dB(0.0);
    params = new Wavechild670Parameters(inputLevel,
    		ACThreshold, timeConstantSelect, DCThreshold, 
		inputLevel, ACThreshold, timeConstantSelect, DCThreshold, 
		true, false, true, outputGain, false);
    zamchild = new Wavechild670(d_getSampleRate(), *params);
    d_setProgram(0);
}

ZamChild670Plugin::~ZamChild670Plugin()
{
	delete params;
	delete zamchild;
}

// -----------------------------------------------------------------------
// Init

void ZamChild670Plugin::d_initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramInlevel:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Input level";
        parameter.symbol     = "inlevel";
        parameter.unit       = "dB";
        parameter.ranges.def = -16.0f;
        parameter.ranges.min = -30.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramAC:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "AC Threshold";
        parameter.symbol     = "acthres";
        parameter.unit       = "dB";
        parameter.ranges.def = -3.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramDC:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "DC Threshold";
        parameter.symbol     = "dcthres";
        parameter.unit       = "dB";
        parameter.ranges.def = -6.0f;
        parameter.ranges.min = -20.0f;
        parameter.ranges.max = 0.0f;
        break;
    case paramTau:
        parameter.hints      = kParameterIsAutomable | kParameterIsInteger;
        parameter.name       = "Time constant select";
        parameter.symbol     = "tau";
        parameter.unit       = " ";
        parameter.ranges.def = 2.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 6.0f;
        break;
    case paramOutlevel:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Output level";
        parameter.symbol     = "outlevel";
        parameter.unit       = "dB";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -30.0f;
        parameter.ranges.max = 0.0f;
        break;
    }
}

void ZamChild670Plugin::d_initProgramName(uint32_t index, d_string& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamChild670Plugin::d_getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramInlevel:
        return inputLevel;
        break;
    case paramAC:
        return ACThreshold;
        break;
    case paramDC:
        return DCThreshold;
        break;
    case paramTau:
        return timeConstantSelect;
        break;
    case paramOutlevel:
        return outputGain;
        break;
    default:
        return 0.0f;
    }
}

void ZamChild670Plugin::d_setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramInlevel:
        inputLevel = value;
        break;
    case paramAC:
        ACThreshold = value;
        break;
    case paramDC:
        DCThreshold = value;
        break;
    case paramTau:
        timeConstantSelect = value;
        break;
    case paramOutlevel:
        outputGain = value;
        break;
    }
}

void ZamChild670Plugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    inputLevel = 0.0;
    ACThreshold = -6.f;
    timeConstantSelect = 2;
    DCThreshold = -12.f;
    outputGain = 0.0;

    /* reset filter values */
    d_activate();
}

// -----------------------------------------------------------------------
// Process

void ZamChild670Plugin::d_activate()
{
	params->inputLevelA = params->inputLevelB = from_dB(inputLevel);
	params->ACThresholdA = params->ACThresholdB = from_dB(ACThreshold);
	params->timeConstantSelectA = params->timeConstantSelectB = timeConstantSelect;
	params->DCThresholdA = params->DCThresholdB = from_dB(DCThreshold);
	params->outputGain = from_dB(outputGain);
	zamchild->setParameters(*params);
	zamchild->warmUp();
}

void ZamChild670Plugin::d_run(const float** inputs, float** outputs, uint32_t frames)
{
	params->inputLevelA = params->inputLevelB = from_dB(inputLevel);
	params->ACThresholdA = params->ACThresholdB = from_dB(ACThreshold);
	params->timeConstantSelectA = params->timeConstantSelectB = timeConstantSelect;
	params->DCThresholdA = params->DCThresholdB = from_dB(DCThreshold);
	params->outputGain = from_dB(outputGain);
	zamchild->setParameters(*params);
	
	zamchild->process(inputs, outputs, (ulong)frames);
}
// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamChild670Plugin();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
