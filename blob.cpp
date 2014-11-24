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
	double meanx, meany, width, angle;
	generator ransource;
	chrono::system_clock::time_point startpoint;
public:
	blob () {
		startpoint = chrono::system_clock::now();
		ransource.initialise(startpoint.time_since_epoch().count()+1000);
		meanx=0.3+ransource.flatnum()*0.2;
		meany=0.3+ransource.flatnum()*0.2;
		width=ransource.flatnum()*0.3*0.1*0.1;
		angle=ransource.flatnum()*pi;
		std::cout << "Created blob: Mean=" << meanx << "," << meany << " Width=" << width << " " << "Angle=" << angle << std::endl;
	}
	
	void setup(options *o) {
		nparam=(int)o->getdoubleval("Nparams");
	}
	
	double eval(double *model) {
		double x, y, result=1;
		
		x = (model[0]-meanx)*cos(angle) + (model[1]-meany)*sin(angle);
		y = (model[1]-meany)*cos(angle) - (model[0]-meanx)*sin(angle);
		
		result*=exp(-(x*x)/(width));
		result*=exp(-(y*y)/(width*3.0));
		
		return result;
	}
};

REGISTERAGENT(blob)
