/************************************************************************************
* 
* Wavechild670 v0.1 
* 
* generatedwdfutilities.h
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



#ifndef GENERATEDWDFUTILITIES_H
#define GENERATEDWDFUTILITIES_H

#include "tubemodel.h"

class BidirectionalUnitDelay;

class BidirectionalUnitDelayInterface {
public:
	friend class BidirectionalUnitDelay;
	void setA(Real a_){ a = a_; }
	Real getB() { return b;}
protected:
	Real a;
	Real b;
};

class BidirectionalUnitDelay {
public:
	BidirectionalUnitDelayInterface* getInterface(uint index){
		if (index == 0){
			return &interface0;
		}
		return &interface1;		
	}
	void advance(){
		interface0.b = interface1.a;
		interface1.b = interface0.a;
	}
protected:
	BidirectionalUnitDelayInterface interface0;
	BidirectionalUnitDelayInterface interface1;
};


#endif
