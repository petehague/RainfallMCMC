/*
	Master control program
	Author: Peter Hague
	Created: 20/07/14
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <dlfcn.h>

#include "include/ompswitch.hpp"
#include "include/agent.hpp"
#include "include/chain.hpp"
#include "include/options.hpp"
#include "include/pick.hpp"

using namespace std;

int main(int argc, char **argv) {
	options o;
	chain c;
	void *agentFile;
	double *sample;
	agent *likeAgent;

 	o.parseCL(argc, argv);
 	sample = new double[(int)o.getdoubleval("nparams")];

	agentFile = dlopen(o.getstringval("Likelihood").c_str(), RTLD_LAZY);
	if (!agentFile) {
		cout << "File not found" << endl;
		return 0;
	} 
		
	dlerror();
	spawn_agent* agentSpawner = (spawn_agent*) dlsym(agentFile, "spawn");
	if (dlerror()) {
		cout << "Spawner not found" << endl;
		return 0;
	} 
	likeAgent = agentSpawner();
    
 	c.init(&o);
    likeAgent->setup(&o);
    
    while(1) {
	    for(int i=0;i<o.getdoubleval("nparams");i++) {
	    	cout <<  o.getstringval("paramname", i) << " = ";
			cin >> sample[i];
   		}
    
    	cout << likeAgent->eval(sample) << endl;
    }
    
    return 0;
}