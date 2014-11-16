/*
	Gaussian number generator
	Author: Ewan Ferguson
	Created: 05/08/14
*/
#include <iostream>
#include <chrono>
#include <random>

#include "include/pick.hpp"

using namespace std;

const double pii=6.283185, lmaxresip=1/4294967295.0, lnlmaxresip=-22.1807097777;
//const double lintmax=4294967295.0;

generator::generator(unsigned x){
 unsigned seed = chrono::system_clock::now().time_since_epoch().count()*x;
 unsigned burn;
 g1.seed(seed);
 lnum=0;
  for (int burnum=0; burnum < 100000; burnum++){
  burn =g1();
  }
}

double generator::flatnum() {
	return (double)g1()*lmaxresip;
}

double generator::getnum(){
 double use,piiv,rootv;
lnum=1-lnum;
if ( lnum == 0) return store;
 
 piiv=pii*(g1()*lnlmaxresip);
 rootv = (sqrt(-2*(log(g1())+lnlmaxresip)));
  use = cos(piiv)*rootv;
  store = sin(piiv)*rootv;

return use;
 
}

void generator::getblock(double *output, int n){
 double use,piiv,rootv;
for(int i=0; i < n; i+=2){
 piiv=pii*(g1()*lmaxresip);
 rootv = (sqrt(-2*(log(g1())+lnlmaxresip)));
  output[i] = cos(piiv)*rootv;
  output[i+1] = sin(piiv)*rootv;
}
}

generator::generator(){

}

void generator::initialise(unsigned x){
 unsigned seed = chrono::system_clock::now().time_since_epoch().count()*x;
 unsigned burn;
 g1.seed(seed);
 lnum=0;
  for (int burnum=0; burnum < 100000; burnum++){
  burn =g1();
  }
}

