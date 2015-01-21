/*
 * ZamPiano physically modelled piano synth
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

#include "ZamPianoPlugin.hpp"

template <int N> inline float faustpower(float x)          { return powf(x,N); } 
template <int N> inline double faustpower(double x)        { return pow(x,N); }
template <int N> inline int faustpower(int x)              { return faustpower<N/2>(x) * faustpower<N-N/2>(x); } 
template <> 	 inline int faustpower<0>(int)            { return 1; }
template <> 	 inline int faustpower<1>(int x)            { return x; }
inline int 	max (unsigned int a, unsigned int b) { return (a>b) ? a : b; }
inline int 	max (int a, int b)		{ return (a>b) ? a : b; }
inline long 	max (long a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (int a, long b) 		{ return (a>b) ? a : b; }
inline long 	max (long a, int b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (int a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, int b) 		{ return (a>b) ? a : b; }
inline float 	max (long a, float b) 		{ return (a>b) ? a : b; }
inline float 	max (float a, long b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (int a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, int b) 		{ return (a>b) ? a : b; }
inline double 	max (long a, double b) 		{ return (a>b) ? a : b; }
inline double 	max (double a, long b) 		{ return (a>b) ? a : b; }
inline double 	max (float a, double b) 	{ return (a>b) ? a : b; }
inline double 	max (double a, float b) 	{ return (a>b) ? a : b; }
inline int	min (int a, int b)		{ return (a<b) ? a : b; }
inline long 	min (long a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (int a, long b) 		{ return (a<b) ? a : b; }
inline long 	min (long a, int b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (int a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, int b) 		{ return (a<b) ? a : b; }
inline float 	min (long a, float b) 		{ return (a<b) ? a : b; }
inline float 	min (float a, long b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (int a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, int b) 		{ return (a<b) ? a : b; }
inline double 	min (long a, double b) 		{ return (a<b) ? a : b; }
inline double 	min (double a, long b) 		{ return (a<b) ? a : b; }
inline double 	min (float a, double b) 	{ return (a<b) ? a : b; }
inline double 	min (double a, float b) 	{ return (a<b) ? a : b; }
#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
  #define AVOIDDENORMALS
#endif

inline int	lsr (int x, int n)		{ return int(((unsigned int)x) >> n); }

#define portno(label) (is_voice_ctrl(label)?-1:nports++)

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

#ifndef FAUSTCLASS 
#define FAUSTCLASS piano
#endif

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

ZamPianoPlugin::ZamPianoPlugin()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    // set default values
    d_setProgram(0);
}

ZamPianoPlugin::~ZamPianoPlugin()
{
}

// -----------------------------------------------------------------------
// Init

void ZamPianoPlugin::d_initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramFreq: // fentry0
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Tone Frequency";
        parameter.symbol     = "freq";
        parameter.unit       = "Hz";
        parameter.ranges.def = 440.0f;
        parameter.ranges.min = 20.0f;
        parameter.ranges.max = 20000.0f;
	break;
    case paramGain: // fentry1
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Gain";
        parameter.symbol     = "gain";
        parameter.unit       = "";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramGate: // fbutton0
        parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
        parameter.name       = "Gate";
        parameter.symbol     = "gate";
        parameter.unit       = "";
        parameter.ranges.def = 1.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
	break;
    case paramBright: // fslider0
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Brightness";
        parameter.symbol     = "brightness";
        parameter.unit       = "";
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramDetuning: // fslider3
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Detuning";
        parameter.symbol     = "detuning";
        parameter.unit       = "";
        parameter.ranges.def = 0.1f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramHammerHard: // fslider1
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Hammer Hardness";
        parameter.symbol     = "hammerhard";
        parameter.unit       = "";
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramStiffness: // fslider2
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Stiffness";
        parameter.symbol     = "stiffness";
        parameter.unit       = "";
        parameter.ranges.def = 0.28f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramReverbGain: // fslider4
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Reverb Gain";
        parameter.symbol     = "reverbgain";
        parameter.unit       = "";
        parameter.ranges.def = 0.137f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramReverbRoom: // fslider6
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Reverb Room Size";
        parameter.symbol     = "reverbroom";
        parameter.unit       = "";
        parameter.ranges.def = 0.72f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 2.0f;
        break;
    case paramPanAngle: // fslider5
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Pan Angle";
        parameter.symbol     = "panangle";
        parameter.unit       = "";
        parameter.ranges.def = 0.6f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramSpatialWidth: // fslider7
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Spatial Width";
        parameter.symbol     = "spatialwidth";
        parameter.unit       = "";
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    }
}

void ZamPianoPlugin::d_initProgramName(uint32_t index, d_string& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float ZamPianoPlugin::d_getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramFreq: // fentry0
        return pfreq;
        break;
    case paramGain: // fentry1
        return pgain;
        break;
    case paramGate: // fbutton0
        return pgate;
        break;
    case paramBright: // fslider0
        return pbright;
        break;
    case paramDetuning: // fslider3
        return pdetune;
        break;
    case paramHammerHard: // fslider1
        return phammer;
        break;
    case paramStiffness: // fslider2
        return pstiff;
        break;
    case paramReverbGain: // fslider4
        return prevgain;
        break;
    case paramReverbRoom: // fslider6
        return prevroom;
        break;
    case paramPanAngle: // fslider5
        return ppanangle;
        break;
    case paramSpatialWidth: // fslider7
        return pspatialwidth;
        break;
    default:
    	return 0;
	break;
    }
}

void ZamPianoPlugin::d_setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramFreq: // fentry0
        pfreq = value;
        break;
    case paramGain: // fentry1
        pgain = value;
        break;
    case paramGate: // fbutton0
        pgate = value;
        break;
    case paramBright: // fslider0
        pbright = value;
        break;
    case paramDetuning: // fslider3
        pdetune = value;
        break;
    case paramHammerHard: // fslider1
        phammer = value;
        break;
    case paramStiffness: // fslider2
        pstiff = value;
        break;
    case paramReverbGain: // fslider4
        prevgain = value;
        break;
    case paramReverbRoom: // fslider6
        prevroom = value;
        break;
    case paramPanAngle: // fslider5
        ppanangle = value;
        break;
    case paramSpatialWidth: // fslider7
        pspatialwidth = value;
        break;
    }
}

void ZamPianoPlugin::d_setProgram(uint32_t index)
{
    if (index != 0)
        return;

    /* Default parameter values */
    /* reset filter values */
    d_activate();
}

/*
void ZamPianoPlugin::d_setState(const char*, const char*)
{
}

void ZamPianoPlugin::d_initStateKey(unsigned int, d_string&)
{
}
*/

// -----------------------------------------------------------------------
// Process

void ZamPianoPlugin::d_activate()
{
	int i;
	for (i = 0; i < 128; i++) {
		note[i].state = SILENT;
		note[i].vel = 0.;
	}

	noteOffDelayTime_points[0] = 89.000;
	noteOffDelayTime_points[1] =  3.000;
	noteOffDelayTime_points[2] = 93.000;
	noteOffDelayTime_points[3] =  2.000;
	noteOffDelayTime_points[4] = 95.000;
	noteOffDelayTime_points[5] =  1.500;
	noteOffDelayTime_points[6] = 99.782;
	noteOffDelayTime_points[7] =  1.020;
	noteOffDelayTime_points[8] = 108.000;
	noteOffDelayTime_points[9] =  0.300;

	singleStringDecayRate_points[0] = 21.000;
	singleStringDecayRate_points[1] = -1.500;
	singleStringDecayRate_points[2] = 24.000;
	singleStringDecayRate_points[3] = -1.500;
	singleStringDecayRate_points[4] = 28.000;
	singleStringDecayRate_points[5] = -1.500;
	singleStringDecayRate_points[6] = 29.000;
	singleStringDecayRate_points[7] = -6.000;
	singleStringDecayRate_points[8] = 36.000;
	singleStringDecayRate_points[9] = -6.000;
	singleStringDecayRate_points[10] = 42.000;
	singleStringDecayRate_points[11] = -6.100;
	singleStringDecayRate_points[12] = 48.000;
	singleStringDecayRate_points[13] = -7.000;
	singleStringDecayRate_points[14] = 52.836;
	singleStringDecayRate_points[15] = -7.000;
	singleStringDecayRate_points[16] = 60.000;
	singleStringDecayRate_points[17] = -7.300;
	singleStringDecayRate_points[18] = 66.000;
	singleStringDecayRate_points[19] = -7.700;
	singleStringDecayRate_points[20] = 72.000;
	singleStringDecayRate_points[21] = -8.000;
	singleStringDecayRate_points[22] = 78.000;
	singleStringDecayRate_points[23] = -8.800;
	singleStringDecayRate_points[24] = 84.000;
	singleStringDecayRate_points[25] = -10.000;
	singleStringDecayRate_points[26] = 88.619;
	singleStringDecayRate_points[27] = -11.215;
	singleStringDecayRate_points[28] = 92.368;
	singleStringDecayRate_points[29] = -12.348;
	singleStringDecayRate_points[30] = 95.684;
	singleStringDecayRate_points[31] = -13.934;
	singleStringDecayRate_points[32] = 99.000;
	singleStringDecayRate_points[33] = -15.000;

	singleStringZero_points[0] = 21.000;
	singleStringZero_points[1] = -1.000;
	singleStringZero_points[2] = 24.000;
	singleStringZero_points[3] = -1.000;
	singleStringZero_points[4] = 28.000;
	singleStringZero_points[5] = -1.000;
	singleStringZero_points[6] = 29.000;
	singleStringZero_points[7] = -1.000;
	singleStringZero_points[8] = 32.534;
	singleStringZero_points[9] = -1.000;
	singleStringZero_points[10] = 36.000;
	singleStringZero_points[11] = -0.700;
	singleStringZero_points[12] = 42.000;
	singleStringZero_points[13] = -0.400;
	singleStringZero_points[14] = 48.000;
	singleStringZero_points[15] = -0.200;
	singleStringZero_points[16] = 54.000;
	singleStringZero_points[17] = -0.120;
	singleStringZero_points[18] = 60.000;
	singleStringZero_points[19] = -0.080;
	singleStringZero_points[20] = 66.000;
	singleStringZero_points[21] = -0.070;
	singleStringZero_points[22] = 72.000;
	singleStringZero_points[23] = -0.070;
	singleStringZero_points[24] = 79.000;
	singleStringZero_points[25] = -0.065;
	singleStringZero_points[26] = 84.000;
	singleStringZero_points[27] = -0.063;
	singleStringZero_points[28] = 88.000;
	singleStringZero_points[29] = -0.060;
	singleStringZero_points[30] = 96.000;
	singleStringZero_points[31] = -0.050;
	singleStringZero_points[32] = 99.000;
	singleStringZero_points[33] = -0.05;


	singleStringPole_points[0] = 21.000;
	singleStringPole_points[1] = 0.350;
	singleStringPole_points[2] = 24.604;
	singleStringPole_points[3] = 0.318;
	singleStringPole_points[4] = 26.335;
	singleStringPole_points[5] = 0.279;
	singleStringPole_points[6] = 28.000;
	singleStringPole_points[7] = 0.250;
	singleStringPole_points[8] = 32.000;
	singleStringPole_points[9] = 0.150;
	singleStringPole_points[10] = 36.000;
	singleStringPole_points[11] = 0.000;
	singleStringPole_points[12] = 42.000;
	singleStringPole_points[13] = 0.000;
	singleStringPole_points[14] = 48.000;
	singleStringPole_points[15] = 0.000;
	singleStringPole_points[16] = 54.000;
	singleStringPole_points[17] = 0.000;
	singleStringPole_points[18] = 60.000;
	singleStringPole_points[19] = 0.000;
	singleStringPole_points[20] = 66.000;
	singleStringPole_points[21] = 0.000;
	singleStringPole_points[22] = 72.000;
	singleStringPole_points[23] = 0.000;
	singleStringPole_points[24] = 76.000;
	singleStringPole_points[25] = 0.000;
	singleStringPole_points[26] = 84.000;
	singleStringPole_points[27] = 0.000;
	singleStringPole_points[28] = 88.000;
	singleStringPole_points[29] = 0.000;
	singleStringPole_points[30] = 96.000;
	singleStringPole_points[31] = 0.000;
	singleStringPole_points[32] = 99.000;
	singleStringPole_points[33] = 0.00;

	releaseLoopGain_points[0] = 21.000;
	releaseLoopGain_points[1] = 0.865;
	releaseLoopGain_points[2] = 24.000;
	releaseLoopGain_points[3] = 0.880;
	releaseLoopGain_points[4] = 29.000;
	releaseLoopGain_points[5] = 0.896;
	releaseLoopGain_points[6] = 36.000;
	releaseLoopGain_points[7] = 0.910;
	releaseLoopGain_points[8] = 48.000;
	releaseLoopGain_points[9] = 0.920;
	releaseLoopGain_points[10] = 60.000;
	releaseLoopGain_points[11] = 0.950;
	releaseLoopGain_points[12] = 72.000;
	releaseLoopGain_points[13] = 0.965;
	releaseLoopGain_points[14] = 84.000;
	releaseLoopGain_points[15] = 0.988;
	releaseLoopGain_points[16] = 88.000;
	releaseLoopGain_points[17] = 0.997;
	releaseLoopGain_points[18] = 99.000;
	releaseLoopGain_points[19] = 0.988;
	
	detuningHz_points[0] = 21.000;
	detuningHz_points[1] = 0.003;
	detuningHz_points[2] = 24.000;
	detuningHz_points[3] = 0.003;
	detuningHz_points[4] = 28.000;
	detuningHz_points[5] = 0.003;
	detuningHz_points[6] = 29.000;
	detuningHz_points[7] = 0.060;
	detuningHz_points[8] = 31.000;
	detuningHz_points[9] = 0.100;
	detuningHz_points[10] = 36.000;
	detuningHz_points[11] = 0.110;
	detuningHz_points[12] = 42.000;
	detuningHz_points[13] = 0.120;
	detuningHz_points[14] = 48.000;
	detuningHz_points[15] = 0.200;
	detuningHz_points[16] = 54.000;
	detuningHz_points[17] = 0.200;
	detuningHz_points[18] = 60.000;
	detuningHz_points[19] = 0.250;
	detuningHz_points[20] = 66.000;
	detuningHz_points[21] = 0.270;
	detuningHz_points[22] = 72.232;
	detuningHz_points[23] = 0.300;
	detuningHz_points[24] = 78.000;
	detuningHz_points[25] = 0.350;
	detuningHz_points[26] = 84.000;
	detuningHz_points[27] = 0.500;
	detuningHz_points[28] = 88.531;
	detuningHz_points[29] = 0.582;
	detuningHz_points[30] = 92.116;
	detuningHz_points[31] = 0.664;
	detuningHz_points[32] = 95.844;
	detuningHz_points[33] = 0.793;
	detuningHz_points[34] = 99.000;
	detuningHz_points[35] = 1.000;

	
	stiffnessCoefficient_points[0] = 21.000;
	stiffnessCoefficient_points[1] = -0.850;
	stiffnessCoefficient_points[2] = 23.595;
	stiffnessCoefficient_points[3] = -0.850;
	stiffnessCoefficient_points[4] = 27.055;
	stiffnessCoefficient_points[5] = -0.830;
	stiffnessCoefficient_points[6] = 29.000;
	stiffnessCoefficient_points[7] = -0.700;
	stiffnessCoefficient_points[8] = 37.725;
	stiffnessCoefficient_points[9] = -0.516;
	stiffnessCoefficient_points[10] = 46.952;
	stiffnessCoefficient_points[11] = -0.352;
	stiffnessCoefficient_points[12] = 60.000;
	stiffnessCoefficient_points[13] = -0.250;
	stiffnessCoefficient_points[14] = 73.625;
	stiffnessCoefficient_points[15] = -0.036;
	stiffnessCoefficient_points[16] = 93.810;
	stiffnessCoefficient_points[17] = -0.006;
	stiffnessCoefficient_points[18] = 99.000;
	stiffnessCoefficient_points[19] = 1.011;

	strikePosition_points[0] = 21.000;
	strikePosition_points[1] = 0.050;
	strikePosition_points[2] = 24.000;
	strikePosition_points[3] = 0.050;
	strikePosition_points[4] = 28.000;
	strikePosition_points[5] = 0.050;
	strikePosition_points[6] = 35.000;
	strikePosition_points[7] = 0.050;
	strikePosition_points[8] = 41.000;
	strikePosition_points[9] = 0.050;
	strikePosition_points[10] = 42.000;
	strikePosition_points[11] = 0.125;
	strikePosition_points[12] = 48.000;
	strikePosition_points[13] = 0.125;
	strikePosition_points[14] = 60.000;
	strikePosition_points[15] = 0.125;
	strikePosition_points[16] = 72.000;
	strikePosition_points[17] = 0.125;
	strikePosition_points[18] = 84.000;
	strikePosition_points[19] = 0.125;
	strikePosition_points[20] = 96.000;
	strikePosition_points[21] = 0.125;
	strikePosition_points[22] = 99.000;
	strikePosition_points[23] = 0.125;

	EQGain_points[0] = 21.000;
	EQGain_points[1] = 2.000;
	EQGain_points[2] = 24.000;
	EQGain_points[3] = 2.000;
	EQGain_points[4] = 28.000;
	EQGain_points[5] = 2.000;
	EQGain_points[6] = 30.000;
	EQGain_points[7] = 2.000;
	EQGain_points[8] = 35.562;
	EQGain_points[9] = 1.882;
	EQGain_points[10] = 41.000;
	EQGain_points[11] = 1.200;
	EQGain_points[12] = 42.000;
	EQGain_points[13] = 0.600;
	EQGain_points[14] = 48.000;
	EQGain_points[15] = 0.500;
	EQGain_points[16] = 54.000;
	EQGain_points[17] = 0.500;
	EQGain_points[18] = 59.928;
	EQGain_points[19] = 0.502;
	EQGain_points[20] = 66.704;
	EQGain_points[21] = 0.489;
	EQGain_points[22] = 74.201;
	EQGain_points[23] = 0.477;
	EQGain_points[24] = 91.791;
	EQGain_points[25] = 1.000;
	EQGain_points[26] = 99.000;
	EQGain_points[27] = 1.000;

	EQBandwidthFactor_points[0] = 21.000;
	EQBandwidthFactor_points[1] = 5.000;
	EQBandwidthFactor_points[2] = 24.112;
	EQBandwidthFactor_points[3] = 5.000;
	EQBandwidthFactor_points[4] = 28.000;
	EQBandwidthFactor_points[5] = 5.000;
	EQBandwidthFactor_points[6] = 35.000;
	EQBandwidthFactor_points[7] = 4.956;
	EQBandwidthFactor_points[8] = 41.000;
	EQBandwidthFactor_points[9] = 6.000;
	EQBandwidthFactor_points[10] = 42.000;
	EQBandwidthFactor_points[11] = 2.000;
	EQBandwidthFactor_points[12] = 48.773;
	EQBandwidthFactor_points[13] = 1.072;
	EQBandwidthFactor_points[14] = 57.558;
	EQBandwidthFactor_points[15] = 1.001;
	EQBandwidthFactor_points[16] = 63.226;
	EQBandwidthFactor_points[17] = 1.048;
	EQBandwidthFactor_points[18] = 69.178;
	EQBandwidthFactor_points[19] = 1.120;
	EQBandwidthFactor_points[20] = 72.862;
	EQBandwidthFactor_points[21] = 1.525;
	EQBandwidthFactor_points[22] = 80.404;
	EQBandwidthFactor_points[23] = 2.788;
	EQBandwidthFactor_points[24] = 97.659;
	EQBandwidthFactor_points[25] = 1.739;
	
	loudPole_points[0] = 21.000;
	loudPole_points[1] = 0.875;
	loudPole_points[2] = 23.719;
	loudPole_points[3] = 0.871;
	loudPole_points[4] = 27.237;
	loudPole_points[5] = 0.836;
	loudPole_points[6] = 28.996;
	loudPole_points[7] = 0.828;
	loudPole_points[8] = 32.355;
	loudPole_points[9] = 0.820;
	loudPole_points[10] = 36.672;
	loudPole_points[11] = 0.816;
	loudPole_points[12] = 40.671;
	loudPole_points[13] = 0.820;
	loudPole_points[14] = 45.788;
	loudPole_points[15] = 0.812;
	loudPole_points[16] = 47.867;
	loudPole_points[17] = 0.812;
	loudPole_points[18] = 54.000;
	loudPole_points[19] = 0.810;
	loudPole_points[20] = 60.000;
	loudPole_points[21] = 0.800;
	loudPole_points[22] = 66.000;
	loudPole_points[23] = 0.800;
	loudPole_points[24] = 72.000;
	loudPole_points[25] = 0.810;
	loudPole_points[26] = 78.839;
	loudPole_points[27] = 0.824;
	loudPole_points[28] = 84.446;
	loudPole_points[29] = 0.844;
	loudPole_points[30] = 89.894;
	loudPole_points[31] = 0.844;
	loudPole_points[32] = 96.463;
	loudPole_points[33] = 0.848;
	loudPole_points[34] = 103.512;
	loudPole_points[35] = 0.840;
	loudPole_points[36] = 107.678;
	loudPole_points[37] = 0.840;

	softPole_points[0] = 21.000;
	softPole_points[1] = 0.990;
	softPole_points[2] = 24.000;
	softPole_points[3] = 0.990;
	softPole_points[4] = 28.000;
	softPole_points[5] = 0.990;
	softPole_points[6] = 29.000;
	softPole_points[7] = 0.990;
	softPole_points[8] = 36.000;
	softPole_points[9] = 0.990;
	softPole_points[10] = 42.000;
	softPole_points[11] = 0.990;
	softPole_points[12] = 48.000;
	softPole_points[13] = 0.985;
	softPole_points[14] = 54.000;
	softPole_points[15] = 0.970;
	softPole_points[16] = 60.000;
	softPole_points[17] = 0.960;
	softPole_points[18] = 66.000;
	softPole_points[19] = 0.960;
	softPole_points[20] = 72.000;
	softPole_points[21] = 0.960;
	softPole_points[22] = 78.000;
	softPole_points[23] = 0.970;
	softPole_points[24] = 84.673;
	softPole_points[25] = 0.975;
	softPole_points[26] = 91.157;
	softPole_points[27] = 0.990;
	softPole_points[28] = 100.982;
	softPole_points[29] = 0.970;
	softPole_points[30] = 104.205;
	softPole_points[31] = 0.950;
	
	normalizedVelocity_points[0] = 0.000;
	normalizedVelocity_points[1] = 0.000;
	normalizedVelocity_points[2] = 0.170;
	normalizedVelocity_points[3] = 0.318;
	normalizedVelocity_points[4] = 0.316;
	normalizedVelocity_points[5] = 0.546;
	normalizedVelocity_points[6] = 0.460;
	normalizedVelocity_points[7] = 0.709;
	normalizedVelocity_points[8] = 0.599;
	normalizedVelocity_points[9] = 0.825;
	normalizedVelocity_points[10] = 0.717;
	normalizedVelocity_points[11] = 0.894;
	normalizedVelocity_points[12] = 0.841;
	normalizedVelocity_points[13] = 0.945;
	normalizedVelocity_points[14] = 1.000;
	normalizedVelocity_points[15] = 1.000;
	
	loudGain_points[0] = 21.873;
	loudGain_points[1] = 0.891;
	loudGain_points[2] = 25.194;
	loudGain_points[3] = 0.870;
	loudGain_points[4] = 30.538;
	loudGain_points[5] = 0.848;
	loudGain_points[6] = 35.448;
	loudGain_points[7] = 0.853;
	loudGain_points[8] = 41.513;
	loudGain_points[9] = 0.842;
	loudGain_points[10] = 47.434;
	loudGain_points[11] = 0.826;
	loudGain_points[12] = 53.644;
	loudGain_points[13] = 0.820;
	loudGain_points[14] = 60.720;
	loudGain_points[15] = 0.815;
	loudGain_points[16] = 65.630;
	loudGain_points[17] = 0.820;
	loudGain_points[18] = 72.995;
	loudGain_points[19] = 0.853;
	loudGain_points[20] = 79.060;
	loudGain_points[21] = 0.920;
	loudGain_points[22] = 85.270;
	loudGain_points[23] = 1.028;
	loudGain_points[24] = 91.624;
	loudGain_points[25] = 1.247;
	loudGain_points[26] = 95.668;
	loudGain_points[27] = 1.296;
	loudGain_points[28] = 99.000;
	loudGain_points[29] = 1.300;
	loudGain_points[30] = 100.000;
	loudGain_points[31] = 1.100;

	softGain_points[0] = 20.865;
	softGain_points[1] = 0.400;
	softGain_points[2] = 22.705;
	softGain_points[3] = 0.400;
	softGain_points[4] = 25.960;
	softGain_points[5] = 0.400;
	softGain_points[6] = 28.224;
	softGain_points[7] = 0.400;
	softGain_points[8] = 31.196;
	softGain_points[9] = 0.400;
	softGain_points[10] = 36.715;
	softGain_points[11] = 0.400;
	softGain_points[12] = 44.499;
	softGain_points[13] = 0.400;
	softGain_points[14] = 53.981;
	softGain_points[15] = 0.400;
	softGain_points[16] = 60.000;
	softGain_points[17] = 0.350;
	softGain_points[18] = 66.000;
	softGain_points[19] = 0.350;
	softGain_points[20] = 72.661;
	softGain_points[21] = 0.350;
	softGain_points[22] = 81.435;
	softGain_points[23] = 0.430;
	softGain_points[24] = 88.311;
	softGain_points[25] = 0.450;
	softGain_points[26] = 93.040;
	softGain_points[27] = 0.500;
	softGain_points[28] = 96.434;
	softGain_points[29] = 0.500;

	sustainPedalLevel_points[0] = 21.000;
	sustainPedalLevel_points[1] = 0.050;
	sustainPedalLevel_points[2] = 24.000;
	sustainPedalLevel_points[3] = 0.050;
	sustainPedalLevel_points[4] = 31.000;
	sustainPedalLevel_points[5] = 0.030;
	sustainPedalLevel_points[6] = 36.000;
	sustainPedalLevel_points[7] = 0.025;
	sustainPedalLevel_points[8] = 48.000;
	sustainPedalLevel_points[9] = 0.010;
	sustainPedalLevel_points[10] = 60.000;
	sustainPedalLevel_points[11] = 0.005;
	sustainPedalLevel_points[12] = 66.000;
	sustainPedalLevel_points[13] = 0.003;
	sustainPedalLevel_points[14] = 72.000;
	sustainPedalLevel_points[15] = 0.002;
	sustainPedalLevel_points[16] = 78.000;
	sustainPedalLevel_points[17] = 0.002;
	sustainPedalLevel_points[18] = 84.000;
	sustainPedalLevel_points[19] = 0.003;
	sustainPedalLevel_points[20] = 90.000;
	sustainPedalLevel_points[21] = 0.003;
	sustainPedalLevel_points[22] = 96.000;
	sustainPedalLevel_points[23] = 0.003;
	sustainPedalLevel_points[24] = 108.000;
	sustainPedalLevel_points[25] = 0.002;

	DryTapAmpT60_points[0] = 21.001;
	DryTapAmpT60_points[1] = 0.491;
	DryTapAmpT60_points[2] = 26.587;
	DryTapAmpT60_points[3] = 0.498;
	DryTapAmpT60_points[4] = 34.249;
	DryTapAmpT60_points[5] = 0.470;
	DryTapAmpT60_points[6] = 40.794;
	DryTapAmpT60_points[7] = 0.441;
	DryTapAmpT60_points[8] = 47.977;
	DryTapAmpT60_points[9] = 0.392;
	DryTapAmpT60_points[10] = 55.000;
	DryTapAmpT60_points[11] = 0.370;
	DryTapAmpT60_points[12] = 60.000;
	DryTapAmpT60_points[13] = 0.370;
	DryTapAmpT60_points[14] = 66.000;
	DryTapAmpT60_points[15] = 0.370;
	DryTapAmpT60_points[16] = 71.934;
	DryTapAmpT60_points[17] = 0.370;
	DryTapAmpT60_points[18] = 78.000;
	DryTapAmpT60_points[19] = 0.370;
	DryTapAmpT60_points[20] = 83.936;
	DryTapAmpT60_points[21] = 0.390;
	DryTapAmpT60_points[22] = 88.557;
	DryTapAmpT60_points[23] = 0.387;
	DryTapAmpT60_points[24] = 92.858;
	DryTapAmpT60_points[25] = 0.400;
	DryTapAmpT60_points[26] = 97.319;
	DryTapAmpT60_points[27] = 0.469;
	DryTapAmpT60_points[28] = 102.400;
	DryTapAmpT60_points[29] = 0.500;
	DryTapAmpT60_points[30] = 107.198;
	DryTapAmpT60_points[31] = 0.494;

	DryTapAmpCurrent = 0.15;
	
	DCBa1_points[0] = 21.000;
	DCBa1_points[1] = -0.999;
	DCBa1_points[2] = 24.000;
	DCBa1_points[3] = -0.999;
	DCBa1_points[4] = 30.000;
	DCBa1_points[5] = -0.999;
	DCBa1_points[6] = 36.000;
	DCBa1_points[7] = -0.999;
	DCBa1_points[8] = 42.000;
	DCBa1_points[9] = -0.999;
	DCBa1_points[10] = 48.027;
	DCBa1_points[11] = -0.993;
	DCBa1_points[12] = 60.000;
	DCBa1_points[13] = -0.995;
	DCBa1_points[14] = 72.335;
	DCBa1_points[15] = -0.960;
	DCBa1_points[16] = 78.412;
	DCBa1_points[17] = -0.924;
	DCBa1_points[18] = 84.329;
	DCBa1_points[19] = -0.850;
	DCBa1_points[20] = 87.688;
	DCBa1_points[21] = -0.770;
	DCBa1_points[22] = 91.000;
	DCBa1_points[23] = -0.700;
	DCBa1_points[24] = 92.000;
	DCBa1_points[25] = -0.910;
	DCBa1_points[26] = 96.783;
	DCBa1_points[27] = -0.850;
	DCBa1_points[28] = 99.000;
	DCBa1_points[29] = -0.800;
	DCBa1_points[30] = 100.000;
	DCBa1_points[31] = -0.850;
	DCBa1_points[32] = 104.634;
	DCBa1_points[33] = -0.700;
	DCBa1_points[34] = 107.518;
	DCBa1_points[35] = -0.500;
	
	secondStageAmpRatio_points[0] = 82.277;
	secondStageAmpRatio_points[1] = -18.508;
	secondStageAmpRatio_points[2] = 88.000;
	secondStageAmpRatio_points[3] = -30.000;
	secondStageAmpRatio_points[4] = 90.000;
	secondStageAmpRatio_points[5] = -30.000;
	secondStageAmpRatio_points[6] = 93.451;
	secondStageAmpRatio_points[7] = -30.488;
	secondStageAmpRatio_points[8] = 98.891;
	secondStageAmpRatio_points[9] = -30.633;
	secondStageAmpRatio_points[10] = 107.573;
	secondStageAmpRatio_points[11] = -30.633;

	r1_1db_points[0] = 100.000;
	r1_1db_points[1] = -75.000;
	r1_1db_points[2] = 103.802;
	r1_1db_points[3] = -237.513;
	r1_1db_points[4] = 108.000;
	r1_1db_points[5] = -400.000;

	r1_2db_points[0] = 98.388;
	r1_2db_points[1] = -16.562;
	r1_2db_points[2] = 100.743;
	r1_2db_points[3] = -75.531;
	r1_2db_points[4] = 103.242;
	r1_2db_points[5] = -154.156;
	r1_2db_points[6] = 108.000;
	r1_2db_points[7] = -300.000;

	
	r2db_points[0] = 100.000;
	r2db_points[1] = -115.898;
	r2db_points[2] = 107.858;
	r2db_points[3] = -250.000;
	
	r3db_points[0] = 100.000;
	r3db_points[1] = -150.000;
	r3db_points[2] = 108.000;
	r3db_points[3] = -400.000;

	secondPartialFactor_points[0] = 88.000;
	secondPartialFactor_points[1] = 2.000;
	secondPartialFactor_points[2] = 108.000;
	secondPartialFactor_points[3] = 2.100;

	thirdPartialFactor_points[0] = 88.000;
	thirdPartialFactor_points[1] = 3.100;
	thirdPartialFactor_points[2] = 108.000;
	thirdPartialFactor_points[3] = 3.100;

	bq4_gEarBalled_points[0] = 100.000;
	bq4_gEarBalled_points[1] = 0.040;
	bq4_gEarBalled_points[2] = 102.477;
	bq4_gEarBalled_points[3] = 0.100;
	bq4_gEarBalled_points[4] = 104.518;
	bq4_gEarBalled_points[5] = 0.300;
	bq4_gEarBalled_points[6] = 106.000;
	bq4_gEarBalled_points[7] = 0.500;
	bq4_gEarBalled_points[8] = 107.000;
	bq4_gEarBalled_points[9] = 1.000;
	bq4_gEarBalled_points[10] = 108.000;
	bq4_gEarBalled_points[11] = 1.500;

	bq4_gEarBalled.set(&bq4_gEarBalled_points[0], 6);
	thirdPartialFactor.set(&thirdPartialFactor_points[0], 2);
	secondPartialFactor.set(&secondPartialFactor_points[0], 2);
	r3db.set(&r3db_points[0], 2);
	r2db.set(&r2db_points[0], 2);
	r1_2db.set(&r1_2db_points[0], 4);
	r1_1db.set(&r1_1db_points[0], 3);
	secondStageAmpRatio.set(&secondStageAmpRatio_points[0], 6);
	DCBa1.set(&DCBa1_points[0], 18);
	DryTapAmpT60.set(&DryTapAmpT60_points[0], 16);
	sustainPedalLevel.set(&sustainPedalLevel_points[0], 13);
	softGain.set(&softGain_points[0], 15);
	loudGain.set(&loudGain_points[0], 16);
	normalizedVelocity.set(&normalizedVelocity_points[0], 8);
	softPole.set(&softPole_points[0], 16);
	loudPole.set(&loudPole_points[0], 19);
	EQBandwidthFactor.set(&EQBandwidthFactor_points[0], 13);
	EQGain.set(&EQGain_points[0], 14);
	strikePosition.set(&strikePosition_points[0], 12);
	stiffnessCoefficient.set(&stiffnessCoefficient_points[0], 10);
	detuningHz.set(&detuningHz_points[0], 18);
	releaseLoopGain.set(&releaseLoopGain_points[0], 10);
	singleStringPole.set(&singleStringPole_points[0], 17);
	singleStringZero.set(&singleStringZero_points[0], 17);
	singleStringDecayRate.set(&singleStringDecayRate_points[0], 17);
	noteOffDelayTime.set(&noteOffDelayTime_points[0], 5);

	fSamplingFreq = d_getSampleRate();
	pfreq = 4.4e+02f;
	iConst0 = min(192000, max(1, fSamplingFreq));
	fConst1 = (0.05f / float(iConst0));
	pbright = 0.0f;
	pgate = 0.0;
	phammer = 0.1f;
	fConst2 = float(iConst0);
	fConst3 = (0.1f * fConst2);
	for (int i=0; i<2; i++) fRec11[i] = 0;
	fConst4 = expf((0 - (5.0f / fConst2)));
	fConst5 = expf((0 - (0.5f / fConst2)));
	fConst6 = (1e+01f / fConst2);
	for (int i=0; i<2; i++) fRec10[i] = 0;
	pgain = 1.0f;
	fConst7 = (float(7) / fConst2);
	for (int i=0; i<2; i++) fRec12[i] = 0;
	for (int i=0; i<2; i++) iRec13[i] = 0;
	for (int i=0; i<2; i++) fVec0[i] = 0;
	for (int i=0; i<2; i++) fVec1[i] = 0;
	for (int i=0; i<2; i++) fRec9[i] = 0;
	for (int i=0; i<2; i++) fRec8[i] = 0;
	for (int i=0; i<2; i++) fRec7[i] = 0;
	for (int i=0; i<2; i++) fRec6[i] = 0;
	for (int i=0; i<2; i++) fRec5[i] = 0;
	fConst8 = (6.283185307179586f / float(iConst0));
	for (int i=0; i<3; i++) fRec4[i] = 0;
	for (int i=0; i<3; i++) fRec3[i] = 0;
	for (int i=0; i<3; i++) fRec2[i] = 0;
	for (int i=0; i<3; i++) fRec1[i] = 0;
	for (int i=0; i<2; i++) fRec0[i] = 0;
	for (int i=0; i<2; i++) fRec24[i] = 0;
	for (int i=0; i<2; i++) fRec23[i] = 0;
	for (int i=0; i<2; i++) fRec22[i] = 0;
	for (int i=0; i<2; i++) fRec21[i] = 0;
	for (int i=0; i<2; i++) fRec20[i] = 0;
	for (int i=0; i<2; i++) fRec25[i] = 0;
	for (int i=0; i<2; i++) fVec2[i] = 0;
	pstiff = 0.28f;
	for (int i=0; i<2; i++) fRec19[i] = 0;
	for (int i=0; i<2; i++) fRec18[i] = 0;
	IOTA = 0;
	for (int i=0; i<4096; i++) fRec17[i] = 0;
	pdetune = 0.1f;
	fConst9 = (0.15915494309189535f * iConst0);
	for (int i=0; i<2; i++) fVec3[i] = 0;
	for (int i=0; i<2; i++) fRec29[i] = 0;
	for (int i=0; i<2; i++) fRec28[i] = 0;
	for (int i=0; i<4096; i++) fRec27[i] = 0;
	for (int i=0; i<2; i++) fVec4[i] = 0;
	for (int i=0; i<2; i++) fRec26[i] = 0;
	for (int i=0; i<2; i++) fRec14[i] = 0;
	for (int i=0; i<2; i++) fRec15[i] = 0;
	iConst10 = faustpower<2>(iConst0);
	fConst11 = (1.0f / float(iConst10));
	fConst12 = (2.0f / float(iConst0));
	for (int i=0; i<3; i++) fRec30[i] = 0;
	fConst13 = (0.5f / float(iConst10));
	for (int i=0; i<4096; i++) fVec5[i] = 0;
	prevgain = 0.137f;
	for (int i=0; i<2; i++) fRec31[i] = 0;
	ppanangle = 0.6f;
	fConst14 = (1.0f / tanf((628.3185307179587f / float(iConst0))));
	fConst15 = (1 + fConst14);
	fConst16 = (1.0f / fConst15);
	fConst17 = (0 - ((1 - fConst14) / fConst15));
	for (int i=0; i<2; i++) fRec43[i] = 0;
	prevroom = 0.72f;
	fConst18 = floorf((0.5f + (0.174713f * iConst0)));
	fConst19 = ((0 - (6.907755278982138f * fConst18)) / float(iConst0));
	fConst20 = (0.5f * fConst19);
	fConst21 = (0.3333333333333333f * fConst19);
	fConst22 = cosf((37699.11184307752f / fConst2));
	for (int i=0; i<2; i++) fRec42[i] = 0;
	for (int i=0; i<8192; i++) fVec6[i] = 0;
	fConst23 = floorf((0.5f + (0.022904f * iConst0)));
	iConst24 = int((int((fConst18 - fConst23)) & 8191));
	for (int i=0; i<4096; i++) fVec7[i] = 0;
	iConst25 = int((int((0.02f * iConst0)) & 8191));
	for (int i=0; i<2048; i++) fVec8[i] = 0;
	iConst26 = int((int((fConst23 - 1)) & 2047));
	for (int i=0; i<2; i++) fRec40[i] = 0;
	for (int i=0; i<2; i++) fRec47[i] = 0;
	fConst27 = floorf((0.5f + (0.153129f * iConst0)));
	fConst28 = ((0 - (6.907755278982138f * fConst27)) / float(iConst0));
	fConst29 = (0.5f * fConst28);
	fConst30 = (0.3333333333333333f * fConst28);
	for (int i=0; i<2; i++) fRec46[i] = 0;
	for (int i=0; i<8192; i++) fVec9[i] = 0;
	fConst31 = floorf((0.5f + (0.020346f * iConst0)));
	iConst32 = int((int((fConst27 - fConst31)) & 8191));
	for (int i=0; i<1024; i++) fVec10[i] = 0;
	iConst33 = int((int((fConst31 - 1)) & 1023));
	for (int i=0; i<2; i++) fRec44[i] = 0;
	for (int i=0; i<2; i++) fRec51[i] = 0;
	fConst34 = floorf((0.5f + (0.127837f * iConst0)));
	fConst35 = ((0 - (6.907755278982138f * fConst34)) / float(iConst0));
	fConst36 = (0.5f * fConst35);
	fConst37 = (0.3333333333333333f * fConst35);
	for (int i=0; i<2; i++) fRec50[i] = 0;
	for (int i=0; i<8192; i++) fVec11[i] = 0;
	fConst38 = floorf((0.5f + (0.031604f * iConst0)));
	iConst39 = int((int((fConst34 - fConst38)) & 8191));
	for (int i=0; i<2048; i++) fVec12[i] = 0;
	iConst40 = int((int((fConst38 - 1)) & 2047));
	for (int i=0; i<2; i++) fRec48[i] = 0;
	for (int i=0; i<2; i++) fRec55[i] = 0;
	fConst41 = floorf((0.5f + (0.125f * iConst0)));
	fConst42 = ((0 - (6.907755278982138f * fConst41)) / float(iConst0));
	fConst43 = (0.5f * fConst42);
	fConst44 = (0.3333333333333333f * fConst42);
	for (int i=0; i<2; i++) fRec54[i] = 0;
	for (int i=0; i<8192; i++) fVec13[i] = 0;
	fConst45 = floorf((0.5f + (0.013458f * iConst0)));
	iConst46 = int((int((fConst41 - fConst45)) & 8191));
	for (int i=0; i<1024; i++) fVec14[i] = 0;
	iConst47 = int((int((fConst45 - 1)) & 1023));
	for (int i=0; i<2; i++) fRec52[i] = 0;
	for (int i=0; i<2; i++) fRec59[i] = 0;
	fConst48 = floorf((0.5f + (0.210389f * iConst0)));
	fConst49 = ((0 - (6.907755278982138f * fConst48)) / float(iConst0));
	fConst50 = (0.5f * fConst49);
	fConst51 = (0.3333333333333333f * fConst49);
	for (int i=0; i<2; i++) fRec58[i] = 0;
	for (int i=0; i<16384; i++) fVec15[i] = 0;
	fConst52 = floorf((0.5f + (0.024421f * iConst0)));
	iConst53 = int((int((fConst48 - fConst52)) & 16383));
	pspatialwidth = 0.5f;
	fConst54 = (0.5f * iConst0);
	for (int i=0; i<4096; i++) fVec16[i] = 0;
	for (int i=0; i<2048; i++) fVec17[i] = 0;
	iConst55 = int((int((fConst52 - 1)) & 2047));
	for (int i=0; i<2; i++) fRec56[i] = 0;
	for (int i=0; i<2; i++) fRec63[i] = 0;
	fConst56 = floorf((0.5f + (0.192303f * iConst0)));
	fConst57 = ((0 - (6.907755278982138f * fConst56)) / float(iConst0));
	fConst58 = (0.5f * fConst57);
	fConst59 = (0.3333333333333333f * fConst57);
	for (int i=0; i<2; i++) fRec62[i] = 0;
	for (int i=0; i<8192; i++) fVec18[i] = 0;
	fConst60 = floorf((0.5f + (0.029291f * iConst0)));
	iConst61 = int((int((fConst56 - fConst60)) & 8191));
	for (int i=0; i<2048; i++) fVec19[i] = 0;
	iConst62 = int((int((fConst60 - 1)) & 2047));
	for (int i=0; i<2; i++) fRec60[i] = 0;
	for (int i=0; i<2; i++) fRec67[i] = 0;
	fConst63 = floorf((0.5f + (0.256891f * iConst0)));
	fConst64 = ((0 - (6.907755278982138f * fConst63)) / float(iConst0));
	fConst65 = (0.5f * fConst64);
	fConst66 = (0.3333333333333333f * fConst64);
	for (int i=0; i<2; i++) fRec66[i] = 0;
	for (int i=0; i<16384; i++) fVec20[i] = 0;
	fConst67 = floorf((0.5f + (0.027333f * iConst0)));
	iConst68 = int((int((fConst63 - fConst67)) & 16383));
	for (int i=0; i<2048; i++) fVec21[i] = 0;
	iConst69 = int((int((fConst67 - 1)) & 2047));
	for (int i=0; i<2; i++) fRec64[i] = 0;
	for (int i=0; i<2; i++) fRec71[i] = 0;
	fConst70 = floorf((0.5f + (0.219991f * iConst0)));
	fConst71 = ((0 - (6.907755278982138f * fConst70)) / float(iConst0));
	fConst72 = (0.5f * fConst71);
	fConst73 = (0.3333333333333333f * fConst71);
	for (int i=0; i<2; i++) fRec70[i] = 0;
	for (int i=0; i<16384; i++) fVec22[i] = 0;
	fConst74 = floorf((0.5f + (0.019123f * iConst0)));
	iConst75 = int((int((fConst70 - fConst74)) & 16383));
	for (int i=0; i<1024; i++) fVec23[i] = 0;
	iConst76 = int((int((fConst74 - 1)) & 1023));
	for (int i=0; i<2; i++) fRec68[i] = 0;
	for (int i=0; i<3; i++) fRec32[i] = 0;
	for (int i=0; i<3; i++) fRec33[i] = 0;
	for (int i=0; i<3; i++) fRec34[i] = 0;
	for (int i=0; i<3; i++) fRec35[i] = 0;
	for (int i=0; i<3; i++) fRec36[i] = 0;
	for (int i=0; i<3; i++) fRec37[i] = 0;
	for (int i=0; i<3; i++) fRec38[i] = 0;
	for (int i=0; i<3; i++) fRec39[i] = 0;
}

void ZamPianoPlugin::compute (uint32_t count, const float**, float** output) {
		float 	fSlow0 = float(pfreq);
		int 	iSlow1 = int(((17.31234049066756f * (logf(fSlow0) - 6.0867747269123065f)) + 69.5f));
		float 	fSlow2 = getValueDCBa1(iSlow1);
		float 	fSlow3 = powf(10,(fConst1 * getValuer1_2db(iSlow1)));
		float 	fSlow4 = faustpower<2>(fSlow3);
		float 	fSlow5 = getValueLoudPole(iSlow1);
		float 	fSlow6 = (0.25f * float(pbright));
		float 	fSlow7 = (fSlow6 - (0.02f + fSlow5));
		float 	fSlow8 = float(pgate);
		int 	iSlow9 = (fSlow8 > 0);
		float 	fSlow10 = float(phammer);
		float 	fSlow11 = (fConst3 * fSlow10);
		float 	fSlow12 = (0 - (fSlow8 - 1));
		float 	fSlow13 = (fConst4 * fSlow12);
		float 	fSlow14 = (fSlow13 - 1);
		float 	fSlow15 = expf((0 - (fConst6 / fSlow10)));
		float 	fSlow16 = (0.2f * getValueSustainPedalLevel(iSlow1));
		int 	iSlow17 = (fSlow8 < 1);
		float 	fSlow18 = expf((0 - (fConst7 / (float(pgain) * getValueDryTapAmpT60(iSlow1)))));
		int 	iSlow19 = (iSlow1 >= 88);
		float 	fSlow20 = (1.1641532188114492e-10f * iSlow19);
		float 	fSlow21 = (2.3283064376228985e-10f * iSlow19);
		float 	fSlow22 = (1 - fSlow2);
		float 	fSlow23 = (0.5f * fSlow22);
		float 	fSlow24 = (0 - fSlow23);
		float 	fSlow25 = (((fSlow6 + 0.98f) - fSlow5) * getValueLoudGain(iSlow1));
		float 	fSlow26 = (2.0f * getValueBq4_gEarBalled(iSlow1));
		float 	fSlow27 = powf(10,(fConst1 * getValuer3db(iSlow1)));
		float 	fSlow28 = faustpower<2>(fSlow27);
		float 	fSlow29 = ((0 - (2 * fSlow27)) * cosf((fConst8 * (fSlow0 * getValueThirdPartialFactor(iSlow1)))));
		float 	fSlow30 = powf(10,(fConst1 * getValuer2db(iSlow1)));
		float 	fSlow31 = faustpower<2>(fSlow30);
		float 	fSlow32 = ((0 - (2 * fSlow30)) * cosf((fConst8 * (fSlow0 * getValueSecondPartialFactor(iSlow1)))));
		float 	fSlow33 = powf(10,(fConst1 * getValuer1_1db(iSlow1)));
		float 	fSlow34 = faustpower<2>(fSlow33);
		float 	fSlow35 = cosf((fConst8 * fSlow0));
		float 	fSlow36 = (fSlow35 * (0 - (2 * fSlow33)));
		float 	fSlow37 = powf(10,(0.05f * getValueSecondStageAmpRatio(iSlow1)));
		float 	fSlow38 = (1 - fSlow37);
		float 	fSlow39 = ((fSlow37 * fSlow34) + (fSlow4 * fSlow38));
		float 	fSlow40 = (0 - (2 * fSlow3));
		float 	fSlow41 = (0 - (2 * ((fSlow37 * fSlow33) + (fSlow3 * fSlow38))));
		float 	fSlow42 = (1.396983862573739e-09f * (fSlow25 * (iSlow1 < 88)));
		float 	fSlow43 = (0.0010000000000000009f * ((0.9996f * fSlow8) + (0.9f * (fSlow12 * getValueReleaseLoopGain(iSlow1)))));
		float 	fSlow44 = getValueStiffnessCoefficient(iSlow1);
		float 	fSlow45 = float(pstiff);
		float 	fSlow46 = (fSlow45 * fSlow44);
		float 	fSlow47 = (5.0f * (float(pdetune) * getValueDetuningHz(iSlow1)));
		float 	fSlow48 = (fSlow0 + fSlow47);
		float 	fSlow49 = getValueSingleStringPole(iSlow1);
		float 	fSlow50 = (powf(10,(0.05f * (getValueSingleStringDecayRate(iSlow1) / fSlow0))) * (1 - fSlow49));
		float 	fSlow51 = getValueSingleStringZero(iSlow1);
		float 	fSlow52 = (1 - fSlow51);
		float 	fSlow53 = ((3 * fSlow52) - fSlow50);
		float 	fSlow54 = (fSlow49 * fSlow52);
		float 	fSlow55 = (3 * fSlow54);
		float 	fSlow56 = (fSlow50 * fSlow51);
		float 	fSlow57 = (fSlow54 - fSlow56);
		float 	fSlow58 = (4 * fSlow57);
		float 	fSlow59 = ((fSlow56 + fSlow58) - fSlow55);
		float 	fSlow60 = (fConst8 * fSlow48);
		float 	fSlow61 = cosf(fSlow60);
		float 	fSlow62 = ((fSlow50 + fSlow51) - 1);
		float 	fSlow63 = (4 * fSlow62);
		float 	fSlow64 = (1 + ((fSlow63 + (fSlow61 * fSlow59)) / fSlow53));
		float 	fSlow65 = (fSlow56 - fSlow55);
		float 	fSlow66 = (0 - (fSlow59 / fSlow53));
		float 	fSlow67 = (1 + ((fSlow65 * fSlow61) / fSlow53));
		float 	fSlow68 = sinf(fSlow60);
		float 	fSlow69 = faustpower<2>(fSlow53);
		float 	fSlow70 = faustpower<2>(fSlow68);
		float 	fSlow71 = (13.690000000000001f * (faustpower<2>(fSlow45) * faustpower<2>(fSlow44)));
		float 	fSlow72 = (fSlow71 - 1.0f);
		float 	fSlow73 = (1.0f + fSlow71);
		float 	fSlow74 = (7.4f * fSlow46);
		float 	fSlow75 = (3 * atan2f((fSlow72 * fSlow68),(fSlow74 + (fSlow73 * fSlow61))));
		int 	iSlow76 = int((fConst9 * ((6.283185307179586f + (fSlow75 + atan2f((fSlow68 * ((fSlow67 * fSlow66) + ((fSlow65 * fSlow64) / fSlow53))),((fSlow67 * fSlow64) + (((fSlow65 * fSlow70) * fSlow59) / fSlow69))))) / fSlow48)));
		int 	iSlow77 = int((iSlow76 & 4095));
		float 	fSlow78 = (fSlow65 + fSlow58);
		float 	fSlow79 = (1 + ((fSlow63 + (fSlow78 * fSlow61)) / fSlow53));
		float 	fSlow80 = (0 - (fSlow78 / fSlow53));
		float 	fSlow81 = (fSlow65 * fSlow78);
		float 	fSlow82 = (fConst9 * ((6.283185307179586f + (fSlow75 + atan2f((fSlow68 * ((fSlow80 * fSlow67) + ((fSlow65 * fSlow79) / fSlow53))),((fSlow67 * fSlow79) + ((fSlow81 * fSlow70) / fSlow69))))) / fSlow48));
		int 	iSlow83 = int(fSlow82);
		float 	fSlow84 = ((1 + iSlow83) - fSlow82);
		float 	fSlow85 = (fSlow0 - fSlow47);
		float 	fSlow86 = (fConst8 * fSlow85);
		float 	fSlow87 = cosf(fSlow86);
		float 	fSlow88 = (1 + ((fSlow63 + (fSlow87 * fSlow59)) / fSlow53));
		float 	fSlow89 = (1 + ((fSlow65 * fSlow87) / fSlow53));
		float 	fSlow90 = sinf(fSlow86);
		float 	fSlow91 = faustpower<2>(fSlow90);
		float 	fSlow92 = (3 * atan2f((fSlow72 * fSlow90),(fSlow74 + (fSlow73 * fSlow87))));
		int 	iSlow93 = int((fConst9 * ((6.283185307179586f + (fSlow92 + atan2f((fSlow90 * ((fSlow89 * fSlow66) + ((fSlow65 * fSlow88) / fSlow53))),((fSlow89 * fSlow88) + (((fSlow65 * fSlow91) * fSlow59) / fSlow69))))) / fSlow85)));
		int 	iSlow94 = int((int((1 + iSlow93)) & 4095));
		float 	fSlow95 = (1 + (((fSlow78 * fSlow87) + fSlow63) / fSlow53));
		float 	fSlow96 = (fConst9 * ((6.283185307179586f + (fSlow92 + atan2f((fSlow90 * ((fSlow80 * fSlow89) + ((fSlow65 * fSlow95) / fSlow53))),((fSlow89 * fSlow95) + ((fSlow81 * fSlow91) / fSlow69))))) / fSlow85));
		int 	iSlow97 = int(fSlow96);
		float 	fSlow98 = (fSlow96 - iSlow97);
		int 	iSlow99 = int((iSlow93 & 4095));
		float 	fSlow100 = ((1 + iSlow97) - fSlow96);
		int 	iSlow101 = int((int((1 + iSlow76)) & 4095));
		float 	fSlow102 = (fSlow82 - iSlow83);
		float 	fSlow103 = (1.0f / fSlow53);
		float 	fSlow104 = getValueEQBandWidthFactor(iSlow1);
		float 	fSlow105 = (faustpower<2>(fSlow0) * faustpower<2>(fSlow104));
		float 	fSlow106 = (fConst11 * fSlow105);
		float 	fSlow107 = ((0 - (fConst12 * (fSlow0 * fSlow104))) * cosf((fConst8 * (fSlow0 / getValueStrikePosition(iSlow1)))));
		float 	fSlow108 = getValueEQGain(iSlow1);
		float 	fSlow109 = (fConst13 * fSlow105);
		float 	fSlow110 = (0.5f - fSlow109);
		float 	fSlow111 = (fSlow109 - 0.5f);
		float 	fSlow112 = (0.0010000000000000009f * float(prevgain));
		float 	fSlow113 = float(ppanangle);
		float 	fSlow114 = (12 * (1.0f - fSlow113));
		float 	fSlow115 = float(prevroom);
		float 	fSlow116 = expf((fConst20 / fSlow115));
		float 	fSlow117 = ((expf((fConst21 / fSlow115)) / fSlow116) - 1);
		float 	fSlow118 = faustpower<2>(fSlow116);
		float 	fSlow119 = (1.0f - fSlow118);
		float 	fSlow120 = (1.0f - (fConst22 * fSlow118));
		float 	fSlow121 = (fSlow120 / fSlow119);
		float 	fSlow122 = sqrtf(max((float)0, ((faustpower<2>(fSlow120) / faustpower<2>(fSlow119)) - 1.0f)));
		float 	fSlow123 = (fSlow116 * ((1.0f + fSlow122) - fSlow121));
		float 	fSlow124 = (fSlow121 - fSlow122);
		float 	fSlow125 = expf((fConst29 / fSlow115));
		float 	fSlow126 = ((expf((fConst30 / fSlow115)) / fSlow125) - 1);
		float 	fSlow127 = faustpower<2>(fSlow125);
		float 	fSlow128 = (1.0f - fSlow127);
		float 	fSlow129 = (1.0f - (fConst22 * fSlow127));
		float 	fSlow130 = (fSlow129 / fSlow128);
		float 	fSlow131 = sqrtf(max((float)0, ((faustpower<2>(fSlow129) / faustpower<2>(fSlow128)) - 1.0f)));
		float 	fSlow132 = (fSlow125 * ((1.0f + fSlow131) - fSlow130));
		float 	fSlow133 = (fSlow130 - fSlow131);
		float 	fSlow134 = expf((fConst36 / fSlow115));
		float 	fSlow135 = ((expf((fConst37 / fSlow115)) / fSlow134) - 1);
		float 	fSlow136 = faustpower<2>(fSlow134);
		float 	fSlow137 = (1.0f - fSlow136);
		float 	fSlow138 = (1.0f - (fConst22 * fSlow136));
		float 	fSlow139 = (fSlow138 / fSlow137);
		float 	fSlow140 = sqrtf(max((float)0, ((faustpower<2>(fSlow138) / faustpower<2>(fSlow137)) - 1.0f)));
		float 	fSlow141 = (fSlow134 * ((1.0f + fSlow140) - fSlow139));
		float 	fSlow142 = (fSlow139 - fSlow140);
		float 	fSlow143 = expf((fConst43 / fSlow115));
		float 	fSlow144 = ((expf((fConst44 / fSlow115)) / fSlow143) - 1);
		float 	fSlow145 = faustpower<2>(fSlow143);
		float 	fSlow146 = (1.0f - fSlow145);
		float 	fSlow147 = (1.0f - (fConst22 * fSlow145));
		float 	fSlow148 = (fSlow147 / fSlow146);
		float 	fSlow149 = sqrtf(max((float)0, ((faustpower<2>(fSlow147) / faustpower<2>(fSlow146)) - 1.0f)));
		float 	fSlow150 = (fSlow143 * ((1.0f + fSlow149) - fSlow148));
		float 	fSlow151 = (fSlow148 - fSlow149);
		float 	fSlow152 = expf((fConst50 / fSlow115));
		float 	fSlow153 = ((expf((fConst51 / fSlow115)) / fSlow152) - 1);
		float 	fSlow154 = faustpower<2>(fSlow152);
		float 	fSlow155 = (1.0f - fSlow154);
		float 	fSlow156 = (1.0f - (fConst22 * fSlow154));
		float 	fSlow157 = (fSlow156 / fSlow155);
		float 	fSlow158 = sqrtf(max((float)0, ((faustpower<2>(fSlow156) / faustpower<2>(fSlow155)) - 1.0f)));
		float 	fSlow159 = (fSlow152 * ((1.0f + fSlow158) - fSlow157));
		float 	fSlow160 = (fSlow157 - fSlow158);
		int 	iSlow161 = int((int((fConst54 * (float(pspatialwidth) / fSlow0))) & 4095));
		float 	fSlow162 = (12 * fSlow113);
		float 	fSlow163 = expf((fConst58 / fSlow115));
		float 	fSlow164 = ((expf((fConst59 / fSlow115)) / fSlow163) - 1);
		float 	fSlow165 = faustpower<2>(fSlow163);
		float 	fSlow166 = (1.0f - fSlow165);
		float 	fSlow167 = (1.0f - (fConst22 * fSlow165));
		float 	fSlow168 = (fSlow167 / fSlow166);
		float 	fSlow169 = sqrtf(max((float)0, ((faustpower<2>(fSlow167) / faustpower<2>(fSlow166)) - 1.0f)));
		float 	fSlow170 = (fSlow163 * ((1.0f + fSlow169) - fSlow168));
		float 	fSlow171 = (fSlow168 - fSlow169);
		float 	fSlow172 = expf((fConst65 / fSlow115));
		float 	fSlow173 = ((expf((fConst66 / fSlow115)) / fSlow172) - 1);
		float 	fSlow174 = faustpower<2>(fSlow172);
		float 	fSlow175 = (1.0f - fSlow174);
		float 	fSlow176 = (1.0f - (fConst22 * fSlow174));
		float 	fSlow177 = (fSlow176 / fSlow175);
		float 	fSlow178 = sqrtf(max((float)0, ((faustpower<2>(fSlow176) / faustpower<2>(fSlow175)) - 1.0f)));
		float 	fSlow179 = (fSlow172 * ((1.0f + fSlow178) - fSlow177));
		float 	fSlow180 = (fSlow177 - fSlow178);
		float 	fSlow181 = expf((fConst72 / fSlow115));
		float 	fSlow182 = ((expf((fConst73 / fSlow115)) / fSlow181) - 1);
		float 	fSlow183 = faustpower<2>(fSlow181);
		float 	fSlow184 = (1.0f - fSlow183);
		float 	fSlow185 = (1.0f - (fConst22 * fSlow183));
		float 	fSlow186 = (fSlow185 / fSlow184);
		float 	fSlow187 = sqrtf(max((float)0, ((faustpower<2>(fSlow185) / faustpower<2>(fSlow184)) - 1.0f)));
		float 	fSlow188 = (fSlow181 * ((1.0f + fSlow187) - fSlow186));
		float 	fSlow189 = (fSlow186 - fSlow187);
		FAUSTFLOAT* output0 = output[0];
		FAUSTFLOAT* output1 = output[1];
		for (uint32_t i=0; i<count; i++) {
			fRec11[0] = (1 + (fSlow8 * fRec11[1]));
			float fTemp0 = (fRec11[0] - 1);
			int iTemp1 = (fTemp0 < fSlow11);
			float fTemp2 = (fSlow8 * ((fSlow15 * iTemp1) + (fConst5 * (fTemp0 >= fSlow11))));
			fRec10[0] = ((fRec10[1] * (fTemp2 + fSlow13)) + (fSlow16 * ((0 - (fTemp2 + fSlow14)) * (iTemp1 & iSlow9))));
			int iTemp3 = ((fTemp0 < 2.0f) & iSlow9);
			float fTemp4 = ((0.0301973834223185f * iTemp3) + (fSlow18 * ((fTemp0 >= 2.0f) | iSlow17)));
			fRec12[0] = ((fRec12[1] * fTemp4) + (0.15f * (iTemp3 * (1 - fTemp4))));
			iRec13[0] = (12345 + (1103515245 * iRec13[1]));
			float fTemp5 = (iRec13[0] * (fRec12[0] + fRec10[0]));
			fVec0[0] = (fSlow21 * fTemp5);
			float fTemp6 = (0 - ((0.5f * fVec0[1]) + (fSlow20 * fTemp5)));
			fVec1[0] = fTemp6;
			fRec9[0] = (((fSlow23 * fVec1[0]) + (fSlow24 * fVec1[1])) - (fSlow2 * fRec9[1]));
			fRec8[0] = ((fSlow25 * fRec9[0]) - (fSlow7 * fRec8[1]));
			fRec7[0] = ((fSlow25 * fRec8[0]) - (fSlow7 * fRec7[1]));
			fRec6[0] = ((fSlow25 * fRec7[0]) - (fSlow7 * fRec6[1]));
			fRec5[0] = ((fSlow25 * fRec6[0]) - (fSlow7 * fRec5[1]));
			fRec4[0] = (0 - (((fSlow29 * fRec4[1]) + (fSlow28 * fRec4[2])) - (fSlow26 * ((0.5f * fRec5[0]) - (0.5f * fRec5[1])))));
			fRec3[0] = (0 - (((fSlow32 * fRec3[1]) + (fSlow31 * fRec3[2])) - (fSlow26 * fRec4[0])));
			fRec2[0] = (0 - (((fSlow36 * fRec2[1]) + (fSlow34 * fRec2[2])) - fRec3[0]));
			fRec1[0] = (((fSlow35 * ((fSlow41 * fRec2[1]) - (fSlow40 * fRec1[1]))) + (fRec2[0] + (fSlow39 * fRec2[2]))) - (fSlow4 * fRec1[2]));
			fRec0[0] = ((fSlow22 * fRec1[0]) - (fSlow2 * fRec0[1]));
			fRec24[0] = ((fSlow42 * fTemp5) - (fSlow7 * fRec24[1]));
			fRec23[0] = ((fSlow25 * fRec24[0]) - (fSlow7 * fRec23[1]));
			fRec22[0] = ((fSlow25 * fRec23[0]) - (fSlow7 * fRec22[1]));
			fRec21[0] = ((fSlow25 * fRec22[0]) - (fSlow7 * fRec21[1]));
			fRec20[0] = (((fSlow24 * fRec21[1]) + (fSlow23 * fRec21[0])) - (fSlow2 * fRec20[1]));
			fRec25[0] = ((0.999f * fRec25[1]) + fSlow43);
			float fTemp7 = (fRec25[0] * (fRec20[0] + fRec14[1]));
			fVec2[0] = fTemp7;
			fRec19[0] = (fVec2[1] + (fSlow46 * ((3.7f * fVec2[0]) - (3.7f * fRec19[1]))));
			fRec18[0] = (fRec19[1] + (fSlow46 * ((3.7f * fRec19[0]) - (3.7f * fRec18[1]))));
			fRec17[IOTA&4095] = (fRec18[1] + (fSlow46 * ((3.7f * fRec18[0]) - (3.7f * fRec17[(IOTA-1)&4095]))));
			float fTemp8 = (fSlow84 * fRec17[(IOTA-iSlow77)&4095]);
			float fTemp9 = (fRec20[0] + (fRec25[0] * fRec15[1]));
			fVec3[0] = fTemp9;
			fRec29[0] = (fVec3[1] + (fSlow46 * ((3.7f * fVec3[0]) - (3.7f * fRec29[1]))));
			fRec28[0] = (fRec29[1] + (fSlow46 * ((3.7f * fRec29[0]) - (3.7f * fRec28[1]))));
			fRec27[IOTA&4095] = (fRec28[1] + (fSlow46 * ((3.7f * fRec28[0]) - (3.7f * fRec27[(IOTA-1)&4095]))));
			float fTemp10 = (fSlow98 * fRec27[(IOTA-iSlow94)&4095]);
			float fTemp11 = (fSlow100 * fRec27[(IOTA-iSlow99)&4095]);
			float fTemp12 = (fSlow102 * fRec17[(IOTA-iSlow101)&4095]);
			float fTemp13 = (fTemp12 + ((fTemp11 + fTemp10) + fTemp8));
			fVec4[0] = fTemp13;
			fRec26[0] = (fSlow103 * ((2 * ((fSlow62 * fVec4[0]) + (fSlow57 * fVec4[1]))) - (fSlow65 * fRec26[1])));
			fRec14[0] = (fTemp12 + (fRec26[0] + fTemp8));
			fRec15[0] = (fTemp10 + (fRec26[0] + fTemp11));
			float 	fRec16 = fVec4[0];
			fRec30[0] = ((fSlow108 * fRec16) - ((fSlow107 * fRec30[1]) + (fSlow106 * fRec30[2])));
			float fTemp14 = ((fSlow111 * fRec30[2]) + ((fSlow110 * fRec30[0]) + (fRec16 + fRec0[0])));
			fVec5[IOTA&4095] = fTemp14;
			fRec31[0] = ((0.999f * fRec31[1]) + fSlow112);
			float fTemp15 = (1 - fRec31[0]);
			fRec43[0] = ((fConst17 * fRec43[1]) + (fConst16 * (fRec36[1] + fRec36[2])));
			fRec42[0] = ((fSlow124 * fRec42[1]) + (fSlow123 * (fRec36[1] + (fSlow117 * fRec43[0]))));
			fVec6[IOTA&8191] = (1e-20f + (0.35355339059327373f * fRec42[0]));
			fVec7[IOTA&4095] = (fSlow114 * (fRec31[0] * fVec5[IOTA&4095]));
			float fTemp16 = (0.3f * fVec7[(IOTA-iConst25)&4095]);
			float fTemp17 = ((fTemp16 + fVec6[(IOTA-iConst24)&8191]) - (0.6f * fRec40[1]));
			fVec8[IOTA&2047] = fTemp17;
			fRec40[0] = fVec8[(IOTA-iConst26)&2047];
			float 	fRec41 = (0.6f * fVec8[IOTA&2047]);
			fRec47[0] = ((fConst17 * fRec47[1]) + (fConst16 * (fRec32[1] + fRec32[2])));
			fRec46[0] = ((fSlow133 * fRec46[1]) + (fSlow132 * (fRec32[1] + (fSlow126 * fRec47[0]))));
			fVec9[IOTA&8191] = (1e-20f + (0.35355339059327373f * fRec46[0]));
			float fTemp18 = ((fVec9[(IOTA-iConst32)&8191] + fTemp16) - (0.6f * fRec44[1]));
			fVec10[IOTA&1023] = fTemp18;
			fRec44[0] = fVec10[(IOTA-iConst33)&1023];
			float 	fRec45 = (0.6f * fVec10[IOTA&1023]);
			float fTemp19 = (fRec45 + fRec41);
			fRec51[0] = ((fConst17 * fRec51[1]) + (fConst16 * (fRec34[1] + fRec34[2])));
			fRec50[0] = ((fSlow142 * fRec50[1]) + (fSlow141 * (fRec34[1] + (fSlow135 * fRec51[0]))));
			fVec11[IOTA&8191] = (1e-20f + (0.35355339059327373f * fRec50[0]));
			float fTemp20 = (fVec11[(IOTA-iConst39)&8191] - (fTemp16 + (0.6f * fRec48[1])));
			fVec12[IOTA&2047] = fTemp20;
			fRec48[0] = fVec12[(IOTA-iConst40)&2047];
			float 	fRec49 = (0.6f * fVec12[IOTA&2047]);
			fRec55[0] = ((fConst17 * fRec55[1]) + (fConst16 * (fRec38[1] + fRec38[2])));
			fRec54[0] = ((fSlow151 * fRec54[1]) + (fSlow150 * (fRec38[1] + (fSlow144 * fRec55[0]))));
			fVec13[IOTA&8191] = (1e-20f + (0.35355339059327373f * fRec54[0]));
			float fTemp21 = (fVec13[(IOTA-iConst46)&8191] - (fTemp16 + (0.6f * fRec52[1])));
			fVec14[IOTA&1023] = fTemp21;
			fRec52[0] = fVec14[(IOTA-iConst47)&1023];
			float 	fRec53 = (0.6f * fVec14[IOTA&1023]);
			float fTemp22 = (fRec53 + (fRec49 + fTemp19));
			fRec59[0] = ((fConst17 * fRec59[1]) + (fConst16 * (fRec33[1] + fRec33[2])));
			fRec58[0] = ((fSlow160 * fRec58[1]) + (fSlow159 * (fRec33[1] + (fSlow153 * fRec59[0]))));
			fVec15[IOTA&16383] = (1e-20f + (0.35355339059327373f * fRec58[0]));
			float fTemp23 = fVec5[(IOTA-iSlow161)&4095];
			fVec16[IOTA&4095] = (fSlow162 * (fRec31[0] * fTemp23));
			float fTemp24 = (0.3f * fVec16[(IOTA-iConst25)&4095]);
			float fTemp25 = (fTemp24 + ((0.6f * fRec56[1]) + fVec15[(IOTA-iConst53)&16383]));
			fVec17[IOTA&2047] = fTemp25;
			fRec56[0] = fVec17[(IOTA-iConst55)&2047];
			float 	fRec57 = (0 - (0.6f * fVec17[IOTA&2047]));
			fRec63[0] = ((fConst17 * fRec63[1]) + (fConst16 * (fRec37[1] + fRec37[2])));
			fRec62[0] = ((fSlow171 * fRec62[1]) + (fSlow170 * (fRec37[1] + (fSlow164 * fRec63[0]))));
			fVec18[IOTA&8191] = (1e-20f + (0.35355339059327373f * fRec62[0]));
			float fTemp26 = (fVec18[(IOTA-iConst61)&8191] + (fTemp24 + (0.6f * fRec60[1])));
			fVec19[IOTA&2047] = fTemp26;
			fRec60[0] = fVec19[(IOTA-iConst62)&2047];
			float 	fRec61 = (0 - (0.6f * fVec19[IOTA&2047]));
			fRec67[0] = ((fConst17 * fRec67[1]) + (fConst16 * (fRec35[1] + fRec35[2])));
			fRec66[0] = ((fSlow180 * fRec66[1]) + (fSlow179 * (fRec35[1] + (fSlow173 * fRec67[0]))));
			fVec20[IOTA&16383] = (1e-20f + (0.35355339059327373f * fRec66[0]));
			float fTemp27 = (((0.6f * fRec64[1]) + fVec20[(IOTA-iConst68)&16383]) - fTemp24);
			fVec21[IOTA&2047] = fTemp27;
			fRec64[0] = fVec21[(IOTA-iConst69)&2047];
			float 	fRec65 = (0 - (0.6f * fVec21[IOTA&2047]));
			fRec71[0] = ((fConst17 * fRec71[1]) + (fConst16 * (fRec39[1] + fRec39[2])));
			fRec70[0] = ((fSlow189 * fRec70[1]) + (fSlow188 * (fRec39[1] + (fSlow182 * fRec71[0]))));
			fVec22[IOTA&16383] = (1e-20f + (0.35355339059327373f * fRec70[0]));
			float fTemp28 = (((0.6f * fRec68[1]) + fVec22[(IOTA-iConst75)&16383]) - fTemp24);
			fVec23[IOTA&1023] = fTemp28;
			fRec68[0] = fVec23[(IOTA-iConst76)&1023];
			float 	fRec69 = (0 - (0.6f * fVec23[IOTA&1023]));
			fRec32[0] = (fRec68[1] + (fRec64[1] + (fRec60[1] + (fRec56[1] + (fRec52[1] + (fRec48[1] + (fRec40[1] + (fRec44[1] + (fRec69 + (fRec65 + (fRec61 + (fRec57 + fTemp22))))))))))));
			fRec33[0] = (0 - ((fRec68[1] + (fRec64[1] + (fRec60[1] + (fRec56[1] + (fRec69 + (fRec65 + (fRec57 + fRec61))))))) - (fRec52[1] + (fRec48[1] + (fRec40[1] + (fRec44[1] + fTemp22))))));
			float fTemp29 = (fRec49 + fRec53);
			fRec34[0] = (0 - ((fRec68[1] + (fRec64[1] + (fRec52[1] + (fRec48[1] + (fRec69 + (fRec65 + fTemp29)))))) - (fRec60[1] + (fRec56[1] + (fRec40[1] + (fRec44[1] + (fRec61 + (fRec57 + fTemp19))))))));
			fRec35[0] = (0 - ((fRec60[1] + (fRec56[1] + (fRec52[1] + (fRec48[1] + (fRec61 + (fRec57 + fTemp29)))))) - (fRec68[1] + (fRec64[1] + (fRec40[1] + (fRec44[1] + (fRec69 + (fRec65 + fTemp19))))))));
			float fTemp30 = (fRec45 + fRec49);
			float fTemp31 = (fRec41 + fRec53);
			fRec36[0] = (0 - ((fRec68[1] + (fRec60[1] + (fRec52[1] + (fRec40[1] + (fRec69 + (fRec61 + fTemp31)))))) - (fRec64[1] + (fRec56[1] + (fRec48[1] + (fRec44[1] + (fRec65 + (fRec57 + fTemp30))))))));
			fRec37[0] = (0 - ((fRec64[1] + (fRec56[1] + (fRec52[1] + (fRec40[1] + (fRec65 + (fRec57 + fTemp31)))))) - (fRec68[1] + (fRec60[1] + (fRec48[1] + (fRec44[1] + (fRec69 + (fRec61 + fTemp30))))))));
			float fTemp32 = (fRec45 + fRec53);
			float fTemp33 = (fRec41 + fRec49);
			fRec38[0] = (0 - ((fRec64[1] + (fRec60[1] + (fRec48[1] + (fRec40[1] + (fRec65 + (fRec61 + fTemp33)))))) - (fRec68[1] + (fRec56[1] + (fRec52[1] + (fRec44[1] + (fRec69 + (fRec57 + fTemp32))))))));
			fRec39[0] = (0 - ((fRec68[1] + (fRec56[1] + (fRec48[1] + (fRec40[1] + (fRec69 + (fRec57 + fTemp33)))))) - (fRec64[1] + (fRec60[1] + (fRec52[1] + (fRec44[1] + (fRec65 + (fRec61 + fTemp32))))))));
			output0[i] = (FAUSTFLOAT)((0.37f * (fRec33[0] + fRec34[0])) + (fSlow114 * (fTemp15 * fVec5[IOTA&4095])));
			output1[i] = (FAUSTFLOAT)((0.37f * (fRec33[0] - fRec34[0])) + (fSlow162 * (fTemp15 * fTemp23)));
			// post processing
			fRec39[2] = fRec39[1]; fRec39[1] = fRec39[0];
			fRec38[2] = fRec38[1]; fRec38[1] = fRec38[0];
			fRec37[2] = fRec37[1]; fRec37[1] = fRec37[0];
			fRec36[2] = fRec36[1]; fRec36[1] = fRec36[0];
			fRec35[2] = fRec35[1]; fRec35[1] = fRec35[0];
			fRec34[2] = fRec34[1]; fRec34[1] = fRec34[0];
			fRec33[2] = fRec33[1]; fRec33[1] = fRec33[0];
			fRec32[2] = fRec32[1]; fRec32[1] = fRec32[0];
			fRec68[1] = fRec68[0];
			fRec70[1] = fRec70[0];
			fRec71[1] = fRec71[0];
			fRec64[1] = fRec64[0];
			fRec66[1] = fRec66[0];
			fRec67[1] = fRec67[0];
			fRec60[1] = fRec60[0];
			fRec62[1] = fRec62[0];
			fRec63[1] = fRec63[0];
			fRec56[1] = fRec56[0];
			fRec58[1] = fRec58[0];
			fRec59[1] = fRec59[0];
			fRec52[1] = fRec52[0];
			fRec54[1] = fRec54[0];
			fRec55[1] = fRec55[0];
			fRec48[1] = fRec48[0];
			fRec50[1] = fRec50[0];
			fRec51[1] = fRec51[0];
			fRec44[1] = fRec44[0];
			fRec46[1] = fRec46[0];
			fRec47[1] = fRec47[0];
			fRec40[1] = fRec40[0];
			fRec42[1] = fRec42[0];
			fRec43[1] = fRec43[0];
			fRec31[1] = fRec31[0];
			fRec30[2] = fRec30[1]; fRec30[1] = fRec30[0];
			fRec15[1] = fRec15[0];
			fRec14[1] = fRec14[0];
			fRec26[1] = fRec26[0];
			fVec4[1] = fVec4[0];
			fRec28[1] = fRec28[0];
			fRec29[1] = fRec29[0];
			fVec3[1] = fVec3[0];
			IOTA = IOTA+1;
			fRec18[1] = fRec18[0];
			fRec19[1] = fRec19[0];
			fVec2[1] = fVec2[0];
			fRec25[1] = fRec25[0];
			fRec20[1] = fRec20[0];
			fRec21[1] = fRec21[0];
			fRec22[1] = fRec22[0];
			fRec23[1] = fRec23[0];
			fRec24[1] = fRec24[0];
			fRec0[1] = fRec0[0];
			fRec1[2] = fRec1[1]; fRec1[1] = fRec1[0];
			fRec2[2] = fRec2[1]; fRec2[1] = fRec2[0];
			fRec3[2] = fRec3[1]; fRec3[1] = fRec3[0];
			fRec4[2] = fRec4[1]; fRec4[1] = fRec4[0];
			fRec5[1] = fRec5[0];
			fRec6[1] = fRec6[0];
			fRec7[1] = fRec7[0];
			fRec8[1] = fRec8[0];
			fRec9[1] = fRec9[0];
			fVec1[1] = fVec1[0];
			fVec0[1] = fVec0[0];
			iRec13[1] = iRec13[0];
			fRec12[1] = fRec12[0];
			fRec10[1] = fRec10[0];
			fRec11[1] = fRec11[0];
		}
}

void ZamPianoPlugin::d_run(const float** inputs, float** outputs, uint32_t frames,
				const MidiEvent* midievent, uint32_t midicount)
{
	uint32_t i;
	bool signal;

	for (i = 0; i < midicount; i++) {
		int type = midievent[i].data[0] & 0xF0;
		int chan = midievent[i].data[0] & 0x0F;
		int n = midievent[i].data[1];
		int v = midievent[i].data[2];
		if (type == 0x90 && chan == 0x0) {
			// NOTE ON
			note[n].state = STRIKE;
			note[n].vel = v / 127.f;
		}
		else if (type == 0x80 && chan == 0x0) {
			// NOTE OFF
			if (note[n].state != SILENT)
				note[n].state = RELEASE;
		}
	}

	for (i = 0; i < frames; i++) {
		outputs[0][i] = 0.f;
		outputs[1][i] = 0.f;
	}
	signal = false;
	int k;
	for (k = 0; k < 128; k++) {
		if (k > 87) continue;
		if (note[k].state == SILENT) {
			continue;
		}
		signal = true;
		if (note[k].state == STRIKE) {
			printf("STRIKE: %d\n", k);
			pfreq = 440. * powf(2., (k-69)/12.);
			pgain = note[k].vel;
			pgate = 0.;
			ZamPianoPlugin::compute(1, inputs, outputs);
			pgate = 1.;
			ZamPianoPlugin::compute(frames, inputs, outputs);
			note[k].state = SUSTAIN;
		} else if (note[k].state == SUSTAIN) {
			ZamPianoPlugin::compute(frames, inputs, outputs);
		} else if (note[k].state == RELEASE) {
			printf("RELEASE: %d\n", k);
			pfreq = 440. * powf(2., (k-69)/12.);
			pgain = 0.001;
			pgate = 0.;
			ZamPianoPlugin::compute(1, inputs, outputs);
			pgate = 1.;
			ZamPianoPlugin::compute(frames, inputs, outputs);
			note[k].state++;
		}
	}
	for (i = 0; i < frames; i++) {
		if (signal) {
			sanitize_denormal(outputs[0][i]);
			sanitize_denormal(outputs[1][i]);
		} else {
			outputs[0][i] = 0.f;
			outputs[1][i] = 0.f;
		}
	}
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new ZamPianoPlugin();
}

// -----------------------------------------------------------------------
END_NAMESPACE_DISTRHO
