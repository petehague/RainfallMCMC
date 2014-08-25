/*
	MCMC chain object
	Author: Peter Hague
	Created: 20/07/14
*/
#include "include/chain.hpp"
#include "include/options.hpp"
#include "include/pick.hpp"

chain::chain(uint32_t l, uint16_t w) {
	length = l;
	width = w;
	buffer = new uint32_t[l*w];
	rangeStart = new double[w];
	dRange = new double[w];
	stepSize = new uint32_t[w];
	pointer = 0;
} 

chain::chain() {
}

void chain::setup(uint32_t l, uint16_t w) {
	length = l;
	width = w;
	buffer = new uint32_t[l*w];
	rangeStart = new double[w];
	dRange = new double[w];
	stepSize = new uint32_t[w];
	pointer = 0;
}

void chain::setParam(int n, double lower, double upper, double step, double start, double dev) {
	rangeStart[n] = lower;
	dRange[n] = (upper-lower)/lintmax;
	stepSize[n] = (uint32_t)(step/dRange[n]);
	startMean[n] = start;
	startDev[n] = dev;
}

void chain::init() {
	pointer = 0;
	for (int i=0;i<width;i++) {
		double p = startMean[i] + startDev[i]; //Add random call
		buffer[i] = (uint32_t)((p-rangeStart[i])/dRange[i]);
	}
}

void chain::getModel(uint32_t index, double *output) {
  index*=width;
  for(int i=0;i<width;i++) 
  	output[i] = rangeStart[i]+(double)buffer[index+i]*dRange[i];
}

void chain::last(double *output) {
  for(int i=0;i<width;i++) 
  	output[i] = rangeStart[i]+(double)buffer[pointer+i]*dRange[i];
}

void chain::push(double *p) {
	for(int i=0;i<width;i++) 
		buffer[pointer+i]=(p[i]-rangeStart[i])/dRange[i];
	pointer+=width;
}

int chain::size() { 
	return (double)pointer/(double)width;
}