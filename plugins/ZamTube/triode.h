/*
wdf.h
Copyright (C) 2013  Damien Zammit

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef __WDF_H__
#define __WDF_H__

#include <math.h>

#define BIG 1e12
#define SMALL 1e-14
#define EPSILON 1e-9
#define ITER 50
#define SWAP_PP(x,y) {T tmp=y; y=x; x=tmp;}
#define SWAP_PN(x,y) {T tmp=y; y=-x; x=tmp;}
#define SWAP_NP(x,y) {T tmp=y; y=x; x=-tmp;}
#define SWAP_NN(x,y) {T tmp=y; y=-x; x=-tmp;}
#define TOLERANCE 1e-6

#define MAX(x, y) (( (x) > (y) ) ? x : y )
#define MIN(x, y) (( (x) < (y) ) ? x : y )

typedef double T;

static inline float
sanitize_denormal(float v) {
        if(!std::isnormal(v) || !std::isfinite(v))
                return 0.f;
        return v;
}

class Triode {
public:
	T Kb, Gb, Pb;
	T Kr, Gr, Pr;

	T vg, vk, vp;
	T g, mu, gamma, c, gg, e, cg, ig0;
	T g1, mu1, gamma1, c1, gg1, e1, cg1, ig01;
	T g2, mu2, gamma2, c2, gg2, e2, cg2, ig02;

	bool insane;
	
	Triode();
	T compute(T Kbb, T Gbb, T Pbb, T R);
	T getIa(T Vgk, T Vak);
	T evaluateImplicitEquation(T Vak, T Vgk, T a, T R);
	T iterateNewtonRaphson(T x, T dx, T Vgk, T a, T R);
};

#endif
