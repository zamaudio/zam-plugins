/************************************************************************************
* 
* Wavechild670 v0.1 
* 
* sidechainamplifier.h
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



#ifndef SIDECHAINAMPLIFIER_H
#define SIDECHAINAMPLIFIER_H

#include "glue.h"
#include "wdfcircuits.h"

class SidechainAmplifier {
public:
	SidechainAmplifier(Real sampleRate, Real ACThresholdNew, Real DCThresholdNew) : inputCircuit(Cw, 0.0, Lm, Lp, Ls, NpOverNs, Rc, RinParallelValue, RpotValue, Rp, Rs, RinSeriesValue, sampleRate) {
		setThresholds(ACThresholdNew, DCThresholdNew);
				
		calls = 0;
		currentOvers = 0;

	}
	virtual ~SidechainAmplifier(){
	}
	
	virtual void setThresholds(Real ACThresholdNew, Real DCThresholdNew){
		Assert(DCThresholdNew >= 0.0);
		Assert(DCThresholdNew <= 1.0);

		Assert(ACThresholdNew >= 0.0);
		Assert(ACThresholdNew <= 1.0);

		DCThresholdProcessed = -DCThresholdScaleFactor*(DCThresholdNew + DCThresholdOffset);
		ACThresholdProcessed = 0.5*ACThresholdNew*ACThresholdNew; //A nice curve that approximates the piecewise linear taper on the centre-potted tap curve on the Fairchild 670		
	}
	
	virtual Real advanceAndGetCurrent(Real VinSidechain, Real VlevelCap) {
		Assert(!isnan(VinSidechain));
		Assert(!isnan(VlevelCap));
		calls++;
		Real VgPlus = ACThresholdProcessed*inputCircuit.advance(VinSidechain);
		Assert(!isnan(VgPlus));
		Real Vsc = getDCThresholdStageVsc(VgPlus);
		Confirm(!isnan(Vsc));		
		Real Vamp = Vsc * overallVoltageGain;
		//Vamp = BasicDSP::clip(Vamp, -finalOutputClipVoltage, finalOutputClipVoltage); //Voltage saturation of the final output stage
		Real Vdiff = fabs(Vamp) - VlevelCap;

		Real Iout = getDriveStageCurrent(Vdiff, VlevelCap);
		Confirm(!isnan(Iout));		
		return Iout;
	}
protected:
				
	inline Real getDCThresholdStageVsc(Real VgPlus) {
		Real xp = log1p(exp(VgPlus + DCThresholdProcessed));
		Real xm = log1p(exp(-VgPlus + DCThresholdProcessed));
		Real x = xp - xm;
		return VscScaleFactor*x;
	}
	
	inline Real sidechainAmplifierCurrentSaturation(Real i) {
		Assert(isfinite(i));
		//One side-saturation (does not saturate negatives)
		const Real b = 10.0/maxOutputCurrent;
		const Real c = 10.0;
		Real isat = log1p(exp(b*i-c))/b;
		isat = fmin(isat, i);
		Confirm(isfinite(isat));
		if (i > maxOutputCurrent) {
			currentOvers += 1;
		}
		return i - isat;
	}
	
	inline Real diodeModel(Real V) {
		const Real b = 10.0/diodeDropX2;
		const Real c = 10.0;
		if (V < 20.0){
			return log1p(exp(b*V-c))/b;
		}
		else{
			return V - diodeDropX2;
		}
	}
	
// 	inline Real resistorPlusDiodeModel(Real Vdiff) {
// 		const Real isat = diodeModelBase(0.0);
// 		Real i = diodeModelBase(Vdiff) - isat;
// 		return i;
// 	}
	
	inline Real getDriveStageCurrent(Real Vdiff, Real) {
		Assert(isfinite(Vdiff));		
		Assert(isfinite(Vcap));
		Real current = diodeModel(Vdiff) * nominalOutputConductance;
		Confirm(isfinite(current));
		current = sidechainAmplifierCurrentSaturation(current);
		return current;
	}
	
protected:
	uint calls;
	uint currentOvers;

	Real ACThresholdProcessed;
	Real DCThresholdProcessed;

	TransformerCoupledInputCircuit inputCircuit;
	
	//Input stage
	static const Real RinSeriesValue;
	static const Real RinParallelValue;
	static const Real Lp;
	static const Real Rp;
	static const Real Rc;
	static const Real Lm;
	static const Real Rs;
	static const Real Ls;
	static const Real Cw;
	static const Real NpOverNs;
	static const Real RpotValue; // = 2*76e3 //include the 25k center tap resistor. The pot itis 100k. Two in series is the load seen by the sidechain input, while the input voltage to the sidechain amp is the voltage across just one of them.
	
	//DC Threshold stage, 12AX7 amplifier
	static const Real DCThresholdScaleFactor; // 12.2
	static const Real DCThresholdOffset; // 0.1
	static const Real VscScaleFactor; // -6.0
		
	//Drive stage, 12BH7 + 6973 amplifier stages
	static const Real overallVoltageGain; // 17 //17 seemed like the gain of the drive stage in my SPICE simulation, but this number was then empirically fiddled to better match the performance implied by Fairchild 670 manual (and to get more compression)
	static const Real finalOutputClipVoltage; // 100.0
	static const Real diodeDropX2; // 1.5 //Twice the diode voltage drop
	static const Real nominalOutputConductance; // 1.0/80.0 //ohms
	static const Real maxOutputCurrent; // 0.5 //amps
	
};

#endif
