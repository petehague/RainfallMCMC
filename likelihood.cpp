/*
	Likelihood Template
	Author: Peter Hague
	Created: 22/07/14
*/
#include <iostream>

#include "include/agent.hpp"

using namespace std;

class likelihood : public agent {
	double *model;
public:
	likelihood () {
		std::cout << "Created object" << std::endl; //Report the name of the module
	}
	
	void setup(options *o) {
		model = new double[(int)o->getdoubleval("nparams")];
		//Run at the beginning of the chain
	}
	
	double invoke(chain *c, options *o){
		c->last(model);
		//Return the likelihood of "model"
		return 0;
	}
};

REGISTERAGENT(likelihood)
