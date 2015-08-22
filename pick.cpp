/*
	Gaussian number generator
	Author: Ewan Ferguson
	Created: 05/08/14
*/
#include <iostream>
#include <chrono>
#include <random>
#include <climits>
#include <cmath>

#include "include/pick.hpp"

using namespace std;

constexpr double oneOverLmax=1/ULONG_MAX;
double logOneOverLmax=log(1/ULONG_MAX);//-22.1807097777;

generator::generator(unsigned x){
  initialise(x);
}

double generator::getFlat() {
	return (double)mersenneEngine()*oneOverLmax;
}

double generator::getNorm(){
	double theta,r;
	numFlag=1-numFlag;
	if ( numFlag == 0) return ranStore;

	theta=2.0*M_PI*(mersenneEngine()*oneOverLmax);
 	r = (sqrt(-2*(log(mersenneEngine())+logOneOverLmax)));
  ranStore = sin(theta)*r;

	return cos(theta)*r;
}

void generator::getNormBlock(double *output, int n){
	double theta, r;

  //Stop the loop overrunning if an odd number of random numbers is requested
  if (n%2==1) {
		n--;
		output[n] = getNorm();
	}

	for(int i=0;i<n;i+=2) {
		theta=2.0*M_PI*(mersenneEngine()*oneOverLmax);
		r = (sqrt(-2*(log(mersenneEngine())+logOneOverLmax)));
		output[i] = cos(theta)*r;
		output[i+1] = sin(theta)*r;
	}
}

generator::generator(){

}

void generator::initialise(unsigned x){
	unsigned seed = chrono::system_clock::now().time_since_epoch().count()*x;
	unsigned burn;

	mersenneEngine.seed(seed);
	for (int i=0;i<100000;i++){
		burn = mersenneEngine();
	}

	numFlag = 0;
}
