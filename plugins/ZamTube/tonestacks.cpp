#include "tonestacks.hpp"

static const float tones[] = {
//	R1	R2	R3	R4	C1	C2	C3
	250e+3,1e+6,	25e+3,	56e+3,	250e-12,20e-9,	20e-9,
	250e+3,250e+3,	25e+3,	100e+3,	250e-12,100e-9,	47e-9,
	250e+3,250e+3,	10e+3,	100e+3,	120e-12,100e-9,	47e-9,
	250e+3,250e+3,	4.8e+3,	100e+3,	250e-12,100e-9,	47e-9,
	220e+3,1e+6,	22e+3,	33e+3,	470e-12,22e-9,	22e-9,
	250e+3,1e+6,	25e+3,	56e+3,	500e-12,22e-9,	22e-9,
	250e+3,1e+6,	25e+3,	33e+3,	270e-12,22e-9,	22e-9,
	250e+3,1e+6,	25e+3,	33e+3,	500e-12,22e-9,	22e-9,
	250e+3,250e+3,	25e+3,	56e+3,	250e-12,47e-9,	47e-9,
	1e+6,	1e+6,	10e+3,	100e+3,	50e-12,	22e-9,	22e-9,
	220e+3,220e+3,	220e+3,	100e+3,	470e-12,100e-9,	47e-9,
	250e+3,1e+6,	25e+3,	47e+3,	470e-12,20e-9,	20e-9,
	500e+3,1e+6,	10e+3,	47e+3,	470e-12,22e-9,	22e-9,
	250e+3,250e+3,	20e+3,	68e+3,	270e-12,22e-9,	22e-9,
	250e+3,250e+3,	10e+3,	100e+3,	270e-12,100e-9,	40e-9,
	250e+3,250e+3,	10e+3,	41e+3,	240e-12,33e-9,	82e-9,
	250e+3,1e+6,	25e+3,	32e+3,	470e-12,22e-9,	22e-9,
	250e+3,250e+3,	10e+3,	100e+3,	100e-12,100e-9,	47e-9,
	250e+3,1e+6,	33e+3,	51e+3,	220e-12,15e-9,	47e-9,
	220e+3,1e+6,	22e+3,	68e+3,	470e-12,22e-9,	22e-9,
	220e+3,220e+3,	10e+3,	100e+3,	220e-12,47e-9,	47e-9,
	250e+3,250e+3,	25e+3,	100e+3,	250e-12,22e-9,	22e-9,
	250e+3,250e+3,	10e+3,	100e+3,	250e-12,100e-9,	47e-9,
	250e+3,250e+3,	25e+3,	130e+3,	250e-12,100e-9,	22e-9,
	1e+6,	1e+6,	94e+3,	270e+3,	25e-12,	60e-9,	20e-9
};

const float* Tonestacks::bassman = (const float*)&tones[0];
const float* Tonestacks::mesa = (const float*)&tones[1*7];
const float* Tonestacks::twin = (const float*)&tones[2*7];
const float* Tonestacks::princeton = (const float*)&tones[3*7];
const float* Tonestacks::jcm800 = (const float*)&tones[4*7];
const float* Tonestacks::jcm2000 = (const float*)&tones[5*7];
const float* Tonestacks::jtm45 = (const float*)&tones[6*7];
const float* Tonestacks::mlead = (const float*)&tones[7*7];
const float* Tonestacks::m2199 = (const float*)&tones[8*7];
const float* Tonestacks::ac30 = (const float*)&tones[9*7];
const float* Tonestacks::ac15 = (const float*)&tones[10*7];
const float* Tonestacks::soldano = (const float*)&tones[11*7];
const float* Tonestacks::sovtek = (const float*)&tones[12*7];
const float* Tonestacks::peavey = (const float*)&tones[13*7];
const float* Tonestacks::ibanez = (const float*)&tones[14*7];
const float* Tonestacks::roland = (const float*)&tones[15*7];
const float* Tonestacks::ampeg = (const float*)&tones[16*7];
const float* Tonestacks::ampegrev = (const float*)&tones[17*7];
const float* Tonestacks::bogner = (const float*)&tones[18*7];
const float* Tonestacks::groove = (const float*)&tones[19*7];
const float* Tonestacks::crunch = (const float*)&tones[20*7];
const float* Tonestacks::fenderblues = (const float*)&tones[21*7];
const float* Tonestacks::fender = (const float*)&tones[22*7];
const float* Tonestacks::fenderdeville = (const float*)&tones[23*7];
const float* Tonestacks::gibson = (const float*)&tones[24*7];

