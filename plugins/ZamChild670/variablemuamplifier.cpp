/************************************************************************************
* 
* Wavechild670 v0.1 
* 
* variablemuamplifier.cpp
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



#include "variablemuamplifier.h"

const Real VariableMuAmplifier::RinputValue = 600.0;
const Real VariableMuAmplifier::RinputTerminationValue = 360.0;
const Real VariableMuAmplifier::inputTxLp = 4e-3;
const Real VariableMuAmplifier::inputTxRp = 10.0;
const Real VariableMuAmplifier::inputTxRc = 10e3;
const Real VariableMuAmplifier::inputTxLm = 35.7;
const Real VariableMuAmplifier::inputTxRs = 50.0;
const Real VariableMuAmplifier::inputTxLs = 1e-3;
const Real VariableMuAmplifier::inputTxCw = 210e-12;
const Real VariableMuAmplifier::inputTxNpOverNs = 2.0/9.0 ;
//1:9 from the Drip Fairchild 670 transformer docs, decreased to meet the desired distortion specs, possibly with good reason given that the two primary sides are connected together!
const Real VariableMuAmplifier::RgateValue = 100e3;
const Real VariableMuAmplifier::VgateBiasConst = -7.2;

//Amplifier
const Real VariableMuAmplifier::numTubeParallelInstances = 2.0;
const Real VariableMuAmplifier::RcathodeValue = 705;
const Real VariableMuAmplifier::CcathodeValue = 8e-6; //Should be twice the number on the Fairchild 670 schematic because there's effectively two of these in series
const Real VariableMuAmplifier::VcathodeBias = -3.1;
const Real VariableMuAmplifier::RoutputValue = 600.0;
const Real VariableMuAmplifier::RsidechainValue = 1000.0; //should only be non-infinite in a feedback topology
const Real VariableMuAmplifier::RplateValue = 33;
const Real VariableMuAmplifier::Vplate = 240.0;

const Real VariableMuAmplifier::outputTxLp = 100e-6;
const Real VariableMuAmplifier::outputTxRp = 5.0;
const Real VariableMuAmplifier::outputTxRc = inputTxRc;
const Real VariableMuAmplifier::outputTxLm = inputTxLm;
const Real VariableMuAmplifier::outputTxRs = 1.0;
const Real VariableMuAmplifier::outputTxLs = 400e-6;
const Real VariableMuAmplifier::outputTxCw = 1e-12;
const Real VariableMuAmplifier::outputTxNpOverNs = 1.0/inputTxNpOverNs; //Somehow this gets inverted in the wdf code
