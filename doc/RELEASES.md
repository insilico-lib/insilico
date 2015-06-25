insilico: Release Notes for Stable releases
===========================================

(Please refer IDC (insilico Developer Channel, https://groups.google.com/forum/#!forum/insilico) mail for detailed description)

Version 0.25
------------
Release: May 6, 2015

New Features:
* Dynamic parameters

Enhancements:
* These functions are more efficient: injector, neuron_id_from_index, synapse_id_from_index, variable_name_from_index
* New example SingleNeuron_HH1952_Stochastic added to demonstrate stochastic integration possibility

Bug Fixes:
* neuron_id_from_index and synapse_id_from_index are now accurate
* Observer header is not skipped if parameters and variables are observed out-of-order

Version 0.24
------------
Release: Apr. 29, 2015

New Features:
* Observer can observe model parameters and runtime variables if available in input file
* Get variable name from its index (location) in global vector using Engine API as given below

Enhancements:
* Headers are part of include/insilico
* Observer supports custom delimiter
* Headers for observer can be disabled
* Value and Index functions does handles exceptions due to failures better than before
* Command-line interface allows space between options to help tab completion
* Example code demonstrate object-oriented insilico (SingleNeuron_HH1952 as of now)
* Use of const in non-variable contexts
* Warnings from GCC and clang are resolved by making code interoperable

Deprecation:
* insilico::engine::current_value() function are deprecated (read removed)

Bug Fixes:
* Comments support in input files improved (neuron and synapse input files only)
* Malfunction in Injector is resolved
* DefaultSynapse uses spike history
* Erroneous exceptions are resolved and will not occur again

Version 0.21
------------
Released: Mar. 18, 2015

New Feature: Engine's Generate functions
New Feature: Configuration's Observe functions
New: core.hpp includes configurtaion, engine and injector
New: Examples Updated
Several bug fixes and minor improvements

Version 0.20
------------
Released: Mar. 9, 2015

New Feature: Extrenal Current input file
New: User interface improved
New: Cross platform support
New: Updated local doc
New: Web moved to new website project - insilico-web
New: Examples Updated
Several bug fixes and minor improvements

Version 0.17
------------
Released: Feb. 11, 2015

New Feature: Observation of constants from Neurons
New: Improved compilation time
New: Explicit include directory
Bug Fix: Values transferred from connections (synapses) to nodes (neurons) are double precision floating points
Bug Fix: connectivity file (a.k.a. synapse file) is optional

Version 0.16
------------
Rel. Date: Jan. 21, 2015

New Feature: 'variables' vector is writable
New: Added Integrate and Fire neuron
New: Added Integrate and Fire neuron example
Improvement: Parser for input files

Version 0.15
------------
Rel. Date: Jan. 19, 2015

New Feature: Supporting comments ("") in input files
New Feature: Current values can be stored and retrieved using independent function
New Feature: Multi-line inputs for single neuron/synapse are supported
New Feature: Update to constant values in file is now permittable
New Feature: Double precision floating points for better precision in computations
New Feature: insilico has its own RNG - Random Number Generator
New: NAMING_CONVENTION document added
Improvement: Improved documentation - Contribution and README
Improvement: Web portal improved and part of library source
Improvement: Input file parsing is faster and cleaner
Improvement: Logic for delay in synapse and spike duration in neuron improved
Improvement: Example code

Version 0.10
------------
Rel. Date: Jan. 3, 2015

- Generalized code compilation strategy
- Enhancement to code execution strategy
- Improved and concise sample example
- Simulation specific concepts - Configuration and Engine
- Community support improvement

Version 0.02
------------
Rel. Date: Dec. 2, 2014

- Makefile improvements
- Minor performance improvements
- Few minor bug fixes
- Example in main.cpp is now improved 
