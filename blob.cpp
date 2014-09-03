/*
	blob
	Author: Ewan Ferguson
	Created: 01/09/14
*/
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>

#include "include/agent.hpp"
#include "include/pick.hpp"
#include "include/options.hpp"


using namespace std;
const double pii = 6.28318530718;
class blob : public agent {
	int nparam;
	double *model;
	double scale, mean, width;
public:
	blob () {
		std::cout << "blob created " << std::endl;
		scale=1/(0.1*sqrt(pii));
		mean=0.5;
		width=2*0.1*0.1;
	}
	int status() const { return 4; }
	double invoke(chain *c, options *o);
};

double blob::invoke(chain *c, options *o){
	double x, y;
	int i;
	nparam=(int)o->getdoubleval("Nparams");
	model=new double [nparam];
	x=1;
	for (i=0; i < nparam; i++){
	y=scale*exp(((x-mean)*(x-mean))/(width));
	x*=y;
	}
	delete model;
	return x;

}

REGISTERAGENT(blob)
