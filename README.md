Rainfall
========

A modular MCMC driver designed for computationally intensive likelihood functions.

Getting Started


Purpose
-------

My astrophysics research uses Bayesian methods, and I require a powerful and flexible MCMC driver. 

Features
--------

- Modular design: Likelihood calculation and other functions are dynamically linked
- Suite of output functions: Produces histograms, contour plots, and subsamples of chains
- Portability: Compiles and runs on Mac OS X and all tested versions of Linux out of the box

Getting Started
---------------

1. Clone the repository
2. In the `RainfallMCMC` folder, type `make build=serial` (the parallel version is still being tested)
3. Type `./rainfall test.ini`
4. The results of the MCMC run will be in the newly created subfolder `output`
5. Use likelihood.cpp as a template to create your own likelihood functions, and test.ini as a parameter file template
