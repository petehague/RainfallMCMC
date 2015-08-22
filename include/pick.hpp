#ifndef DEFAULTS
#define DEFAULTS

#include <random>

using namespace std;

class generator{
  mt19937 mersenneEngine;
  int numFlag;
  double ranStore;
public:
  void getNormBlock (double *output, int n);
  generator(unsigned x);
  double getFlat();
  double getNorm();
  generator();
  void initialise(unsigned x);
};

#endif
