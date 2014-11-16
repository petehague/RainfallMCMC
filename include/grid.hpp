#include <fstream>
#include <string>
#include <cmath>
#include <cinttypes>

using namespace std;

class grid {
private: 
  uint64_t *count;
  int size;
  double x0;
  double x1;
  double dx;
  double y0;
  double y1;
  double dy;
public:
  grid(int bins, double xmin, double xmax, double ymin, double ymax);
  grid();
  void setup(int bins, double xmin, double xmax, double ymin, double ymax);
  void incbin(double xdata, double ydata);
  void exbin(double xdata, double ydata);
  void write(string filename);
  double* peak();
};


