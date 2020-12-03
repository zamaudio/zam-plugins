#include <stdio.h>
#include <inttypes.h>
#include <cmath>
#include "triode.h"
using std::abs;
#define DUMP(x) x

#if 0
T Triode::compute_old(T a, T R, T Vg, T Vk) {
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
#endif

T Triode::getIa(T Vgk, T Vpk) const {
	if (Vpk < 0.0) {
		Vpk = 0.0;
	}
	if (Vgk > 0.0) {
		Vgk = 0.0;
	}

	/* exact solution (expensive) */
	T ee1 = Vpk*log1p(exp(kp*(1./mu+Vgk/sqrt(kvb+Vpk*Vpk))))/kp;
	if (ee1 < 0) {
		return 0.;
	}
	//printf("Vpk=%f ans=%f e1=%f exact_e1=%f\n", Vpk, ans, e1, ee1);
	return 1e+6*pow(ee1, kx) / kg1;
}

T Triode::iterateNewtonRaphson(T x, T dx, T Vgk, T a, T R) const {
	T xIak = getIa(Vgk, x);
	T dxIak = getIa(Vgk, x + dx);
	T xNew = x - dx*(x + R*xIak - a)/(dx + R*(dxIak - xIak));
	return xNew;
}


T Triode::compute(T a, T R, T Vg, T Vk) {
	T mu1, vp2, vgg, evgg, evgg1, levgg1, powkx, pkxvg;
	T dIpVp, ra;
	T Vp, Ip, Vpk, Vgk, b;

	Vgk = Vk - Vg;
	Vpk = -mu * Vgk;
	Ip = getIa(Vgk, Vpk);

	b = Vpk - R*Ip;
	Vp = a - R*Ip;

	mu1 = 1.0/mu;
	vp2 = sqrt(Vp*Vp + kvb);
	vgg = Vg / vp2 + mu1;
	evgg = exp(kp*vgg);
	evgg1 = 1. + evgg;
	levgg1 = log1p(evgg);
	powkx = pow(Vp*levgg1/kp, kx - 1.0);
	pkxvg = kx*powkx*evgg;

	dIpVp = pkxvg/(vp2*evgg1);
	ra = kg1 / dIpVp;
	if (ra < 1.)
		ra = 1.;
	
	//dIpVg = Vp*pkxvg*(vgg/(kp*evgg1) - levgg1/(kp*kp));
	//gm = dIpVg / kg1;
	
	b = Vpk - (R + 0.1/ra)*Ip;
	return b;

}

Triode::Triode()
{
	//12AX7 RSD-1 (custom)
	mu = 100.;
	kx = 1.4;
	kg1 = 1060.;
	kp = 600.;
	kvb = 300.;
}

