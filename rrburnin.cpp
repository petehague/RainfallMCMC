/*
    Adaptive step size agent for a Metropolis-Hastings algorithm (NOT suitable for e.g. Gibbs sampling)
    Step sizes are only adapted during the burn-in phase, according to Roberts & Rosenthal (2001)
    Authors: Peter Hague and Flor Allaert
    Created: 25/08/15
*/
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <cinttypes>

#include "include/agent.hpp"

using namespace std;

// If one free parameter: use target acceptance rate of 0.44. Else: use target rate of 0.234
// Only adapt the step sizes during the burn-in phase.
// Only adapt the step sizes every so many iterations
//------------------------------------------------

class rrburnin : public agent {
    uint16_t nparam, nfree;
    uint32_t burnin;
    uint32_t *model;
    uint32_t *prevmodel;
    double target;
    uint16_t lookback, tick;
    string filename;
    bool *accepted;
    vector <double>stepbuffer;
public:
    rrburnin() {
        std::cout << "Will adapt the step size during the burn-in phase" << std::endl;
    }

    ~rrburnin() {
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
        lookback = (int)o->getdoubleval("lookback");
        nparam=(int)o->getdoubleval("nparams");
        nfree = (int)o->getdoubleval("nfree");
        burnin = (int)o->getdoubleval("burnin");
        if (nfree==1) target = 0.44; else target = 0.234;
        model = new uint32_t[nparam];
        prevmodel = new uint32_t[nparam];
        accepted = new bool[lookback];
        accepted[0] = true;
        tick = 1;
        filename = o->getstringval("path")+"/stepsize.txt";
    }

    double InverseCompCDFNormal(double t) {
        /*
          Rational approximation for inverse of Complementary CDF of standard normal distribution.
          Based on Abramowitz and Stegun formula 26.2.23.
          Only works for 0 < p <= 0.5. The absolute value of the error should be less than 4.5 e-4.
        */
        double c[] = {2.515517, 0.802853, 0.010328};
        double d[] = {1.432788, 0.189269, 0.001308};
        return t - ((c[2]*t + c[1])*t + c[0]) /
                   (((d[2]*t + d[1])*t + d[0])*t + 1.0);
    }

    double InverseCDFNormal(double p) {
        /*
          Compute inverse CDF ('F') of standard normal distribution from the inverse CCDF ('G') of that distribution
          Based on http://www.johndcook.com/blog/normal_cdf_inverse/
        */
        if (p < 0.5) {
            // F^-1(p) = - G^-1(p)
            return -InverseCompCDFNormal( sqrt(-2.0*log(p)) );
        }
        else {
            // F^-1(p) = G^-1(1-p)
            return InverseCompCDFNormal( sqrt(-2.0*log(1-p)) );
        }
    }

    double invoke(chain *c, options *o) {
        if (c->size()>=2 & c->size()<=burnin) {
            c->rawlast(model);
            c->getRawModel(c->size()-2, prevmodel);
            if (model[0]==prevmodel[0]) accepted[tick] = false; else accepted[tick] = true;
            tick++;
            if (tick==lookback) {
                tick = 0;
                double rate = 0.0;
                for(uint16_t i=0;i<lookback;i++) {
                    if (accepted[i]) rate+=1.0;
                }
                if (rate==0.0) { rate+=1.0; }       // avoid dividing by infinity (InverseCDFNormal(0)=-inf)
                if (rate==lookback) { rate-=1.0; }  // avoid dividing by zero (InverseCDFNormal(0.5)=0)
                rate/=(double)lookback;
                double scale = InverseCDFNormal(target*0.5)/InverseCDFNormal(rate*0.5);
                for(int i=0;i<nparam;i++) {
                    c->setStep(i, c->getStep(i)*scale);
                }
            }
        }
        for (uint16_t j=0;j<nparam;j++) {
            stepbuffer.push_back(c->getStep(j));
        }

        return 0.0;
    }
};

REGISTERAGENT(rrburnin)
