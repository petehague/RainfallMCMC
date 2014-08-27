#include <fstream>
#include <string>
#include <cmath>

#include <cinttypes>

using namespace std;

class histogram {
private: 
<<<<<<< HEAD
=======
public:
>>>>>>> 5281327999039e469a40fcfb672f32b609601c6a
  double *count;
  int size;
  double x0;
  double x1;
  double dx;
<<<<<<< HEAD
public:
=======
//public:
>>>>>>> 5281327999039e469a40fcfb672f32b609601c6a
  histogram(int bins, double min, double max);
  histogram();
  void setup(int bins, double min, double max);
  void incbin(double data);
<<<<<<< HEAD
  void exbin(double data);
=======
>>>>>>> 5281327999039e469a40fcfb672f32b609601c6a
  void normalise(string filename);
  void write(string filename);
  void writeprior(string filename);
  double peak();
};
