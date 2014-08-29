#ifndef DEFAULTS
#define DEFAULTS

#include <random>

using namespace std;

class generator{
  mt19937 g1;
  int lnum;
  double store;
public:
  void getblock (double *output, int n);
  generator(unsigned x);
  double getnum();
  generator();
  void initialize(unsigned x);

};

#endif
