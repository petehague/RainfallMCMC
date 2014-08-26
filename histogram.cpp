/*
	Histogram agent
	Author: Peter Hague
	Created: 26/08/14
*/
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>

#include <cinttypes>

#include "include/histogram.hpp"
#include "include/agent.hpp"

using namespace std;

histogram::histogram(int bins, double min, double max) {
  count = new double[bins];
  x0 = min;
  x1 = max;
  dx = (max-min)/((double)bins);
  size=bins;

  for(int i=0;i<bins;i++)
    count[i]=0.0;
}

histogram::histogram() {
}

void histogram::setup(int bins, double min, double max) {  
  count = new double[bins];
  x0 = min;
  x1 = max;
  dx = (max-min)/((double)bins);
  size=bins;

  for(int i=0;i<bins;i++)
    count[i]=0.0;
}

void histogram::incbin(double data) {
  int i;
  if (data>=x0 && data<=x1) {
    data -= x0;
    data /= dx;
    i = (int)floor(data);
    count[i]++;
  }
}

void histogram::exbin(double data) {
  int i;
  if (data>=x0 && data<=x1) {
    data -= x0;
    data /= dx;
    i = (int)floor(data);
    count[i]--;
  }
}

void histogram::normalise(string filename) {
  double normval;
  fstream inputfile;

  inputfile.open(filename.c_str(), fstream::in);

  for (int i=0;i<size;i++) {
    inputfile >> normval;
    count[i] /= normval;
  }  

  inputfile.close();
}

void histogram::write(string filename) {
  int i;
  fstream outputfile;
  
  outputfile.open(filename.c_str(), fstream::out);

  for (i=0;i<size;i++)
    outputfile << x0+(dx/2)+dx*(double)i << " " << count[i] << endl;

  outputfile.close();
}

void histogram::writeprior(string filename) {
  int i;
  fstream outputfile;
  
  outputfile.open(filename.c_str(), fstream::out);

  for (i=0;i<size;i++)
    outputfile << count[i] << endl;

  outputfile.close();
}

double histogram::peak() {
  int i, maxi, maxcount;

  maxcount=0;
  maxi=0;
  for (i=0;i<size;i++)
    if (count[i]>maxcount) {
      maxcount=count[i];
      maxi=i;
    }

  return x0+(dx/2)+dx*(double)maxi;
}

class plothist : public agent {
public:
	plothist(chain *c, options *o); 
	int status() const { return 4; }
	void invoke(chain *c, options *o);
};

plothist::plothist(chain *c, options *o) : agent(c,o) {
	//Intantiate histogram for each parameter in the chain
}

void plothist::invoke(chain *c, options *o) {
}

REGISTERAGENT(plothist)