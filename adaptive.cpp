/*
	Adaptive step size agent
	Author: Peter Hague
	Created: 13/03/15
*/
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <cinttypes>

#include "include/agent.hpp"

using namespace std;


//------------------------------------------------

class adaptive : public agent {
	uint16_t nparam;
	uint32_t *model;
	uint32_t *prevmodel;
	double target;
	uint16_t lookback, tick;
	string filename;
	bool **record;
	vector <double>stepbuffer;
public:
	adaptive() {
		std::cout << "Created adaptive step size" << std::endl;
	}

	~adaptive() {
		fstream outputfile;
		outputfile.open(filename.c_str(), fstream::out);
		for (int i=0;i<stepbuffer.size()/nparam;i++) {
			for (int j=0;j<nparam;j++) {
				outputfile << stepbuffer[i*nparam+j] << " ";
			}
			outputfile << endl;
		}
		outputfile.close();
	}

	void setup(options *o) {
		target = 0.6;
		lookback = 100;
		nparam=(int)o->getdoubleval("nparams");
		model = new uint32_t[nparam];
		prevmodel = new uint32_t[nparam];
		record = new bool*[nparam];
		for (uint16_t i=0;i<nparam;i++) {
			record[i] = new bool[lookback];
		}
		filename = o->getstringval("path")+"/stepsize.txt";
		tick = 0;
	}

	double invoke(chain *c, options *o) {
		c->rawlast(model);
		if (c->size()<2) return 0.0;
		c->getRawModel(c->size()-2, prevmodel);
		for (uint16_t i=0;i<nparam;i++) {
			if (model[i]==prevmodel[i]) record[i][tick] = false; else record[i][tick] = true;
		}

		tick++;
		if (tick==lookback) {
			tick=0;
			for(uint16_t i=0;i<nparam;i++) {
				double rate=0.0;
				for(uint16_t j=0;j<lookback;j++) {
					if (record[i][j]) rate+=1.0;
				}
				rate/=(double)lookback;
				c->setStep(i, c->getStep(i)*((target-rate)/target));
				stepbuffer.push_back(c->getStep(i));
			}
		}

		return 0;
	}
};

REGISTERAGENT(adaptive)
