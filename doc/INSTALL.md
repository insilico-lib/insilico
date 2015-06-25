insilico - Neuronal Simulation Library
======================================

Installation
============

insilico's install guide. Please read through to get started with insilico.

Pre-requisites
--------------

insilico is written in C++ and uses C++11 standard. And for numerical integration insilico uses Boost.odeint.

[+] On Linux
 |
 |- Boost 1.56.0+
 |- g++ 4.7.2+
 '- make 3.8+

[+] On Mac OS X
 |
 |- Boost 1.56.0+
 '- Xcode Command Line Tools (for g++ and make)

[+] On Windows
 |
 |- Visual Studio 2012+ Express edition
 |- Boost 1.56.0+
 '- MinGW 3.7+ (for g++ & make)

(Optionally, for all the platforms)
 - Python 2.7+ - if you want to use bundled scripts for generating input files
 - Gnuplot - if you want to use bundled scripts for plotting simulation results

The installation of above softwares for all platforms is expected to be in standard directories. Please do any essential manual modifications that are required to the environment for using this library.

Platforms
---------

We wish to make this library portable across platform. The known minimum versions for a given platforms or compatible platforms:

Linux:    Ubuntu 11.04 +
Mac:      OS X 10.0.0 +
Windows:  XP +

Source Inclusion
----------------

insilico is a C++ headers with source library. Include the respective files from the library to get the API for your simulation.
