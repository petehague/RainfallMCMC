/*
	Blob likelihood function
	Authors: Ewan Ferguson, Peter Hague
	Created: 01/09/14
*/
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>

#include "include/agent.hpp"
#include "include/pick.hpp"

using namespace std;

class blob : public agent {
	int nparam;
	double *model;
	double mean, width;
public:
	blob () {
		mean=0.5;
		width=2*0.1*0.1;
		std::cout << "Created blob: Mean=" << mean << " Width=" << width << std::endl;
	}
	
	void setup(options *o) {
		nparam=(int)o->getdoubleval("Nparams");
		model=new double [nparam];
	}
	
	double invoke(chain *c, options *o){
		double x=1;

		c->current(model);
		for (int i=0; i<nparam; i++)
			x*=exp(-((model[i]-mean)*(model[i]-mean))/(width));
		
		return x;
	}
};

REGISTERAGENT(blob)
