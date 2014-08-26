#include <fstream>
#include <string>
#include <cmath>

#include <cinttypes>

using namespace std;

class histogram {
private: 
public:
  double *count;
  int size;
  double x0;
  double x1;
  double dx;
//public:
  histogram(int bins, double min, double max);
  histogram();
  void setup(int bins, double min, double max);
  void incbin(double data);
  void normalise(string filename);
  void write(string filename);
  void writeprior(string filename);
  double peak();
};
