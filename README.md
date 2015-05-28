insilico
========

![GNU GPLv3 License](http://img.shields.io/badge/license-GPLv3-green.svg)
[![insilico Trello](https://img.shields.io/badge/Trello-insilico-blue.svg)](https://trello.com/b/lkXzPGqD/insilico)

`insilico` is a Computational Neuroscience simulation library written in C++. `insilico` encourages ready-to-start approach for quick setup of simulation environment, without hindering programmers time and focus from intended experiment.

Library homepage: http://insilico-lib.github.io/insilico/

Download `insilico`'s Source
========================

We currently have insilico C++ source available in beta versions. These can be downloaded from [`insilico`'s Releases](https://github.com/insilico-lib/insilico/releases) page.

Install insilico
================

Please refer to [INSTALL](https://github.com/insilico-lib/insilico/blob/master/doc/INSTALL) file for more details on installation.

Support and Contact info
========================

- Google Groups: https://groups.google.com/forum/#!forum/insilico
- Email: insilicolib@gmail.com
- Github issues: https://github.com/insilico-lib/insilico/issues

Example
=======
```C++11
#include "insilico/core.hpp"
#include <boost/numeric/odeint.hpp>

using namespace insilico;

int main(int argc, char **argv) {
  configuration::initialize(argc, argv);
  configuration::observe("v");

  engine::generate_neuron<HH_Neuron>();

  state_type variables = engine::get_variables();
  integrate_const(boost::numeric::odeint::runge_kutta4<state_type>(),
                  engine::driver(), variables,
                  0.0, 100.0, 0.05, configuration::observer());

  configuration::finalize();
}
```

Documentation
=============

You can find total documentation: here (will be updated soon)

Quick Links:

- [Download Source](https://github.com/insilico-lib/insilico/releases)
- List of functions (Coming soon)
- Tutorial (Coming soon)
- [Examples](https://github.com/insilico-lib/insilico/tree/master/examples)

Contribute
==========

Contributions are more than welcome! Please refer to this [document](https://github.com/insilico-lib/insilico/blob/master/doc/CONTRIBUTION) to learn more.

insilico License
================

This simulator library is licensed under GNU GPLv3 which can be found in `LICENSE` file under home directory of this project.
