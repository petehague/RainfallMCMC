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
	
	ransource.initialise(1);
	
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
		
		std::cout << "Parameter " << o->getstringval("paramname", i) << ": " << rangeStart[i] << "+n*" << dRange[i] << std::endl;
	}
}

void chain::setStep(uint16_t n, double value) {
	stepSize[n] = value;
}

double chain::getStep(uint16_t n) {
	return stepSize[n];
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

void chain::rawlast(uint32_t *output) {
	int pointer = data.size()-width;
	for(int i=0;i<width;i++) 
  		output[i] = data[pointer+i];
}

void chain::step() {
	int64_t p, np, dp;
	uint32_t top[width];
	rawlast(top);
	for(int i=0;i<width;i++) { 
		p = (int64_t)top[i];
		dp = (int64_t)(ransource.getnum() * (double)stepSize[i]);
		np = p + dp;
		if (np<0) np=-np;
		if (np>lintmax) np=2L*(int64_t)lintmax-np;
		buffer[i] = (uint32_t)np;
	}
}

void chain::current(double *output) {
	for(int i=0;i<width;i++) 
  		output[i] = rangeStart[i]+(double)buffer[i]*dRange[i];
}

void chain::repeat() {
	uint32_t top[width];
	rawlast(top);
	for(int i=0;i<width;i++)
		data.push_back(top[i]);
}

void chain::push() {
	for(int i=0;i<width;i++) {
		data.push_back(buffer[i]);
	}
}

void chain::pop() {
	for(int i=0;i<width;i++) {
		data.pop_back();
	}
}

int chain::size() { 
	return buffer.size()/(double)width;
}