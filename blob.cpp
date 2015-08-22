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

const double pi=3.14159265;

class blob : public agent {
	int nparam;
	double meanx, meany, width, angle, asp;
	generator ransource;
	chrono::system_clock::time_point startpoint;
public:
	blob () {
		startpoint = chrono::system_clock::now();
		ransource.initialise(startpoint.time_since_epoch().count()+1000);
		meanx=0.3+ransource.getFlat()*0.2;
		meany=0.3+ransource.getFlat()*0.2;
		width=0.01+ransource.getFlat()*0.1;
		asp=1.0+ransource.getFlat()*2.0;
		angle=ransource.getFlat()*pi;
		std::cout << "Created blob: Mean=" << meanx << "," << meany << " Width=" << width
			 << " Aspect ratio=" << asp << " Angle=" << angle << std::endl;
	}

	void setup(options *o) {
		nparam=(int)o->getdoubleval("Nparams");
	}

	double eval(double *model) {
		double x, y, result=1;

		x = (model[0]-meanx)*cos(angle) + (model[1]-meany)*sin(angle);
		y = (model[1]-meany)*cos(angle) - (model[0]-meanx)*sin(angle);

		result*=exp(-(x*x)/(width*width));
		result*=exp(-(y*y)/(width*width*asp*asp));

		return result;
	}
};

REGISTERAGENT(blob)
