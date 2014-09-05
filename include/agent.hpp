#ifndef AGENT
#define AGENT

#include "chain.hpp"
#include "options.hpp"

class agent {
public:
    agent() {}
    virtual ~agent() { }
    virtual void setup(options *o) { };
    virtual double invoke(chain *c, options *o) { return 0.0; }
};

// the types of the class factories
typedef agent* spawn_agent();
typedef void clean_agent(agent*);

// Registration macro - to be included at the end of all agent files
#define REGISTERAGENT(AGENTNAME) \
	extern "C" agent* spawn() { return new AGENTNAME; } \
	extern "C" void clean(agent* a) { delete a; }
#endif