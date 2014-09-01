#ifndef CHAIN
#define CHAIN

#include <vector>
#include <cinttypes> 

#include "pick.hpp"

using std::vector;

const double lintmax=4294967295; 

class chain {
	uint32_t length;
	uint16_t width;
	uint32_t *buffer;
	uint64_t pointer;
	double *rangeStart;
	double *dRange;
	uint32_t *stepSize;
	double *startMean;
	double *startDev;
	generator ransource;
public:
	chain(uint32_t l, uint16_t w);
	chain();
	void setup(uint32_t l, uint16_t w);
	void setParam(int n, double lower, double upper, double step, double start, double dev); 
	void init();
	void getModel(uint32_t index, double *output);
	void last(double *output);
	void push(double *p);
	int size();
};

#endif