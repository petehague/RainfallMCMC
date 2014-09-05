/*
	MCMC chain object
	Author: Peter Hague
	Created: 20/07/14
*/
#include "include/chain.hpp"
#include "include/options.hpp"
#include "include/pick.hpp"
#include <iostream>

chain::chain() {
}

void chain::init(options *o) {
	double p;
	int i;
	
	width = o->getdoubleval("nparams");
	for (i=0;i<width;i++) {
		//Get parameters from options object
		rangeStart.push_back(o->getdoubleval("lowerlimit", i));
		dRange.push_back((o->getdoubleval("upperlimit", i)-rangeStart.back())/lintmax);
		stepSize.push_back((uint32_t)(o->getdoubleval("stepsize", i)/dRange.back()));
		startMean.push_back(o->getdoubleval("startmean", i));
		startDev.push_back(o->getdoubleval("startdev", i));
		
		//Pick some starting values
		p = startMean.back()+startDev.back()*ransource.getnum();
		data.push_back((uint32_t)((p-rangeStart.back())/dRange.back()));
		buffer.push_back(data[i]);
	}
}

void chain::getModel(uint32_t index, double *output) {
	index*=width;
	for(int i=0;i<width;i++) 
		output[i] = rangeStart[i]+(double)data[index+i]*dRange[i];
}

void chain::last(double *output) {
	int pointer = data.size()-width;
	for(int i=0;i<width;i++) 
  		output[i] = rangeStart[i]+(double)data[pointer+i]*dRange[i];
}

void chain::step() {
	int p, dp, np;
	for(int i=0;i<width;i++) { 
		p = buffer[buffer.size()-(width-1)];
		dp = ransource.getnum() * stepSize[i];
		np = p + dp;
		if (dp>p) np = dp-p;
		if (dp>(lintmax-p)) np=lintmax-(dp-(lintmax-p));
		buffer[i] = np;
	}
}

void chain::repeat() {
	double top[width];
	last(top);
	for(int i=0;i<width;i++)
		data.push_back(top[i]);
}

void chain::push() {
	for(int i=0;i<width;i++) {
		data.push_back(buffer[i]);
	}
}

int chain::size() { 
	return buffer.size()/(double)width;
}