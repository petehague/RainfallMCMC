/*
    Likelihood Template
    Author: Peter Hague
    Created: 22/07/14
*/
#include <iostream>

#include "include/agent.hpp"

using namespace std;

class likelihood : public agent {
public:
    likelihood () {
        std::cout << "Created object" << std::endl; //Report the name of the module
    }

    void setup(options *o) {
        //Run at the beginning of the chain
    }

    double eval(double *model) {
        //Return the likelihood of "model"
        return 0;
    }
};

REGISTERAGENT(likelihood)
