/*
	Filler likelihood estimator
	Author: Peter Hague
	Created: 22/07/14
*/
#include <iostream>
#include <string>

#include "include/agent.hpp"

using namespace std;

class likelihood : public agent {
public:
	likelihood () {
		std::cout << "Created object" << std::endl;
	}
	void setup(options *o) { };
	double invoke(chain *c, options *o);
};

double likelihood::invoke(chain *c, options *o) {
	double model[(int)o->getdoubleval("nparams")];
	c->last(model);
	return model[0]*model[0];
}

REGISTERAGENT(likelihood)
