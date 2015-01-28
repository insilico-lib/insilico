
insilico - Neuronal Simulation Library
======================================

Official homepage: http://www.iiserpune.ac.in/~collins/insilico/

Software Pre-requisites
-----------------------

insilico is written in C++ and uses C++11 standard of the langauge. Also, integration-of-ODE part of the library is handled using Boost.

* Boost 1.56.0+ - for ODE integrator Boost.odeint
* g++ 4.8.1+ - for compilation with C++11 support
* make 3.8+ - building

(Optionally)
* Python 2.7+ - if you want to use scripts for generating input files
* Gnuplot - if you want to use scripts for plotting simulation results

The installation of above softwares for all platforms is expected to be in standard directories. Please do any essential manual changes are required to the environment for using this library.

Addition to these, programmers who wish to contribute may need following packages as well.

* PHP 5+ - web documentation and site
* Python 2.7+ - automated scripts
* Gnuplot 4.5+ - plotting

Build
=====

Clone the source and run the following commands on terminal.

   make SOURCE="path/to/source/file/with/main_function.cpp"

Input and Output files in detail
================================

Makefile generates the executable for the program which can accept three parameters as given below.

* 1st Parameter is Output file
------------------------------

  Simulation results will be written to this file as comma separated values in following format for each line of output.

  -----------------------------------------------------------------------
  | time-step , variable-1 , variable-2 , variable-3 , ... , variable-n |
  -----------------------------------------------------------------------

  - time-step:
    current time-step
  
  - variable-1 to variable-n:
    all the varible values that are provided to insilico::configuration::observer

* 2nd Parameter is Neuron file
------------------------------

  Neurons taking part in simulation. Every line represents a Neuron.

* 3rd Parameter is Synapse file
-------------------------------

  Synapse taking part in simulation. This input file is optional argument if there is no synapse in the simulation. Every line represents a synapse.

  Both the above parameter files has their specification as comma separated key:value pair(s) in the following format on each line.

  -------------------------------------------------------------------------
  | dxdt:<ODE-vars> , <var1>:<init> , <var2>:<init> , ... , <varN>:<init> |
  -------------------------------------------------------------------------

  - dxdt:<ODE-vars>:
    denotes the count of ODE variables, all the variables for ODEs in the simulation code should satisfy their count and the value replacing <ODE-vars> and those many variables should follow immediately after ('dxdt' is a library keyword, <ODE-vars> should be replaced by positive integer)
  
  - <var1>:<init> to <varN>:<init>:
    variables to be used in simulation with initial values (<var1> to <varN> should be replaced by desired variable name and <init> should be replaced by initial values of the respective variables), in the case of Synapse file variabled 'pre' and 'post' are mandatory with pre-synaptic and post-synaptic neuron indices as their values respectively

Execute
=======

Run the following command on terminal to execute the code.

* Linux / Mac OS

  ./insilico.out <outputfile>.dat <neuron_file>.conf [<synapse_file>.conf]

* Windows

  .\insilico.out <outputfile>.dat <neuron_file>.conf [<synapse_file>.conf]

Platforms
=========

We wish to make this library platform independent. The known minimum versions for a given platforms or compatible platforms:

Linux:   Ubuntu 11.04 +
Mac:     OS X +
Windows: XP +

Library is developed with majorly on Linux platform.

Contributing
============

Please refer to CONTRIBUTION file.

Release Notes
=============

Please refer to RELEASES file.

License
=======

This simulator library is licensed under GNU GPLv3 which is described in file LICENSE found in home directory of this project.
