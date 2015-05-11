/************************************************************************************
* 
* Wavechild670 v0.1 
* 
* sidechainamplifier.cpp
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



#include "sidechainamplifier.h"

//Input stage
const Real SidechainAmplifier::RinSeriesValue = 600;
const Real SidechainAmplifier::RinParallelValue = 1360;
const Real SidechainAmplifier::Lp = 2e-3;
const Real SidechainAmplifier::Rp = 10.0;
const Real SidechainAmplifier::Rc = 50e3;
const Real SidechainAmplifier::Lm = 5.7;
const Real SidechainAmplifier::Rs = 50.0;
const Real SidechainAmplifier::Ls = 1e-3;
const Real SidechainAmplifier::Cw = 10e-12;
const Real SidechainAmplifier::NpOverNs = 1.0/17.0;
const Real SidechainAmplifier::RpotValue = 2*76e3; //includes the 25k center tap resistor. The pot itself is 100k. Two in series is the load seen by the sidechain input, while the input voltage to the sidechain amp is the voltage across just one of them.

//DC Threshold stage, 12AX7 amplifier
const Real SidechainAmplifier::DCThresholdScaleFactor = 12.2;
const Real SidechainAmplifier::DCThresholdOffset = 0.1;
const Real SidechainAmplifier::VscScaleFactor = -6.0;

//Drive stage, 12BH7 + 6973 amplifier stages
const Real SidechainAmplifier::overallVoltageGain = 8.4; //Used to be 17
const Real SidechainAmplifier::finalOutputClipVoltage = 100.0;
const Real SidechainAmplifier::diodeDropX2 = 0.6; //Twice the diode voltage drop, germanium diodes
const Real SidechainAmplifier::nominalOutputConductance = 1.0 / 160.0; //ohms. Used to be 80
const Real SidechainAmplifier::maxOutputCurrent = 0.5; //amps
