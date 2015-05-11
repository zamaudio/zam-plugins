/************************************************************************************
* 
* Wavechild670 v0.1 
* 
* tubemodel.h
* 
* By Peter Raffensperger 10 July 2012
* 
* Reference:
* Toward a Wave Digital Filter Model of the Fairchild 670 Limiter, Raffensperger, P. A., (2012). 
* Proc. of the 15th International Conference on Digital Audio Effects (DAFx-12), 
* York, UK, September 17-21, 2012.
* 
* Note:
* Fairchild (R) a registered trademark of Avid Technology, Inc., which is in no way associated or 
* affiliated with the author.
* 
* License:
* Wavechild670 is licensed under the GNU GPL v2 license. If you use this
* software in an academic context, we would appreciate it if you referenced the original
* paper.
* 
************************************************************************************/



#ifndef TUBEMODEL_H
#define TUBEMODEL_H

#include "glue.h"

class TriodeModel {
public:
	TriodeModel() {}
	virtual ~TriodeModel() {}
	virtual Real getIa(Real Vgk, Real Vak) { 
		return 0.0; 
	}
	virtual Real getIg(Real Vgk, Real Vak) { 
		return 0.0; 
	}

	virtual TriodeModel* clone() const { return new TriodeModel(*this); }
private:
	TriodeModel(const TriodeModel& other) { }
};

class TriodeRemoteCutoff6386 : public TriodeModel {
public:
	TriodeRemoteCutoff6386() : VgkLast(1.0), numeratorLast(0.0) { }
	virtual ~TriodeRemoteCutoff6386() { }
	
	virtual Real getIa(Real Vgk, Real Vak){
		if (Vak < 0.0) {
			Vak = 0.0;
		}
		if (Vgk > 0.0) {
			Vgk = 0.0;
		}
		Real p1 = 3.981e-8;
		Real p2 = 2.383;
		Real p3 = 0.5;
		Real p4 = 0.1;
		Real p5 = 1.8;
		Real p6 = 0.5;
		Real p7 = -0.03922;
		Real p8 = 0.2;
		Real iakAlt = p1*pow(Vak, p2) / (pow((p3-p4*Vgk), p5)*(p6+exp(p7*Vak-p8*Vgk)));
		return iakAlt;
		
	}
	virtual TriodeModel* clone() const { return new TriodeRemoteCutoff6386(*this); }	

private:
	TriodeRemoteCutoff6386(const TriodeRemoteCutoff6386& other) {
		VgkLast = other.VgkLast; 
		numeratorLast = other.numeratorLast; 
	}

protected:

	static inline Real getA(Real Vak){
		return aa*pow(Vak, ab);
	}
	static inline Real getF(Real Vak){
		return fa*Vak;
	}			
	
protected:
	Real VgkLast;
	Real numeratorLast;
	
	static const Real fa;
	static const Real aa;
	static const Real ab;
	static const Real e;
	static const Real d;
	static const Real c;
	static const Real g;
	static const Real h;	
};

class WDFTubeInterface {
public:
	WDFTubeInterface() { model = NULL; }
	WDFTubeInterface(TriodeModel *model_, Real numParallelInstances_=3.0) : model(model_), 
	numParallelInstances(numParallelInstances_) {
		a = 0.0;
		Vgk = 0.0;
		Iak = 0.0;
		VakGuess = 100.0;
	}
	~WDFTubeInterface() { delete model; }
	
	WDFTubeInterface(WDFTubeInterface& other) {
		model = other.model->clone();
		numParallelInstances = other.numParallelInstances;
		a = other.a;
		Vgk = other.Vgk;
		Iak = other.Iak;
		VakGuess = other.VakGuess;
	}
	
	Real getB(Real a_, Real r0_, Real Vgate, Real Vk){
		Assert(model);
		/*
		Reference:
		"Wave Digital Simulation of a Vacuum-Tube Amplifier"
		By M. Karjalainen and J. Pakarinen, ICASSP 2006

		Vak + R0*f(Vgk, Vak) - a = 0 	#[Karjalainen and Pakarinen, eq 7]
		b = Vak - Ro*f(Vgk, Vak)		#[Karjalainen and Pakarinen, eq 8]
		*/
		
		r0 = r0_;
		
		a = a_;
		
		Vgk = Vgate - Vk;

		Real Vak = VakGuess;
		uint iteration = 0;
		Real err = 1e6;
		Iak = 0.0;
		
		while (fabs(err)/fabs(Vak) > 1e-9){
			VakGuess = iterateNewtonRaphson(Vak);
			err = Vak - VakGuess;
			Vak = VakGuess;

			if (iteration > 100){
				break;
			}
			++iteration;
		}
		Real b = Vak - r0*Iak;
		/*
		a = v + Ri
		b = v - Ri
		v = a + b
		*/
		return b;
	}

protected:
	Real evaluateImplicitEquation(Real Vak){
		Assert(!isnan(Vak));
		Assert(!isnan(Vgk));		
		Iak = model->getIa(Vgk, Vak) * numParallelInstances;
		Assert(!isnan(Iak));
		return Vak + r0*Iak - a;
	}
	Real iterateNewtonRaphson(Real x, Real dxFactor=1e-6);

	Real numParallelInstances;

	Real r0;
	Real a;
	Real Vgk;
	Real Iak;
	Real VakGuess;
	TriodeModel *model;
};

#endif
