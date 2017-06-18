#include "ZamVerbImpulses.hpp"

static const float impulse_1[] = {
	#include "studioA_4.inc"
};
const float* ZamVerbImpulses::studioA = (const float*)impulse_1;

static const float impulse_2[] = {
	#include "studioB_4.inc"
};
const float* ZamVerbImpulses::studioB = (const float*)impulse_2;

static const float impulse_3[] = {
	#include "roomA_4.inc"
};
const float* ZamVerbImpulses::roomA = (const float*)impulse_3;

static const float impulse_4[] = {
	#include "roomB_4.inc"
};
const float* ZamVerbImpulses::roomB = (const float*)impulse_4;

static const float impulse_5[] = {
	#include "hallA_4.inc"
};
const float* ZamVerbImpulses::hallA = (const float*)impulse_5;

static const float impulse_6[] = {
	#include "hallB_4.inc"
};
const float* ZamVerbImpulses::hallB = (const float*)impulse_6;

static const float impulse_7[] = {
	#include "plate_4.inc"
};
const float* ZamVerbImpulses::plate = (const float*)impulse_7;
