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

enum errorType {
    err_noerror,
    err_nofile,
    err_nospawn,
    err_noclean
};

const char *error[] = {"", "File not found", "Spawner not found", "Cleaner not found"};

int errMessage(errorType e) {
    cout << error[e] << endl;
    return 1;
}

errorType pushAgent(string agentName, vector <void *> *fStack, vector<agent *> *aStack, vector<clean_agent *> *cStack, chain *c, options *o) {
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

  return err_noerror;
}

int main(int argc, char **argv) {
    vector<clean_agent *> cleanStack;
    vector<agent *> agentStack;
    vector<void *> fileStack;
    options o;
    chain c;
    double *model;
    double *newmodel;
    int index = 0;
    int is_parallel;
    double oldlikelihood, newlikelihood, ratio;
    generator ransource;
    fstream output;
    auto startpoint = chrono::system_clock::now();
    chrono::system_clock::time_point midpoint;
    double elapsed;
    errorType result;


    o.parseCL(argc, argv);

    if (!o.vetoCheck()) {
        cout << "Could not process options" << endl;
        return 1;
    }

    o.report();

    system(("mkdir "+o.getstringval("path")).c_str());

    model = new double[(uint16_t)o.getdoubleval("nparams")];
    newmodel = new double[(uint16_t)o.getdoubleval("nparams")];

    output.open(o.getstringval("path")+"/"+o.getstringval("outputfile"), fstream::out);
    if (!output.is_open()) {
        cout << "Can't open output file" << endl;
        return 1;
    }

    c.init(&o);
    ransource.initialise(startpoint.time_since_epoch().count());

    //Load in likelihood function
    result = pushAgent(o.getstringval("Likelihood"), &fileStack, &agentStack, &cleanStack, &c, &o);
    if (result!=err_noerror) return errMessage(result);
    (agentStack.back())->setup(&o);

    //Load in adaptive step size agent
    result = pushAgent(o.getstringval("adaptstep"), &fileStack, &agentStack, &cleanStack, &c, &o);
    if (result!=err_noerror) return errMessage(result);
    (agentStack.back())->setup(&o);

    //Load in all agents
    if (o.getrank("Agent")!=opt_emptyarray) {
        for (int i=0;i<o.keycount("Agent");i++) {
            result = pushAgent(o.getstringval("Agent", i), &fileStack, &agentStack, &cleanStack, &c, &o);
            if (result!=err_noerror) return errMessage(result);
            (agentStack.back())->setup(&o);
        }
    }

    cout << "Running on " << num_threads() << " threads" << endl;

    if (num_threads()==1) is_parallel=0; else is_parallel=1;

    oldlikelihood = 0;
    c.last(model);
    #pragma omp parallel reduction(+:oldlikelihood)
    {
        if (thread_num()>=is_parallel) oldlikelihood += agentStack[0]->eval(model);
    }
    output << " " << oldlikelihood;

    midpoint = chrono::system_clock::now();

    for(int i=0;i<o.getdoubleval("MaxModels");i++) {
        if (i%1000 == 0) cout << i << endl;
        agentStack[1]->invoke(&c, &o);     //Update the step size if necessary.
        c.step();
        c.current(newmodel);
        newlikelihood = 0;
        #pragma omp parallel reduction(+:newlikelihood)
        {
            if (thread_num()==0) {
                for(int agent_i=2;agent_i<agentStack.size(); agent_i++) {
                    agentStack[agent_i]->invoke(&c, &o);
                }

                c.last(model);

                for(int j=0;j<o.getdoubleval("nparams");j++) {
                    output << " " << model[j];
                }
                output << endl;
            }

            if (thread_num()>=is_parallel) {
                newlikelihood += agentStack[0]->eval(newmodel);
            }
        }

        c.push();

        if (newlikelihood<oldlikelihood) {
            output << " " << newlikelihood;
            oldlikelihood = newlikelihood;
        } else {
            if (ransource.getFlat()<exp(oldlikelihood-newlikelihood)) {
                output << " " << newlikelihood;
                oldlikelihood = newlikelihood;
            } else {
                c.pop();
                c.repeat();
                output << " " << oldlikelihood;
            }
        }
    }
    cout << endl;

    c.last(model);
    for(int j=0;j<o.getdoubleval("nparams");j++) {
        output << " " << model[j];
    }

    cout << "Unloading " << cleanStack.size() << " agents..." << endl;

    //Unload all agents
    for(int agent_i=0; agent_i<cleanStack.size(); agent_i++) {
        cleanStack[agent_i](agentStack[agent_i]);
        dlclose(fileStack[agent_i]);
    }

    output.close();

    elapsed = (double)(std::chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now()-startpoint).count() )/1000.0;

    cout << "Time elapsed: " << elapsed << " seconds." << endl;

    elapsed = (double)(std::chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now()-midpoint).count() )/1000.0;

    cout << "Chain time: " << elapsed << " seconds." << endl;
    if (elapsed/o.getdoubleval("MaxModels")>1.0) {
        cout << "Time per model: " << elapsed/o.getdoubleval("MaxModels") << " seconds." << endl;
    } else {
        cout << "Time per model: " << (elapsed/o.getdoubleval("MaxModels"))*1000.0 << " milliseconds." << endl;
    }

}
