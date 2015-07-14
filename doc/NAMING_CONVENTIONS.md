
insilico Neuronal Simulator Library
===================================

(by Pranav Kulkarni, Collins Assisi Lab, IISER Pune <pranavcode@gmail.com>)

Naming Conventions
------------------

Naming Convention is a agreed upon mechanism of well understood and finite set of rules to be followed to "name" any part of software by the people directly involved. This helps in various forms as mentioned below, also it the convention is in place finding, communication and refering a specific part of the software becomes easier while development, testing, raising issues, etc.

Following rules for naming things may seem to be difficult at first instance, but, has proven to help projects in long run.

* Positives:
 - Given an object can be well understood contextually just by name
 - Less naming conflicts
 - Less bugs and errors
 - Brings in discipline

* Negatives:
 - Remembering and following conventions everytime anything new needs to be introduce, making this task overhead
 - Slows down the development (mere early feeling!)

These are the list of objects around the naming conventions followed throughout the library, arranged in alphabetical order.

Please read as:

* OBJECT / REPRESENTATION:
  NAMING CONVENTION
  EXAMPLE

* Constants in the code:
  All alphabets should be capital
  PI, MAGIC_NUMBER

* Constants in the input file:
  constant_name_<channel_type|neuron_type|compartment_type|synapse_type>
  iext_hh

* Current class:
  I_<channel>_<dependent>_<neuron>_<model>_<authors><year>
  I_K_HH_HH1952

* Current file:
  I_<channel>_<dependent>_<neuron>_<model>_<authors><year>.hpp
  I_K_HH_HH1952.hpp

* Executable file extension:
  'out'
  insilico.out, main.out, a.out

* Header file extension:
  'hpp'
  engine.hpp, configuration.hpp

* Helper class and extension:
  Small case alphanumeric words separated by '_' (underscore), with extension 'hpp'
  simulation_engine.hpp

* Library global namespace:
  'insilico'
  insilico::engine, insilico::configuration

* Macros:
  Capital case alphanumeric words separated by '_' (underscore)
  #define THIS_IS_MACRO(x) (x+x) 

* Namespace:
  Small case alphanumeric words separated by '_' (underscore), under insilico, preferably one word
  simulator, input_generator

* Neuron class:
  N_<neuron_type>_<authors><year>
  N_SquidAxon_HH1952

* Neuron file and extension:
  N_<neuron_type>_<authors><year>.hpp
  N_SquidAxon_HH1952.hpp

* Output file extension:
  'dat'
  output.dat, result_123.dat

* Source file extension:
  'cpp'
  main.cpp

* Synapse class:
  S_<synapse_type>_<authors><year>
  S_DefaultSynapse_XX (not yet published) 

* Synapse file and extension:
  S_<synapse_type>_<authors><year>.hpp
  S_DefaultSynapse_XX (not yet published).hpp

* Variables in the input file:
  variable_name_<channel_type|neuron_type|compartment_type|synapse_type>
  v_NaCa, v_dent
