#include "instrument.hpp"
#include <cmath>
#include <inttypes.h>

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

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif  

class PianoNote {
public:
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

	PianoNote() {};
	void init (void);
	void compute (uint32_t count, const float**, float** output);
	float pfreq, pbright, pgate, phammer, pgain, pstiff, pdetune, prevgain,
		ppanangle, prevroom, pspatialwidth;

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
};
