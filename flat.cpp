/*
	flat
	Author: Ewan Ferguson
	Created: 01/09/14
*/
#include <iostream>

#include "include/agent.hpp"

using namespace std;

class flat : public agent {
public:
	flat () {
		std::cout << "flat created " << std::endl;
	}
	int status() const { return 4; }
	double invoke(chain *c, options *o);
};

double flat::invoke(chain *c, options *o) {
	return 1.0;
}

REGISTERAGENT(flat)
