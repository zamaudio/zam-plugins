#include <stdio.h>
#include <inttypes.h>
#include <cmath>
#include "triode.h"
using std::abs;

T Triode::getC(void)
{
	return Kb;
}

T Triode::getP(void)
{
	return Pb;
}

T Triode::getG(void)
{
	return Gb;
}

void Triode::compute(T Pbb, T Gbb, T Kbb)
{
//	T Kb_o = Kb;
//	T Gb_o = Gb;
//	T Pb_o = Pb;

//	Kb = (2.0*vk-Kbb);
//	Gb = (2.0*vg-Gbb);
//	Pb = (2.0*vp-Pbb);

	Kb = Kbb;
	Gb = Gbb;
	Pb = Pbb;

	//Step 3: compute wave reflections inside the triode
	T vg0, vg1, vp0, vp1;

	vg0 = -10.0;
	vg1 = 10.0;
	vg = sanitize_denormal(zeroffg(vg0, vg1, TOLERANCE));
	//v.vg = v.secantfg(&vg0,&vg1);

	vp0 = e;
	vp1 = 0.0;
	if (insane) {
		vp = sanitize_denormal(zeroffp_insane(vp0,vp1,TOLERANCE));
	} else {
		vp = sanitize_denormal(zeroffp(vp0,vp1,TOLERANCE));
	}
	//v.vp = v.secantfp(&vp0,&vp1);

	vk = sanitize_denormal(ffk());

	Kb = (2.0*vk-Kb);
	Gb = (2.0*vg-Gb);
	Pb = (2.0*vp-Pb);

	//printf("vg = %f  vp = %f  vk = %f  Gb = %f  Kb = %f  Pb = %f\n", vg, vp, vk, Gb, Kb, Pb);
}

inline T _exp(const T x)
{
    if(x > -10.0 && x < 10.0)
        return 1.0 + x + x*x/2.0 + x*x*x/6.0 + x*x*x*x/24.0 + x*x*x*x*x/120.0
            + x*x*x*x*x*x/720.0 + x*x*x*x*x*x*x/5040.0;
    else
        return exp(x);
}

inline T _log(const T x)
{
    if(x > 10)
        return log(x);
    const T a=(x-1)/(x+1);
    return 2.0*(a+a*a*a/3.0+a*a*a*a*a/5.0+a*a*a*a*a*a*a/7.0+a*a*a*a*a*a*a*a*a/9.0); 
}

T Triode::ffg(T VG) {
        return (Gb-Gr*(gg*pow(log(1.0+exp(cg*VG))/cg,e)+ig0)-VG);
}

T Triode::fgdash(T VG) {
        T a1 = exp(cg*VG);
        T b1 = -e*pow(log(a1+1.0)/cg,e-1.0);
        T c1 = a1/(a1+1.0)*gg*Gr;
        return (b1*c1);
}

T Triode::ffp(T VP) {
	static bool prepared = false;
	if(!prepared) {
		//go go series expansion
		const double L2 = log(2.0);

		const double scale = pow(L2,gamma-2)/(8.0*pow(c,gamma));
		ffp_raw[0] = 8.0*L2*L2*scale;
		ffp_raw[1] = gamma*c*L2*4*scale;
		ffp_raw[2] = (c*c*gamma*gamma+L2*c*c*gamma-c*c*gamma)*scale;
		prepared = true;
	}

	return ffp_coeff[0]+ffp_coeff[1]*VP+ffp_coeff[2]*VP*VP;
}

void Triode::prepare(void)
{
    const double c0 = ffp_raw[0],
                 c1 = ffp_raw[1],
                 c2 = ffp_raw[2];
    ffp_coeff[0] = Pb + Pr*Gb/Gr + c2*Pr*vg*vg - Pr*vg/Gr + c1*Pr*vg + c0*Pr;
    ffp_coeff[1] = 2.*c2*Pr*vg/mu + c1*Pr/mu - 1.;
    ffp_coeff[2] = c2*Pr/(mu*mu);
}

T Triode::ffp_insane(T VP) {
	return (Pb+Pr*((g*pow(log(1.0+exp(c*(VP/mu+vg)))/c,gamma))+(Gb-vg)/Gr)-VP);
}

T Triode::fpdash(T VP) {
        T a1 = exp(c*(vg+VP/mu));
        T b1 = a1/(mu*(a1+1.0));
        T c1 = g*gamma*Pr*pow(log(a1+1.0)/c,gamma-1.0);
        return (c1*b1);
}

T Triode::ffk() {
        return (Kb - Kr*(g*pow(log(1.0+exp(c*(vp/mu+vg)))/c,gamma)));
}
/*
T Triode::secantfg(T *i1, T *i2) {
        T vgn = 0.0;
        T init = *i1;
        for (int i = 0; i<ITER; ++i) {
                vgn = *i1 - fg(*i1)*(*i1-*i2)/(fg(*i1)-fg(*i2));
                *i2 = *i1;
                *i1 = vgn;
                if ((fabs(fg(vgn))) < EPSILON) break;
        }
        if ((fabs(fg(vgn)) >= EPSILON)) {
                DUMP(fprintf(stderr,"Vg did not converge\n"));
		return init;
	}
        return vgn; 
}               
        
T Triode::newtonfg(T *i1) {
        T init = *i1;
        if (fabs(fg(*i1)) < EPSILON || fgdash(*i1)==0.0) return *i1;
        T vgn = 0.0;
        for (int i = 0; i<ITER; ++i) {
                vgn = *i1 - fg(*i1)/fgdash(*i1);
                *i1 = vgn;
                if (fabs(fg(vgn)) < EPSILON) break;
        } 
        if ((fabs(fg(vgn)) >= EPSILON)) {
//                vgn = init;
                DUMP(fprintf(stderr,"Vg did not converge\n"));
        }       
        return vgn;
}

T Triode::newtonfp(T *i1) {
        T init = *i1;
        if (fabs(fp(*i1)) < EPSILON || fpdash(*i1)==0.0) return *i1;
        T vpn = 0.0;
        for (int i = 0; i<ITER; ++i) {
                vpn = *i1 - fp(*i1)/fpdash(*i1);
                *i1 = vpn;
                if (fabs(fp(vpn)) < EPSILON) break;
        }
        if ((fabs(fp(vpn)) >= EPSILON)) {
//                vpn = init;
                DUMP(fprintf(stderr,"Vp did not converge\n"));
        }
        return vpn;
}

T Triode::secantfp(T *i1, T *i2) {
        T vpn = 0.0;
        for (int i = 0; i<ITER; ++i) {
                vpn = *i1 - fp(*i1)*(*i1-*i2)/(fp(*i1)-fp(*i2));
                *i2 = *i1;
                *i1 = vpn;
                if ((fabs(fp(vpn))) < EPSILON) break;
        }

        if ((fabs(fp(vpn)) >= EPSILON))
                DUMP(fprintf(stderr,"Vp did not converge\n"));
        return vpn;
}
*/

//****************************************************************************80
//	Purpose:
//
//		Brent's method root finder.
//
//	Licensing:
//
//		This code below is distributed under the GNU LGPL license.
//
//	Author:
//
//		Original FORTRAN77 version by Richard Brent.
//		C++ version by John Burkardt.
//		Adapted for zamvalve by Damien Zammit.

T Triode::r8_abs ( T x )
{
	T value;

	if ( 0.0 <= x )
	{
		value = x;
	}
	else
	{
		value = - x;
	}
	return value;
}

Triode::Triode()
{
	vg = 0.0;
	vk = 0.0;
	vp = 0.0;
	insane = false;

	T r = 1.0;

	while ( 1.0 < ( T ) ( 1.0 + r )	)
	{
		r = r / 2.0;
	}

	r *= 2.0;
	r8_epsilon = r;
}

T Triode::r8_max ( T x, T y )
{
	T value;

	if ( y < x )
	{
		value = x;
	}
	else
	{
		value = y;
	}
	return value;
}

T Triode::r8_sign ( T x )
{
	T value;

	if ( x < 0.0 )
	{
		value = -1.0;
	}
	else
	{
		value = 1.0;
	}
	return value;
}

T Triode::zeroffp_insane ( T a, T b, T t )
{
	T c;
	T d;
	T e;
	T fa;
	T fb;
	T fc;
	T m;
	T macheps;
	T p;
	T q;
	T r;
	T s;
	T sa;
	T sb;
	T tol;
//
//	Make local copies of A and B.
//
	sa = a;
	sb = b;
	fa = ffp_insane ( sa );
	fb = ffp_insane ( sb );

	c = sa;
	fc = fa;
	e = sb - sa;
	d = e;

	macheps = r8_epsilon;

	for ( ; ; )
	{
		if ( abs ( fc ) < abs ( fb ) )
		{
			sa = sb;
			sb = c;
			c = sa;
			fa = fb;
			fb = fc;
			fc = fa;
		}

		tol = 2.0 * macheps * abs ( sb ) + t;
		m = 0.5 * ( c - sb );

		if ( abs ( m ) <= tol || fb == 0.0 )
		{
			break;
		}

		if ( abs ( e ) < tol || abs ( fa ) <= abs ( fb ) )
		{
			e = m;
			d = e;
		}
		else
		{
			s = fb / fa;

			if ( sa == c )
			{
				p = 2.0 * m * s;
				q = 1.0 - s;
			}
			else
			{
				q = fa / fc;
				r = fb / fc;
				p = s * ( 2.0 * m * q * ( q - r ) - ( sb - sa ) * ( r - 1.0 ) );
				q = ( q - 1.0 ) * ( r - 1.0 ) * ( s - 1.0 );
			}

			if ( 0.0 < p )
			{
				q = - q;
			}
			else
			{
				p = - p;
			}

			s = e;
			e = d;

			if ( 2.0 * p < 3.0 * m * q - abs ( tol * q ) &&
				p < abs ( 0.5 * s * q ) )
			{
				d = p / q;
			}
			else
			{
				e = m;
				d = e;
			}
		}
		sa = sb;
		fa = fb;

		if ( tol < abs ( d ) )
		{
			sb = sb + d;
		}
		else if ( 0.0 < m )
		{
			sb = sb + tol;
		}
		else
		{
			sb = sb - tol;
		}

		fb = ffp_insane ( sb );

		if ( ( 0.0 < fb && 0.0 < fc ) || ( fb <= 0.0 && fc <= 0.0 ) )
		{
			c = sa;
			fc = fa;
			e = sb - sa;
			d = e;
		}
	}
	return sb;
}

T Triode::zeroffp ( T a, T b, T t )
{
	T c;
	T d;
	T e;
	T fa;
	T fb;
	T fc;
	T m;
	T macheps;
	T p;
	T q;
	T r;
	T s;
	T sa;
	T sb;
	T tol;

	prepare();
//
//	Make local copies of A and B.
//
	sa = a;
	sb = b;
	fa = ffp ( sa );
	fb = ffp ( sb );

	c = sa;
	fc = fa;
	e = sb - sa;
	d = e;

	macheps = r8_epsilon;

	for ( ; ; )
	{
		if ( abs ( fc ) < abs ( fb ) )
		{
			sa = sb;
			sb = c;
			c = sa;
			fa = fb;
			fb = fc;
			fc = fa;
		}

		tol = 2.0 * macheps * abs ( sb ) + t;
		m = 0.5 * ( c - sb );

		if ( abs ( m ) <= tol || fb == 0.0 )
		{
			break;
		}

		if ( abs ( e ) < tol || abs ( fa ) <= abs ( fb ) )
		{
			e = m;
			d = e;
		}
		else
		{
			s = fb / fa;

			if ( sa == c )
			{
				p = 2.0 * m * s;
				q = 1.0 - s;
			}
			else
			{
				q = fa / fc;
				r = fb / fc;
				p = s * ( 2.0 * m * q * ( q - r ) - ( sb - sa ) * ( r - 1.0 ) );
				q = ( q - 1.0 ) * ( r - 1.0 ) * ( s - 1.0 );
			}

			if ( 0.0 < p )
			{
				q = - q;
			}
			else
			{
				p = - p;
			}

			s = e;
			e = d;

			if ( 2.0 * p < 3.0 * m * q - abs ( tol * q ) &&
				p < abs ( 0.5 * s * q ) )
			{
				d = p / q;
			}
			else
			{
				e = m;
				d = e;
			}
		}
		sa = sb;
		fa = fb;

		if ( tol < abs ( d ) )
		{
			sb = sb + d;
		}
		else if ( 0.0 < m )
		{
			sb = sb + tol;
		}
		else
		{
			sb = sb - tol;
		}

		fb = ffp ( sb );

		if ( ( 0.0 < fb && 0.0 < fc ) || ( fb <= 0.0 && fc <= 0.0 ) )
		{
			c = sa;
			fc = fa;
			e = sb - sa;
			d = e;
		}
	}
	return sb;
}

T Triode::zeroffg ( T a, T b, T t )
{
	T c;
	T d;
	T e;
	T fa;
	T fb;
	T fc;
	T m;
	T macheps;
	T p;
	T q;
	T r;
	T s;
	T sa;
	T sb;
	T tol;
//
//	Make local copies of A and B.
//
	sa = a;
	sb = b;
	fa = ffg ( sa );
	fb = ffg ( sb );

	c = sa;
	fc = fa;
	e = sb - sa;
	d = e;

	macheps = r8_epsilon;

	for ( ; ; )
	{
		if ( abs ( fc ) < abs ( fb ) )
		{
			sa = sb;
			sb = c;
			c = sa;
			fa = fb;
			fb = fc;
			fc = fa;
		}

		tol = 2.0 * macheps * abs ( sb ) + t;
		m = 0.5 * ( c - sb );

		if ( abs ( m ) <= tol || fb == 0.0 )
		{
			break;
		}

		if ( abs ( e ) < tol || abs ( fa ) <= abs ( fb ) )
		{
			e = m;
			d = e;
		}
		else
		{
			s = fb / fa;

			if ( sa == c )
			{
				p = 2.0 * m * s;
				q = 1.0 - s;
			}
			else
			{
				q = fa / fc;
				r = fb / fc;
				p = s * ( 2.0 * m * q * ( q - r ) - ( sb - sa ) * ( r - 1.0 ) );
				q = ( q - 1.0 ) * ( r - 1.0 ) * ( s - 1.0 );
			}

			if ( 0.0 < p )
			{
				q = - q;
			}
			else
			{
				p = - p;
			}

			s = e;
			e = d;

			if ( 2.0 * p < 3.0 * m * q - abs ( tol * q ) &&
				p < abs ( 0.5 * s * q ) )
			{
				d = p / q;
			}
			else
			{
				e = m;
				d = e;
			}
		}
		sa = sb;
		fa = fb;

		if ( tol < abs ( d ) )
		{
			sb = sb + d;
		}
		else if ( 0.0 < m )
		{
			sb = sb + tol;
		}
		else
		{
			sb = sb - tol;
		}

		fb = ffg ( sb );

		if ( ( 0.0 < fb && 0.0 < fc ) || ( fb <= 0.0 && fc <= 0.0 ) )
		{
			c = sa;
			fc = fa;
			e = sb - sa;
			d = e;
		}
	}
	return sb;
}
