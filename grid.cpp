/*
	Grid agent
	Author: Peter Hague
	Created: 26/08/14
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cinttypes>

#include "include/grid.hpp"
#include "include/agent.hpp"

using namespace std;

grid::grid(int bins, double xmin, double xmax, double ymin, double ymax) {
  count = new uint64_t[bins*bins];
  x0 = xmin;
  x1 = xmax;
  dx = (xmax-xmin)/((double)bins);
  y0 = ymin;
  y1 = ymax;
  dy = (ymax-ymin)/((double)bins);
  size=bins;

  for(int i=0;i<bins;i++)
    count[i]=0;
}

grid::grid() {
}

void grid::setup(int bins, double xmin, double xmax, double ymin, double ymax) {
  count = new uint64_t[bins*bins];
  x0 = xmin;
  x1 = xmax;
  dx = (xmax-xmin)/((double)bins);
  y0 = ymin;
  y1 = ymax;
  dy = (ymax-ymin)/((double)bins);
  size=bins;

  for(int i=0;i<bins;i++)
    count[i]=0;
}

void grid::incbin(double xdata, double ydata) {
  if (xdata>=x0 && xdata<=x1 && ydata>=y0 && ydata<=y1) {
    xdata -= x0;
    xdata /= dx;
    ydata -= y0;
    ydata /= dy;
    count[(int)floor(xdata)+size*(int)floor(ydata)]++;
  }
}

void grid::exbin(double xdata, double ydata) {
  if (xdata>=x0 && xdata<=x1 && ydata>=y0 && ydata<=y1) {
    xdata -= x0;
    xdata /= dx;
    ydata -= y0;
    ydata /= dy;
    count[(int)floor(xdata)+size*(int)floor(ydata)]--;
  }
}

void grid::write(string filename) {
  int x,y;
  fstream outputfile;
  
  outputfile.open(filename.c_str(), fstream::out);

  for (x=0;x<size;x++) {
    for(y=0;y<size;y++)
      outputfile << " " << count[x+y*size];
    outputfile << endl;
  }

  outputfile.close();
}

double* grid::peak() {
  int x, y, maxcount, xi, yi;
  double *result = new double[2];

  maxcount=0;
  xi = 0;
  yi = 0; 

  for (x=0;x<size;x++)
    for(y=0;y<size;y++)
      if (count[x+y*size]>maxcount) {
        xi=x;
        yi=y;
        maxcount=count[x+y*size];
      }

  result[0] = x0+(dx/2)+dx*(double)xi;
  result[1] = y0+(dy/2)+dy*(double)yi;

  cout << result[0] << ", " << result[1] << "\n";
  
  return result;
}

//------------------------------------------------

class plotgrid : public agent {	
	grid **gr;
	uint16_t nparam;
	double *model;
	string **filename;
public:
	plotgrid() {
		std::cout << "Created Grids" << std::endl;
	}
	
	~plotgrid() {
		for (uint16_t y=0;y<nparam;y++) {
			for (uint16_t x=0;x<nparam;x++) {
				if (x!=y) {
					cout << "Writing " << filename[x][y] << endl;
					gr[x][y].write(filename[x][y]);
				}
			}
		}
	}
	
	void setup(options *o) {
		nparam = o->getdoubleval("nparams");
		
		gr = new grid*[nparam];
		filename = new string*[nparam];
		for (uint16_t i=0;i<nparam;i++) {
			gr[i] = new grid[nparam];
			filename[i] = new string[nparam];
		}
		model=new double [nparam];
	
		for (uint16_t y=0;y<nparam;y++) {
			for (uint16_t x=0;x<nparam;x++) {
				gr[x][y].setup(10, o->getdoubleval("lowerlimit", x), o->getdoubleval("upperlimit", x),
				  o->getdoubleval("lowerlimit", y), o->getdoubleval("upperlimit", y));
				filename[x][y] = o->getstringval("path")+"/"+o->getstringval("paramname", x)+o->getstringval("paramname", y)+"-grid.txt";
			}
		}
	}

	double invoke(chain *c, options *o) {
		c->last(model);
		for (uint16_t y=0;y<nparam;y++) {
			for (uint16_t x=0;x<nparam;x++) {
				gr[x][y].incbin(model[x], model[y]);
			}
		}
		return 0;
	}
};

REGISTERAGENT(plotgrid)
