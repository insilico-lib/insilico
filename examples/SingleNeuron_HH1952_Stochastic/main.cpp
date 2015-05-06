/*
  examples/SingleNeuron_HH1952/main.cpp - insilico's example using neuron

  Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab,
                     IISER, Pune <pranavcode@gmail.com>
  Copyright (C) 2014 Arun Neru, Collins Assisi Lab,
                     IISER, Pune <areinsdel@gmail.com>
  Copyright (C) 2015 Himanshu Rajmane, Suhita Nadkarni Lab,
                     IISER, Pune <himanshu14121992@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "insilico/core.hpp"

#include <boost/numeric/odeint.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <random>

using namespace insilico;
using namespace std;
using namespace boost::numeric::odeint;

class I_Na {
 private:
  const double gna = 120;
  const double ena = 115;
 public:
  void current(state_type &variables, state_type &dxdt,
               const double t, const unsigned index) {
    int v_index = engine::neuron_index(index, "v");
    int m_index = engine::neuron_index(index, "m");
    int h_index = engine::neuron_index(index, "h");

    double v = variables[v_index];
    double m = variables[m_index];
    double h = variables[h_index];

    double alpha_m = (2.5 - 0.1 * v) / (exp(2.5-0.1 * v) - 1.0);
    double beta_m  = 4.0 * exp(-v / 18.0);
    double alpha_h = 0.07 * exp(-v / 20.0);
    double beta_h  = 1.0 / (exp(3 - 0.1 * v) + 1);

    dxdt[m_index] = (alpha_m * (1-m) - beta_m * m);
    dxdt[h_index] = (alpha_h * (1-h) - beta_h * h);

    engine::neuron_value(index, "I_Na", (gna * pow(m, 3) * h * (v - ena)));
  }
};

class I_K {
 private:
  const double gk = 36;
  const double ek = -12;
 public:
  void current(state_type &variables, state_type &dxdt,
               const double t, const unsigned index) {
    int v_index = engine::neuron_index(index, "v");
    int n_index = engine::neuron_index(index, "n");

    double v = variables[v_index];
    double n = variables[n_index];

    double alpha_n = (0.1 - 0.01 * v) / (exp(1 - 0.1 * v) - 1.0);
    double beta_n  = 0.125 * exp(-v / 80.0);

    dxdt[n_index]=(alpha_n*(1 - n)-beta_n * n);

    engine::neuron_value(index, "I_K", (gk * pow(n,4) * (v - ek)));
  }
};

class I_Leak {
 private:
  const double gl = 0.3;
  const double el = 10.6;
 public:
  void current(state_type &variables, state_type &dxdt,
               const double t, const unsigned index) {
    int v_index = engine::neuron_index(index, "v");
    double v = variables[v_index];

    engine::neuron_value(index, "I_Leak", (gl * (v - el)));
  }
};

class HH_Neuron : public Neuron {
 private:
  I_Na i_na_component;
  I_K i_k_component;
  I_Leak i_leak_component;
 public:
  void ode_set(state_type &variables, state_type &dxdt,
               const double t, unsigned index) {
    int v_index = engine::neuron_index(index, "v");

    i_na_component.current(variables, dxdt, t, index);
    i_k_component.current(variables, dxdt, t, index);
    i_leak_component.current(variables, dxdt, t, index);

    double I_Na = engine::neuron_value(index, "I_Na");
    double I_K = engine::neuron_value(index, "I_K");
    double I_Leak = engine::neuron_value(index, "I_Leak");
    double I_Ext = engine::neuron_value(index, "I_Ext");

    dxdt[v_index] = - I_Na - I_K - I_Leak + I_Ext;
  }
};

class random {
 public:
  template<class T>
  static T rand(T mean) {
    random_device rd;
    mt19937_64 gen(rd());
    poisson_distribution<> dist(mean);
    return dist(gen);
  }
};

class stoch_driver {
 public:
  void operator()(state_type &variables, state_type &dxdt, const double time) {
    string var_str, var_str_cpy;
    double e_noise, i_noise;
    unsigned nid, sid;
    bool nerror = false, serror = false;

    for(unsigned index = 0 ; index < variables.size() ; ++index ) {
      var_str = engine::variable_name_from_index(index);
      var_str_cpy = var_str;
      var_str_cpy.append("_stoch"); // stochastic variable from input file
      nid = engine::neuron_id_from_index(index, nerror);
      sid = engine::synapse_id_from_index(index, serror);
      if(!nerror) {
        if(engine::neuron_value(nid, var_str_cpy) > 0) {
          e_noise = random::rand(4.928);
          i_noise = random::rand(4.2);
          dxdt[index] = e_noise - i_noise;
        }
      }
      else if(!serror) {
        if(engine::synapse_value(sid, var_str_cpy) > 0) {
          e_noise = random::rand(4.928);
          i_noise = random::rand(4.2);
          dxdt[index] = e_noise - i_noise;
        }
      }
    }
  }
};

class stochastic_euler {
 public:
  using state_type = vector<double>;
  using deriv_type = vector<double>;
  using value_type = double;
  using time_type = double;
  using order_type = unsigned short;
  using stepper_category = boost::numeric::odeint::stepper_tag;
  static order_type order() { return 1; }

  template<class System>
  void do_step(System system, state_type &variables,
               const time_type t, time_type dt) const {
    deriv_type det(variables.size());
    deriv_type stoch(variables.size());
    system.first(variables , det, t);
    system.second(variables, stoch, t);
    for(unsigned i=0; i < variables.size(); ++i) {
      variables[i] += dt * det[i] + sqrt( dt ) * stoch[i];
    }
  }
};

int main(int argc, char **argv) {
  configuration::initialize(argc, argv);
  configuration::observe_header(false);
  configuration::observe("v");

  engine::generate_neuron<HH_Neuron>(1);

  state_type variables = engine::get_variables();
  integrate_const(stochastic_euler(),
                  make_pair(engine::driver(), stoch_driver()),
                  variables, 0.0, 50.0, 0.01, configuration::observer());

  configuration::finalize();
}
