/*
    bimodal2dgauss likelihood function
    Author: Flor Allaert
    Created: 28/08/15
*/
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>

#include "include/agent.hpp"
#include "include/pick.hpp"

using namespace std;

class bimodal2dgauss : public agent {
    double meanx1, meany1, meanx2, meany2;
    double sigx1, sigy1, sigx2, sigy2;
    double max1, max2;
public:
    bimodal2dgauss() {
        meanx1 = 10;
        meany1 = 8;
        meanx2 = 35;
        meany2 = 30;
        sigx1 = 3;
        sigy1 = 2;
        sigx2 = 4;
        sigy2 = 5;
        max1 = 10;
        max2 = 15;
    }

    ~bimodal2dgauss() {
        std::cout << "Created 2D bimodal Gaussian: Mean1=" << meanx1 << ";" << meany1 << " Mean2=" << meanx2
                  << ";" << meany2 << " Width1=" << sigx1 << ";" << sigy1 << " Width2=" << sigx2 << ";"
                  << sigy2 << " Height1=" << max1 << " Height2=" << max2 << std::endl;
    }

    void setup(options *o) {
    }

    double eval(double *model) {
        double x, y, result=0;
        double g1=max1, g2=max2;
        x = model[0];
        y = model[1];

        g1 *= exp(-(x-meanx1)*(x-meanx1)/(2*sigx1*sigx1));
        g1 *= exp(-(y-meany1)*(y-meany1)/(2*sigy1*sigy1));
        g2 *= exp(-(x-meanx2)*(x-meanx2)/(2*sigx2*sigx2));
        g2 *= exp(-(y-meany2)*(y-meany2)/(2*sigy2*sigy2));
        result = g1 + g2;

        return result;
    }

    double evalratio(double *model, double *newmodel) {
        double G1, G2;
        G1 = bimodal2dgauss::eval(model);
        G2 = bimodal2dgauss::eval(newmodel);
        return G2/G1;
    }
};

REGISTERAGENT(bimodal2dgauss)
