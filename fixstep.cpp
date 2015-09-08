/*
    Fixed step size
    Authors: Flor Allaert
    Created: 26/08/15
*/
#include <iostream>

#include "include/agent.hpp"

using namespace std;

class fixstep : public agent {
public:
    fixstep() {
        std::cout << "Width of the proposal distribution (\"step size\") will be kept fixed" << std::endl;
    }

    void setup(options *o) { }

    double invoke(chain *c, options *o) {
        return 0.0;
    }
};

REGISTERAGENT(fixstep)
