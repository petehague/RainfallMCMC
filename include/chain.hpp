#ifndef CHAIN
#define CHAIN

#include <vector>
#include <cinttypes> 

#include "pick.hpp"
#include "options.hpp"

using std::vector;

const double lintmax=4294967295; 

class chain {
	uint16_t width;
	vector <uint32_t> data;
	vector <uint32_t> buffer;
	vector <double> rangeStart;
	vector <double> dRange;
	vector <uint32_t> stepSize;
	vector <double> startMean;
	vector <double> startDev;
	generator ransource;
public:
	chain(); 
	void init(options *o);
	void getModel(uint32_t index, double *output);
	void setStep(uint16_t n, double val);
	double getStep(uint16_t n);
	void last(double *output);
	void current(double *output);
	void rawlast(uint32_t *output);
	void push();
	void pop();
	void step();
	void repeat();
	int size();
};

#endif