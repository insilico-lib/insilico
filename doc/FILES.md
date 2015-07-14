insilico - Neuronal Simulation Library
======================================

Makefile generates the executable for the program which can accept three parameters as given below.

Output files
============

  Simulation results will be written to this file as comma separated values (CSV) in following format for each line of output.
```
  -----------------------------------------------------------------------
  | time-step , variable-1 , variable-2 , variable-3 , ... , variable-n |
  -----------------------------------------------------------------------
```
  - time-step:
    current time-step

  - variable-1 to variable-n:
    all the varible values that are provided to insilico::configuration::observer

Input files
===========

Neuron and Synapse file
-----------------------

  Neuron(s) and/or Synapse(s) taking part in simulation. Line ending with a semi-colon represents a single Neuron or Synapse. Synapse input file is optional argument if there is no synapse in the simulation.

  Both the above parameter files has their specification as comma separated key:value pair(s) in the following format on each line.

  --------------------------------------------------------------------------
  | dxdt:<ODE-vars> , <var1>:<init> , <var2>:<init> , ... , <varN>:<init>; |
  --------------------------------------------------------------------------

  - dxdt:<ODE-vars>:
    denotes the count of ODE variables, all the variables for ODEs in the simulation code should satisfy their count and the value replacing <ODE-vars> and those many variables should follow immediately after ('dxdt' is a library keyword, <ODE-vars> should be replaced by positive integer)
  
  - <var1>:<init> to <varN>:<init>:
    variables to be used in simulation with initial values (<var1> to <varN> should be replaced by desired variable name and <init> should be replaced by initial values of the respective variables), in the case of Synapse file variabled 'pre' and 'post' are mandatory with pre-synaptic and post-synaptic neuron indices as their values respectively

External Current file
---------------------

  External current to be injected into Neuron(s) or their comparment. This input file is optional, if extrenal current is not involved or implemented into the simulation input or code.

  Use the following insilico Simulation File format for current (isfc).
```
  ----------------------------------------------------------
  | time   , neuron-id-1 , neuron-id-2 , ... , neuron-id-n |
  | tstart , cur-val-1   , cur-val-2   , ... , cur-val-n   |
  | +tstep , ...         , ...         , ... , ...         |
  | ...                                                    |
  | tend   , cur-val-1m  , cur-val-2m  , ... , cur-val-nm  |
  ----------------------------------------------------------
```
  - time:
    String to signify the header of the input file

  - neuron-id-1 to neuron-id-n:
    List of Neuron IDs (simulator specific ids, which are read from Neuron file and starts from 0). A given ID can be repeated to denote the different compartment of the same neuron. And this should be handled by programmer.

  - tstart, +tstep and tend:
    Increasing time of simulation run. Simulation needs current starting from time 'tstart', and end at 'tend'. '+tsteps' can be steps in between.

  - (cur-val-1 to cur-val-n) to (cur-val-1m to cur-val-nm):
    External current for a specific neuron in its column for all neurons available in header and for all time steps from 'tstart' to 'tend'.

Dynamic Parameters file
-----------------------

  Dynamic parameters file is a special file with named parameters like value-params in neuron file. This input file is optional.

  Use the following insilico Simulation File format for dynamic params (isfdp).
```
  -----------------------------------------------------------
  | time   , param-name-1, param-name-2, ... , param-name-n |
  | tstart , param-val-1 , param-val-2 , ... , param-val-n  |
  | +tstep , ...         , ...         , ... , ...          |
  | ...                                                     |
  | tend   , param-val-1m, param-val-2m, ... , param-val-nm |
  -----------------------------------------------------------
```
  - time:
    String to signify the header of the input file

  - param-name-1 to param-name-n:
    List of parameters name. Parameter should not be repeated.

  - tstart, +tstep and tend:
    Increasing time of simulation run. Simulation parameters can start from time 'tstart', and end at 'tend'. '+tsteps' can be steps in between.

  - (param-val-1 to param-val-n) to (param-val-1m to param-val-nm):
    Dynamic param values for all time steps from 'tstart' to 'tend'.
