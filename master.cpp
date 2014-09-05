/*
	Master control program
	Author: Peter Hague
	Created: 20/07/14
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <dlfcn.h>

#include "include/agent.hpp"
#include "include/chain.hpp"
#include "include/options.hpp"
#include "include/pick.hpp"

using namespace std;

const int err_nofile = 1, err_nospawn = 2, err_noclean = 3;
const char *error[] = {"", "File not found", "Spawner not found", "Cleaner not found"};

int errMessage(int e) {
	cout << error[e] << endl;
	return 1;
}

int pushAgent(string agentName, vector <void *> *fStack, vector<agent *> *aStack, vector<clean_agent *> *cStack, chain *c, options *o) {
    void *agentFile;
	cout << "Loading " << agentName << endl;
	agentFile = dlopen(agentName.c_str(), RTLD_LAZY);
	if (!agentFile) return err_nofile;
		
	dlerror();
	spawn_agent* agentSpawner = (spawn_agent*) dlsym(agentFile, "spawn");
	if (dlerror()) return err_nospawn;
    
	cStack->push_back((clean_agent*) dlsym(agentFile, "clean"));
	if (dlerror()) return err_noclean;
	
    aStack->push_back(agentSpawner());
	fStack->push_back(agentFile);
    
    return 0;
}

int main(int argc, char **argv) {
	vector<clean_agent *> cleanStack;
	vector<agent *> agentStack;
	vector<void *> fileStack;
	options o;
	chain c;
	double *model;
	int index = 0;
	double oldlikelihood, newlikelihood;
	generator ransource(42);
	fstream output;
	
	o.parseCL(argc, argv);
	
	if (!o.vetoCheck()) {
		cout << "Could not process options" << endl;
		return 1;
	}
	
	o.report();

	model = new double[(uint16_t)o.getdoubleval("nparams")];
	output.open(o.getstringval("outputfile"), fstream::out);
	if (!output.is_open()) {
		cout << "Can't open output file" << endl;
		return 1;
	}

	c.init(&o);
	
	//Load in likelihood function
	pushAgent(o.getstringval("Likelihood"), &fileStack, &agentStack, &cleanStack, &c, &o);

	//Load in all agents
	for (int i=0;i<o.keycount("Agent");i++) {
    	int result = pushAgent(o.getstringval("Agent", i), &fileStack, &agentStack, &cleanStack, &c, &o);
    	if (result!=0) return errMessage(result);
    	(agentStack.back())->setup(&o);
	}
	
	oldlikelihood = agentStack[0]->invoke(&c, &o);
	for(int i=0;i<o.getdoubleval("MaxModels");i++) {
		c.step();
		newlikelihood = agentStack[0]->invoke(&c, &o);
		if (newlikelihood>oldlikelihood) {
			c.push();
			output << " " << newlikelihood;
			oldlikelihood = newlikelihood;
		} else {
			if (ransource.flatnum()>(newlikelihood/oldlikelihood)) {
				c.push();
				output << " " << newlikelihood;
				oldlikelihood = newlikelihood;
			} else {
				c.repeat();
				output << " " << oldlikelihood;
			}
		}
		c.last(model);
		for(int j=0;j<o.getdoubleval("nparams");j++)
			output << " " << model[j];
		output << endl;
	}
  
	cout << "Unloading " << cleanStack.size() << " agents..." << endl;

	//Unload all agents
	for(int agent_i=0; agent_i<cleanStack.size(); agent_i++) {
		cleanStack[agent_i](agentStack[agent_i]);
		dlclose(fileStack[agent_i]);
	}
	
	output.close();
}