/*
	Filler likelihood estimator
	Author: Peter Hague
	Created: 22/07/14
*/
#include <iostream>
#include <string>
#include <cmath>

const double pi = 3.141592;

#include "include/agent.hpp"

using namespace std;

class likelihood : public agent {
	double *model;
public:
	likelihood () {
		std::cout << "Created object" << std::endl;
	}
	void setup(options *o);
	double invoke(chain *c, options *o);
};

void likelihood::setup(options *o) {
	model = new double[(int)o->getdoubleval("nparams")];
}

double likelihood::invoke(chain *c, options *o) {
	c->last(model);
	return -sin(model[0]*pi);
}

REGISTERAGENT(likelihood)
