#include <stdio.h>
#include <inttypes.h>
#include <cmath>
#include "triode.h"
using std::abs;
#define DUMP(x) x

T Triode::compute(T a, T R, T Vgate, T Vk) {
	T VakGuess = 100.;
	
	T Vgk = Vgate - Vk;

	T Vak = VakGuess; // initial guess
	int iteration = 0;
	T err = 1e6;
	while (fabs(err)/fabs(Vak) > 1e-9){
		VakGuess = iterateNewtonRaphson(Vak, 1e-6, Vgk, a, R);
		err = Vak - VakGuess;
		Vak = VakGuess;

		if (iteration > 100){
			printf("Convergence failure!");
			break;
		}
		++iteration;
	}
	T b = Vak - R*getIa(Vgk, Vak);

	//printf("Vgate=%f Vk=%f  Vgk=%f b=%f\n", Vgate, Vk, Vgk, b);
	return b;
}

T Triode::getIa(T Vgk, T Vak) {
	if (Vak < 0.0) {
		printf("Less than zero!\n");
		Vak = 0.0;
	}
	if (Vgk > 0.0) {
		Vgk = 0.0;
	}
	T mu = 100.;
	T kx = 1.4;
	T kg1 = 3.981e-8;
	T kp = 600.;
	T kvb = 300.;

	T e1 = Vak*log(1.+exp(kp*(1./mu+Vgk/pow(kvb+Vak*Vak, 0.5))))/kp;
	if (e1 < 0) {
		return 0.;
	}
	return (pow(e1, kx) / kg1);
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
	vg = 0.0;
	vk = 0.0;
	vp = 0.0;
	insane = false;
}
