/*
	Filler likelihood estimator
	Author: Peter Hague
	Created: 22/07/14
*/
#include <iostream>
#include <string>

#include "include/agent.hpp"

using namespace std;

class likelihood : public agent {
public:
	likelihood (chain *c, options *o) : agent(c, o) {
		std::cout << "Created object" << std::endl;
	}
	int status() const { return 4; }
	void invoke(chain *c, options *o);
};

void likelihood::invoke(chain *c, options *o) {
	cout << c->size() << endl;
}

REGISTERAGENT(likelihood)
