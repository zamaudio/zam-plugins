/************************************************************************************
* 
* Wavechild670 v0.1 
* 
* wavechild670.h
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



#ifndef WAVECHILD670_H
#define WAVECHILD670_H

#include "glue.h"
#include "sidechainamplifier.h"
#include "variablemuamplifier.h"

#define LEVELTC_CIRCUIT_DEFAULT_C_C1 2e-6
#define LEVELTC_CIRCUIT_DEFAULT_C_C2 8e-6
#define LEVELTC_CIRCUIT_DEFAULT_C_C3 20e-6
#define LEVELTC_CIRCUIT_DEFAULT_R_R1 220e3
#define LEVELTC_CIRCUIT_DEFAULT_R_R2 1e9
#define LEVELTC_CIRCUIT_DEFAULT_R_R3 1e9


class Wavechild670Parameters {
public:
	Wavechild670Parameters(Real inputLevelA_, Real ACThresholdA_, uint timeConstantSelectA_, Real DCThresholdA_, 
	Real inputLevelB_, Real ACThresholdB_, uint timeConstantSelectB_, Real DCThresholdB_, 
	bool sidechainLink_, bool isMidSide_, bool useFeedbackTopology_, Real outputGain_, bool hardClipOutput_){
		inputLevelA = inputLevelA_;
		ACThresholdA = ACThresholdA_; 
		timeConstantSelectA = timeConstantSelectA_; 
		DCThresholdA = DCThresholdA_; 
				
		inputLevelB = inputLevelB_; 
		ACThresholdB = ACThresholdB_; 
		timeConstantSelectB = timeConstantSelectB_; 
		DCThresholdB = DCThresholdB_; 
				
		sidechainLink = sidechainLink_; 
		isMidSide = isMidSide_; 
		useFeedbackTopology = useFeedbackTopology_;
		outputGain = outputGain_;
		hardClipOutput = hardClipOutput_;
		
	}
	virtual ~Wavechild670Parameters() {}
public:
	Real inputLevelA;
	Real ACThresholdA; 
	uint timeConstantSelectA; 
	Real DCThresholdA; 
	
	Real inputLevelB; 
	Real ACThresholdB; 
	uint timeConstantSelectB; 
	Real DCThresholdB; 
	
	bool sidechainLink; 
	bool isMidSide; 
	bool useFeedbackTopology;
	
	Real outputGain;
	bool hardClipOutput;
private:
	Wavechild670Parameters() {}
};

class Wavechild670 {
public:
	Wavechild670(Real sampleRate_, Wavechild670Parameters& parameters) : 
	sampleRate(sampleRate_),
	useFeedbackTopology(parameters.useFeedbackTopology), isMidSide(parameters.isMidSide), sidechainLink(parameters.sidechainLink),
	sidechainAmplifierA(sampleRate, parameters.ACThresholdA, parameters.DCThresholdA), sidechainAmplifierB(sampleRate, parameters.ACThresholdB, parameters.DCThresholdB), 
	levelTimeConstantCircuitA(LEVELTC_CIRCUIT_DEFAULT_C_C1, LEVELTC_CIRCUIT_DEFAULT_C_C2, LEVELTC_CIRCUIT_DEFAULT_C_C3, LEVELTC_CIRCUIT_DEFAULT_R_R1, LEVELTC_CIRCUIT_DEFAULT_R_R2, LEVELTC_CIRCUIT_DEFAULT_R_R3, sampleRate), 
	levelTimeConstantCircuitB(LEVELTC_CIRCUIT_DEFAULT_C_C1, LEVELTC_CIRCUIT_DEFAULT_C_C2, LEVELTC_CIRCUIT_DEFAULT_C_C3, LEVELTC_CIRCUIT_DEFAULT_R_R1, LEVELTC_CIRCUIT_DEFAULT_R_R2, LEVELTC_CIRCUIT_DEFAULT_R_R3, sampleRate), 
	VlevelCapA(0.0), VlevelCapB(0.0),
	signalAmplifierA(sampleRate), signalAmplifierB(sampleRate), inputLevelA(parameters.inputLevelA), inputLevelB(parameters.inputLevelB) {
		setParameters(parameters);
	}
	virtual ~Wavechild670() {}

	virtual void setParameters(Wavechild670Parameters& parameters){
		inputLevelA = parameters.inputLevelA;
		sidechainAmplifierA.setThresholds(parameters.ACThresholdA, parameters.DCThresholdA); 
				
		inputLevelB = parameters.inputLevelB; 
		sidechainAmplifierB.setThresholds(parameters.ACThresholdB, parameters.DCThresholdB); 
		
		select670TimeConstants(parameters.timeConstantSelectA, parameters.timeConstantSelectB);
		
		sidechainLink = parameters.sidechainLink; 
		isMidSide = parameters.isMidSide; 
		useFeedbackTopology = parameters.useFeedbackTopology;		
		outputGain = parameters.outputGain;
		hardClipOutput = parameters.hardClipOutput;
	}
	
	virtual void warmUp(Real warmUpTimeInSeconds=0.5){
		Real VoutA, VoutB;
		ulong numSamples = (ulong) warmUpTimeInSeconds*sampleRate;
		for (ulong i = 0; i < numSamples/2; i += 1) {
			VoutA = signalAmplifierA.advanceAndGetOutputVoltage(0.0, VlevelCapA);
			VoutB = signalAmplifierB.advanceAndGetOutputVoltage(0.0, VlevelCapB);
		}
		for (ulong i = 0; i < numSamples/2; i += 1) {
			VoutA = signalAmplifierA.advanceAndGetOutputVoltage(0.0, VlevelCapA);
			VoutB = signalAmplifierB.advanceAndGetOutputVoltage(0.0, VlevelCapB);
			advanceSidechain(VoutA, VoutB); //Feedback topology with implicit unit delay between the sidechain input and the output, 
		}
	}

	//virtual void process(Real *VinputLeft, Real *VinputRight, Real *VoutLeft, Real *VoutRight, ulong numSamples) {
	virtual void process(const float **VinputInterleaved, float **VoutInterleaved, ulong numSamples) {
		Assert(VinputInterleaved);
		Assert(VoutInterleaved);
		static uint numChannels = 2;
		
		for (ulong i = 0; i < numSamples; i += numChannels) {
			Real VinputA;
			Real VinputB;
			if (isMidSide) {
				VinputA = (double)((VinputInterleaved[0][i]) + (VinputInterleaved[0][i]))/sqrt(2.0);
				VinputB = (double)((VinputInterleaved[1][i]) - (VinputInterleaved[1][i]))/sqrt(2.0);
			}
			else {
				VinputA = (double)(VinputInterleaved[0][i]);
				VinputB = (double)(VinputInterleaved[1][i]);
			}
			VinputA *= inputLevelA;
			VinputB *= inputLevelB;
			
			
			if (!useFeedbackTopology) { // => Feedforward
				advanceSidechain(VinputA, VinputB); //Feedforward topology
			}
			Real VoutA = signalAmplifierA.advanceAndGetOutputVoltage(VinputA, VlevelCapA);
			Real VoutB = signalAmplifierB.advanceAndGetOutputVoltage(VinputB, VlevelCapB);
			if (useFeedbackTopology) {
				advanceSidechain(VoutA, VoutB); //Feedback topology with implicit unit delay between the sidechain input and the output, 
				//and probably an implicit unit delay between the sidechain capacitor voltage input and the capacitor voltage 
				//(at least they're not the proper WDF coupling between the two)
			}
			
			
			Real VoutLeft;
			Real VoutRight;			
			
			if (isMidSide) {
				VoutLeft = (VoutA + VoutB)/sqrt(2.0);
				VoutRight  = (VoutA - VoutB)/sqrt(2.0);
			}
			else {
				VoutLeft = VoutA;
				VoutRight = VoutB;
			}
			VoutLeft = clip(VoutLeft * outputGain);
			VoutRight = clip(VoutRight * outputGain);
			
			VoutInterleaved[0][i] = (float)VoutLeft;
			VoutInterleaved[1][i] = (float)VoutRight;
		}
	}

protected:
	virtual void select670TimeConstants(uint tcA, uint tcB){
		Assert(tcA >= 1);
		Assert(tcA <= 6);
		tcA -= 1;
		levelTimeConstantCircuitA.updateRValues(levelTimeConstantCircuitComponentValues[tcA][0], levelTimeConstantCircuitComponentValues[tcA][1], levelTimeConstantCircuitComponentValues[tcA][2], levelTimeConstantCircuitComponentValues[tcA][3], levelTimeConstantCircuitComponentValues[tcA][4], levelTimeConstantCircuitComponentValues[tcA][5], sampleRate);
		Assert(tcB >= 1);
		Assert(tcB <= 6);
		tcB -= 1;
		levelTimeConstantCircuitA.updateRValues(levelTimeConstantCircuitComponentValues[tcB][0], levelTimeConstantCircuitComponentValues[tcB][1], levelTimeConstantCircuitComponentValues[tcB][2], levelTimeConstantCircuitComponentValues[tcB][3], levelTimeConstantCircuitComponentValues[tcB][4], levelTimeConstantCircuitComponentValues[tcB][5], sampleRate);
	}

	virtual void advanceSidechain(Real VinSidechainA, Real VinSidechainB) {
	
		Real sidechainCurrentA = sidechainAmplifierA.advanceAndGetCurrent(VinSidechainA, VlevelCapA);
		Real sidechainCurrentB = sidechainAmplifierB.advanceAndGetCurrent(VinSidechainB, VlevelCapB);
		if (sidechainLink) {
			Real sidechainCurrentTotal = (sidechainCurrentA + sidechainCurrentB)/2.0;// #Effectively compute the two circuits in parallel, crude but effective (I haven't prove this is exactly right)
			Real VlevelCapAx = levelTimeConstantCircuitA.advance(sidechainCurrentTotal);
			Real VlevelCapBx = levelTimeConstantCircuitB.advance(sidechainCurrentTotal); // #maintain the voltage in circuit B in case the user disengages the link
			VlevelCapA = (VlevelCapAx + VlevelCapBx) / 2.0;
			VlevelCapB = (VlevelCapAx + VlevelCapBx) / 2.0;
		}
		else {
			VlevelCapA = levelTimeConstantCircuitA.advance(sidechainCurrentA);
			VlevelCapB = levelTimeConstantCircuitB.advance(sidechainCurrentB);
		}
	}	

	static Real clip(Real in)
	{
		Real out = in;
		if (in < -1.0)
			out = -1.0;
		if (in > 1.0)
			out = 1.0;
		return out;
	}

protected:
	Real sampleRate;
	Real outputGain;
	bool hardClipOutput;
	
	Real VlevelCapA;
	Real VlevelCapB;

	Real inputLevelA;
	Real inputLevelB;

	bool useFeedbackTopology;
	bool isMidSide;
	bool sidechainLink;
	SidechainAmplifier sidechainAmplifierA;
	SidechainAmplifier sidechainAmplifierB;
	LevelTimeConstantCircuit levelTimeConstantCircuitA;
	LevelTimeConstantCircuit levelTimeConstantCircuitB;
	VariableMuAmplifier signalAmplifierA;
	VariableMuAmplifier signalAmplifierB;
	
	static const Real levelTimeConstantCircuitComponentValues[6][6];
};

#endif
