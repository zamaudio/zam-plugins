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

#ifndef ZAMPIANOPLUGIN_HPP_INCLUDED
#define ZAMPIANOPLUGIN_HPP_INCLUDED

#include <string.h>
#include "DistrhoPlugin.hpp"
#include "instrument.h"

#define STRIKE 0
#define ATTACK 1
#define SUSTAIN 2
#define RELEASE 1000
#define DECAY 1001
#define SILENT 1011

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class ZamPianoPlugin : public Plugin
{
public:
    enum Parameters
    {
        paramFreq,
	paramGain,
        paramGate,
	paramBright,
	paramDetuning,
	paramHammerHard,
	paramStiffness,
	paramReverbGain,
        paramReverbRoom,
	paramPanAngle,
	paramSpatialWidth,
        paramCount
    };
    ZamPianoPlugin();
    ~ZamPianoPlugin();

	float fSamplingFreq;
	int 	iConst0;
	float 	fConst1;
	float 	fConst2;
	float 	fConst3;
	float 	fRec11[2];
	float 	fConst4;
	float 	fConst5;
	float 	fConst6;
	float 	fRec10[2];
	float 	fConst7;
	float 	fRec12[2];
	int 	iRec13[2];
	float 	fVec0[2];
	float 	fVec1[2];
	float 	fRec9[2];
	float 	fRec8[2];
	float 	fRec7[2];
	float 	fRec6[2];
	float 	fRec5[2];
	float 	fConst8;
	float 	fRec4[3];
	float 	fRec3[3];
	float 	fRec2[3];
	float 	fRec1[3];
	float 	fRec0[2];
	float 	fRec24[2];
	float 	fRec23[2];
	float 	fRec22[2];
	float 	fRec21[2];
	float 	fRec20[2];
	float 	fRec25[2];
	float 	fVec2[2];
	float 	fRec19[2];
	float 	fRec18[2];
	int 	IOTA;
	float 	fRec17[4096];
	float 	fConst9;
	float 	fVec3[2];
	float 	fRec29[2];
	float 	fRec28[2];
	float 	fRec27[4096];
	float 	fVec4[2];
	float 	fRec26[2];
	float 	fRec14[2];
	float 	fRec15[2];
	int 	iConst10;
	float 	fConst11;
	float 	fConst12;
	float 	fRec30[3];
	float 	fConst13;
	float 	fVec5[4096];
	float 	fRec31[2];
	float 	fConst14;
	float 	fConst15;
	float 	fConst16;
	float 	fConst17;
	float 	fRec43[2];
	float 	fConst18;
	float 	fConst19;
	float 	fConst20;
	float 	fConst21;
	float 	fConst22;
	float 	fRec42[2];
	float 	fVec6[8192];
	float 	fConst23;
	int 	iConst24;
	float 	fVec7[4096];
	int 	iConst25;
	float 	fVec8[2048];
	int 	iConst26;
	float 	fRec40[2];
	float 	fRec47[2];
	float 	fConst27;
	float 	fConst28;
	float 	fConst29;
	float 	fConst30;
	float 	fRec46[2];
	float 	fVec9[8192];
	float 	fConst31;
	int 	iConst32;
	float 	fVec10[1024];
	int 	iConst33;
	float 	fRec44[2];
	float 	fRec51[2];
	float 	fConst34;
	float 	fConst35;
	float 	fConst36;
	float 	fConst37;
	float 	fRec50[2];
	float 	fVec11[8192];
	float 	fConst38;
	int 	iConst39;
	float 	fVec12[2048];
	int 	iConst40;
	float 	fRec48[2];
	float 	fRec55[2];
	float 	fConst41;
	float 	fConst42;
	float 	fConst43;
	float 	fConst44;
	float 	fRec54[2];
	float 	fVec13[8192];
	float 	fConst45;
	int 	iConst46;
	float 	fVec14[1024];
	int 	iConst47;
	float 	fRec52[2];
	float 	fRec59[2];
	float 	fConst48;
	float 	fConst49;
	float 	fConst50;
	float 	fConst51;
	float 	fRec58[2];
	float 	fVec15[16384];
	float 	fConst52;
	int 	iConst53;
	float 	fConst54;
	float 	fVec16[4096];
	float 	fVec17[2048];
	int 	iConst55;
	float 	fRec56[2];
	float 	fRec63[2];
	float 	fConst56;
	float 	fConst57;
	float 	fConst58;
	float 	fConst59;
	float 	fRec62[2];
	float 	fVec18[8192];
	float 	fConst60;
	int 	iConst61;
	float 	fVec19[2048];
	int 	iConst62;
	float 	fRec60[2];
	float 	fRec67[2];
	float 	fConst63;
	float 	fConst64;
	float 	fConst65;
	float 	fConst66;
	float 	fRec66[2];
	float 	fVec20[16384];
	float 	fConst67;
	int 	iConst68;
	float 	fVec21[2048];
	int 	iConst69;
	float 	fRec64[2];
	float 	fRec71[2];
	float 	fConst70;
	float 	fConst71;
	float 	fConst72;
	float 	fConst73;
	float 	fRec70[2];
	float 	fVec22[16384];
	float 	fConst74;
	int 	iConst75;
	float 	fVec23[1024];
	int 	iConst76;
	float 	fRec68[2];
	float 	fRec32[3];
	float 	fRec33[3];
	float 	fRec34[3];
	float 	fRec35[3];
	float 	fRec36[3];
	float 	fRec37[3];
	float 	fRec38[3];
	float 	fRec39[3];


	double noteOffDelayTime_points[5*2];
	double singleStringDecayRate_points[17*2];
	double singleStringZero_points[17*2];
	double singleStringPole_points[17*2];
	double releaseLoopGain_points[10*2];
	double detuningHz_points[18*2];
	double stiffnessCoefficient_points[10*2];
	double strikePosition_points[12*2];
	double EQGain_points[14*2];
	double EQBandwidthFactor_points[13*2];
	double loudPole_points[19*2];
	double softPole_points[16*2];
	double normalizedVelocity_points[8*2];
	double loudGain_points[16*2];
	double softGain_points[15*2];
	double sustainPedalLevel_points[13*2];
	double DryTapAmpT60_points[16*2];
	double DryTapAmpCurrent;
	double DCBa1_points[18*2];
	double secondStageAmpRatio_points[6*2];
	double r1_1db_points[3*2];
	double r1_2db_points[4*2];
	double r2db_points[2*2];
	double r3db_points[2*2];
	double secondPartialFactor_points[2*2];
	double thirdPartialFactor_points[2*2];
	double bq4_gEarBalled_points[6*2];

	float getValueDryTapAmpT60 (float index){
		return DryTapAmpT60.getValue(index);
	}

	float getValueSustainPedalLevel (float index){
		return sustainPedalLevel.getValue(index);
	}

	float getValueLoudPole(float index){
		return loudPole.getValue(index);
	}

	float getValuePoleValue(float index){
		return softPole.getValue(index);
	}

	float getValueLoudGain(float index){
		return loudGain.getValue(index);
	}

	float getValueSoftGain(float index){
		return softGain.getValue(index);
	}

	float getValueDCBa1(float index){
		return DCBa1.getValue(index);
	}

	float getValuer1_1db(float index){
		return r1_1db.getValue(index);
	}

	float getValuer1_2db(float index){
		return r1_2db.getValue(index);
	}

	float getValuer2db(float index){
		return r2db.getValue(index);
	}

	float getValuer3db(float index){
		return r3db.getValue(index);
	}

	float getValueSecondStageAmpRatio(float index){
		return secondStageAmpRatio.getValue(index);
	}

	float getValueSecondPartialFactor(float index){
		return secondPartialFactor.getValue(index);
	}

	float getValueThirdPartialFactor(float index){
		return thirdPartialFactor.getValue(index);
	}

	float getValueBq4_gEarBalled(float index){
		return bq4_gEarBalled.getValue(index);
	}

	float getValueStrikePosition(float index){
		return strikePosition.getValue(index);
	}

	float getValueEQBandWidthFactor(float index){
		return EQBandwidthFactor.getValue(index);
	}

	float getValueEQGain(float index){
		return EQGain.getValue(index);
	}

	float getValueDetuningHz(float index){
		return detuningHz.getValue(index);
	}

	float getValueSingleStringDecayRate(float index){
		return singleStringDecayRate.getValue(index);
	}

	float getValueSingleStringZero(float index){
		return singleStringZero.getValue(index);
	}

	float getValueSingleStringPole(float index){
		return singleStringPole.getValue(index);
	}

	float getValueStiffnessCoefficient(float index){
		return stiffnessCoefficient.getValue(index);
	}

	float getValueReleaseLoopGain(float index){
		return releaseLoopGain.getValue(index);
	}

protected:
    // -------------------------------------------------------------------
    // Information

    const char* d_getLabel() const noexcept override
    {
        return "ZamPiano";
    }

    const char* d_getMaker() const noexcept override
    {
        return "Damien Zammit";
    }

    const char* d_getLicense() const noexcept override
    {
        return "GPL v2+";
    }

    uint32_t d_getVersion() const noexcept override
    {
        return 0x1000;
    }

    int64_t d_getUniqueId() const noexcept override
    {
        return d_cconst('Z', 'P', 'N', '2');
    }

    // -------------------------------------------------------------------
    // Init

    void d_initParameter(uint32_t index, Parameter& parameter) ;
    void d_initProgramName(uint32_t index, d_string& programName) ;

    // -------------------------------------------------------------------
    // Internal data

    float d_getParameterValue(uint32_t index) const override;
    void  d_setParameterValue(uint32_t index, float value) override;
    void  d_setProgram(uint32_t index) ;

    // -------------------------------------------------------------------
    // Process

	static inline float
	sanitize_denormal(float v) {
	        if(!std::isnormal(v))
	                return 0.f;
	        return v;
	}

	static inline float
	from_dB(float gdb) {
	        return (exp(gdb/20.f*log(10.f)));
	}

	static inline float
	to_dB(float g) {
	        return (20.f*log10(g));
	}


    void compute(uint32_t count, const float** inputs, float** outputs);

    void d_activate() override;
    void d_run(const float** inputs, float** outputs, uint32_t frames,
    		const MidiEvent* midievent, uint32_t midicount) override;
    // -------------------------------------------------------------------

	typedef struct {
		int state;
		double vel;
	} note_t;
	note_t note[128];
private:
        float pfreq;
        float pgain;
        float pgate;
        float pbright;
        float pdetune;
        float phammer;
        float pstiff;
        float prevgain;
        float prevroom;
        float ppanangle;
        float pspatialwidth;
	LookupTable bq4_gEarBalled;
	LookupTable thirdPartialFactor;
	LookupTable secondPartialFactor;
	LookupTable r3db;
	LookupTable r2db;
	LookupTable r1_2db;
	LookupTable r1_1db;
	LookupTable secondStageAmpRatio;
	LookupTable DCBa1;
	LookupTable DryTapAmpT60;
	LookupTable sustainPedalLevel;
	LookupTable softGain;
	LookupTable loudGain;
	LookupTable normalizedVelocity;
	LookupTable softPole;
	LookupTable loudPole;
	LookupTable EQBandwidthFactor;
	LookupTable EQGain;
	LookupTable strikePosition;
	LookupTable stiffnessCoefficient;
	LookupTable detuningHz;
	LookupTable releaseLoopGain;
	LookupTable singleStringPole;
	LookupTable singleStringZero;
	LookupTable singleStringDecayRate;
	LookupTable noteOffDelayTime;
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // ZAMPIANO_HPP_INCLUDED
