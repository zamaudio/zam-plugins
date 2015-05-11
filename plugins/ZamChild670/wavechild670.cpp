/************************************************************************************
* 
* Wavechild670 v0.1 
* 
* wavechild670.cpp
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



#include "wavechild670.h"

const Real Wavechild670::levelTimeConstantCircuitComponentValues[6][6] = {
	/* C1,    C2,   C3,   R1,   R2,    R3 */
	{ 2e-6, 8e-6, 20e-6, 51.9e3, 10e9, 10e9 },
	{ 2e-6, 8e-6, 20e-6, 149.9e3, 10e9, 10e9 },
	{ 4e-6, 8e-6, 20e-6, 220e3, 10e9, 10e9 },
	{ 8e-6, 8e-6, 20e-6, 220e3, 10e9, 10e9 },
	{ 4e-6, 8e-6, 20e-6, 220e3, 100e3, 10e9 },
	{ 2e-6, 8e-6, 20e-6, 220e3, 100e3, 100e3 }};
