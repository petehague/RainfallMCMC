/*
	Adaptive step size agent
	Author: Peter Hague
	Created: 13/03/15
*/
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <cinttypes>

#include "include/agent.hpp"

using namespace std;


//------------------------------------------------

class adaptive : public agent {	
	uint16_t nparam;
	double *model;
	double *prevmodel;
	double target;
	uint16_t lookback, tick;
	bool **record;
public:
	adaptive() {
		std::cout << "Created adaptive step size" << std::endl;
	}
	
	
	void setup(options *o) {
		target = 0.4;
		lookback = 100;
		nparam=(int)o->getdoubleval("Nparams");
		model = new double[nparam];
		prevmodel = new double[nparam];
		record = new bool*[nparam];
		for (uint16_t i=0;i<nparam;i++)
			record[i] = new bool[lookback];
		tick = 0;
	}

	double invoke(chain *c, options *o) {
		c->last(model);
		if (c->size()<2) return 0.0;
		c->getModel(c->size()-2, prevmodel);
		for (uint16_t i=0;i<nparam;i++) {
			if (model[i]==prevmodel[i]) record[i][tick] = false; else record[i][tick] = true;
		}
			
		tick++;	
		if (tick==lookback) {
			tick=0;
			for(uint16_t i=0;i<nparam;i++) {
				double rate=0.0;
				for(uint16_t j=0;j<nparam;j++) 
					if (record[i][j]) rate+=1.0; 
				rate/=(double)lookback;
				c->setStep(i, c->getStep(i)*(rate/target));
				std::cout << c->getStep(i) << " ";
			}
			std::cout << std::endl;
		}	
		
		return 0;
	}
};

REGISTERAGENT(adaptive)
