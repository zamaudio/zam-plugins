/************************************************************************************
* 
* Wavechild670 v0.1 
* 
* variablemuamplifier.h
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



#ifndef VARIABLEMUAMPLIFIER_H
#define VARIABLEMUAMPLIFIER_H

#include "glue.h"
#include "wdfcircuits.h"
#include "tubemodel.h"

#define CATHODE_CAPACITOR_CONN_R 1e-6

class VariableMuAmplifier {
	/*
	Simulation of a variable-Mu tube amplifier using the 6386 remote-cutoff tube 
	Peter Raffensperger
	2012 
	
	References:
	"Wave Digital Simulation of a Vacuum-Tube Amplifier"
	By M. Karjalainen and J. Pakarinen, ICASSP 2006

	*/
public:
	VariableMuAmplifier(Real sampleRate) : inputCircuit(inputTxCw, 0.0, inputTxLm, inputTxLp, inputTxLs, inputTxNpOverNs, inputTxRc, RinputTerminationValue, RgateValue, inputTxRp, inputTxRs, RinputValue, sampleRate),
	tubeModelInterface(new TriodeRemoteCutoff6386(), numTubeParallelInstances),
	tubeAmpPush(CcathodeValue, outputTxCw, VcathodeBias, Vplate, outputTxLm, outputTxLp, outputTxLs, outputTxNpOverNs, outputTxRc, 	RoutputValue, outputTxRp, outputTxRs, RsidechainValue, RcathodeValue, RplateValue, CATHODE_CAPACITOR_CONN_R, cathodeCapacitorConn.getInterface(0), sampleRate, tubeModelInterface),
	tubeAmpPull(CcathodeValue, outputTxCw, VcathodeBias, Vplate, outputTxLm, outputTxLp, outputTxLs, outputTxNpOverNs, outputTxRc, 	RoutputValue, outputTxRp, outputTxRs, RsidechainValue, RcathodeValue, RplateValue, CATHODE_CAPACITOR_CONN_R, cathodeCapacitorConn.getInterface(1), sampleRate, tubeModelInterface) {
	
	}
	virtual ~VariableMuAmplifier() { }
	
	virtual Real advanceAndGetOutputVoltage(Real inputVoltage, Real VlevelCap){
		Assert(!isnan(inputVoltage));
		Assert(!isnan(VlevelCap));
		Real Vgate = inputCircuit.advance(inputVoltage);
		Assert(!isnan(Vgate));		
		Real VoutPush = tubeAmpPush.advance(VgateBiasConst - VlevelCap + Vgate);
		Real VoutPull = tubeAmpPull.advance(VgateBiasConst - VlevelCap - Vgate);
		cathodeCapacitorConnector.advance();
		return VoutPush - VoutPull;
	}
protected:
	//Input circuit
	TransformerCoupledInputCircuit inputCircuit;
	BidirectionalUnitDelay cathodeCapacitorConn;

	//Amplifier
	WDFTubeInterface tubeModelInterface;
	BidirectionalUnitDelay cathodeCapacitorConnector;
	TubeStageCircuit tubeAmpPull;
	TubeStageCircuit tubeAmpPush;

	//Input circuit
	static const Real RinputValue;
	static const Real RinputTerminationValue;
	static const Real inputTxLp;
	static const Real inputTxRp;
	static const Real inputTxRc;
	static const Real inputTxLm;
	static const Real inputTxRs;
	static const Real inputTxLs;
	static const Real inputTxCw;
	static const Real inputTxNpOverNs;
	static const Real RgateValue;
	static const Real VgateBiasConst;

	//Amplifier
	static const Real numTubeParallelInstances;
	
	static const Real RcathodeValue; 
	static const Real CcathodeValue;  //Should be twice the number on the Fairchild 670 schematic because there's effectively two of these in series
	static const Real VcathodeBias; 
	static const Real RoutputValue; 
	static const Real RsidechainValue;  //should only be non-infinite in a feedback topology
	static const Real RplateValue; 
	static const Real Vplate; 
	
	static const Real outputTxLp;	
	static const Real outputTxRp; 
	static const Real outputTxRc; 
	static const Real outputTxLm; 
	static const Real outputTxRs; 
	static const Real outputTxLs; 
	static const Real outputTxCw; 
	static const Real outputTxNpOverNs; 
};


#endif 
