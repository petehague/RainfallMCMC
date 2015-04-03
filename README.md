Rainfall
========

A modular MCMC code designed for computationally intensive likelihood functions. It is designed to run on shared memory machines, with the likelihood function running in parallel whilst a single thread takes care of administrative functions and postprocessing. All functions, including likelihood calculation, are dynamically linked.

Purpose
-------

My astrophysics research uses Bayesian methods to examine galaxies, and I require a powerful and flexible MCMC code to do this. The work that has led to the development of RainfallMCMC is covered in the following papers:

[Dark matter in disc galaxies - I. A Markov Chain Monte Carlo method and application to DDO 154](http://adsabs.harvard.edu/abs/2013MNRAS.433.2314H)

[Dark matter in disc galaxies - II. Density profiles as constraints on feedback scenarios](http://adsabs.harvard.edu/abs/2014MNRAS.443.3712H)


Features
--------

- Modular design: Likelihood calculation and other functions are dynamically linked
- Suite of output functions: Produces histograms, contour plots, and subsamples of chains
- Portability: Compiles and runs on Mac OS X and all tested versions of Linux out of the box

Status
------

Currently, only the serial version of the code is being tested. 

The only implementation of MCMC available is the Metropolis-Hastings algorithm.

At the moment the focus is on constructing a set of utility modules, such as ones providing adaptive step size.

Getting Started
---------------

1. Clone the repository
2. In the `RainfallMCMC` folder, type `make` 
3. Type `./rainfall test.ini`
4. The results of the MCMC run will be in the newly created subfolder `output`
5. Use likelihood.cpp as a template to create your own likelihood functions, and test.ini as a parameter file template
