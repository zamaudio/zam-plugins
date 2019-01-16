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
	static bool prepared = false;
	static double coeff[3];

	if (!prepared) {
		const double L2 = log(2.0);
		const double scale = 1e+6*pow(L2, kx-2.0)/(8.0*pow(kp, kx));
		coeff[0] = 8.0*L2*L2*scale;
		coeff[1] = kx*kp*L2*4.0*scale;
		coeff[2] = (kp*kp*kx*kx + L2*kp*kp*kx - kp*kp*kx) * scale;
		prepared = true;
	}

	if (Vpk < 0.0) {
		//printf("Less than zero!\n");
		Vpk = 0.0;
	}
	if (Vgk > 0.0) {
		Vgk = 0.0;
	}

	double A = 1./mu + Vgk / sqrt(kvb + Vpk*Vpk);
	return Vpk*(coeff[0] + coeff[1]*A + coeff[2]*A*A) / kg1;

	/* exact solution (takes > 3x longer)
		e1 = Vpk*log1p(exp(kp*(1./mu+Vgk/sqrt(kvb+Vpk*Vpk))))/kp;
		if (e1 < 0) {
			return 0.;
		}
		return 1e+6*pow(e1, kx) / kg1;
	*/
}

T Triode::iterateNewtonRaphson(T x, T dx, T Vgk, T a, T R){
	T xIak = getIa(Vgk, x);
	T dxIak = getIa(Vgk, x + dx);
	T xNew = x - dx*(x + R*xIak - a)/(dx + R*(dxIak - xIak));
	return xNew;
}

Triode::Triode()
{
	//12AX7 RSD-1
	kvb = 300.;
	mu = 103.2;
	kx = 1.26;
	kg1 = 446.0;
	kp = 3.4;
}
