#include <stdio.h>
#include <inttypes.h>
#include <cmath>
#include "triode.h"
using std::abs;
#define DUMP(x) x

T Triode::compute(T a, T R, T Vg, T Vk) {
	T VakGuess = 100.;
	T Vgk = Vg - Vk;
	T Vak = VakGuess;
	int iteration = 0;
	T err = 1e6;

	for (iteration = 0; (fabs(err)/fabs(Vak) > EPSILON) && (iteration <= ITER); iteration++){
		VakGuess = iterateNewtonRaphson(Vak, TOLERANCE, Vgk, a, R);
		err = Vak - VakGuess;
		Vak = VakGuess;
	}
	T b = Vak - R*getIa(Vgk, Vak);

	//printf("Vgate=%f Vk=%f  Vgk=%f b=%f\n", Vgate, Vk, Vgk, b);
	return b;
}

T Triode::getIa(T Vgk, T Vpk) {
	if (Vpk < 0.0) {
		//printf("Less than zero!\n");
		Vpk = 0.0;
	}
	if (Vgk > 0.0) {
		Vgk = 0.0;
	}

	T e1 = Vpk*log1p(exp(kp*(1./mu+Vgk/pow(kvb+Vpk*Vpk, 0.5))))/kp;
	if (e1 < 0) {
		return 0.;
	}
	T test = 1e+6*pow(e1, kx) / kg1;
	//printf("XXX Ip=%f lut=%f diff=%f  vgk=%f vpk=%f\n", test, test2, test2-test, Vgk, Vpk);
	return test;

}

T Triode::evaluateImplicitEquation(T Vak, T Vgk, T a, T R){
	T Iak = getIa(Vgk, Vak);
	return Vak + R*Iak - a;
}

T Triode::iterateNewtonRaphson(T x, T dx, T Vgk, T a, T R){
	T F = evaluateImplicitEquation(x, Vgk, a, R);
	T xNew = x - dx*F/(evaluateImplicitEquation(x + dx, Vgk, a, R) - F);
	return xNew;
}

Triode::Triode()
{
	insane = true;

	//12AX7 EHX-1
	kvb = 300.;
	mu = 103.2;
	kx = 1.26;
	kg1 = 446.0;
	kp = 3.4;
}

