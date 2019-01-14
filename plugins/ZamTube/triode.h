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

#define EPSILON 1e-9
#define ITER 50
#define TOLERANCE 1e-6

#define MAX(x, y) (( (x) > (y) ) ? x : y )
#define MIN(x, y) (( (x) < (y) ) ? x : y )

typedef double T;

class Triode {
public:
	T mu, kp, kvb, kg1, kx;

	Triode();
	T compute(T a, T R, T Vg, T Vk);
	T getIa(T Vgk, T Vak);
	T evaluateImplicitEquation(T Vak, T Vgk, T a, T R);
	T iterateNewtonRaphson(T x, T dx, T Vgk, T a, T R);
	bool insane;
};

#endif
