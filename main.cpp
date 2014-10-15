//
// 		 main.cpp - nsim source supporting models and main()
//
//     Copyright (C) 2014 Pranav Kulkarni
//
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/support/pair.hpp>
#include <boost/numeric/odeint.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>

typedef class neuron {}n_;

typedef class neuron_network {
	public:
		static int variables_per_neuron;
		static long int network_strength;
		int id(int n_id, int var_id) {
			return n_id*variables_per_neuron+var_id;
		}
}nn_;
int nn_::variables_per_neuron = 4;
long int nn_::network_strength = 1;

typedef std::vector<long double> state_type;
typedef boost::range_iterator<state_type>::type boost_iter;
typedef boost::range_iterator<const state_type>::type boost_citer;

class value {
	private:
		long double datum;
	public:
		value() { datum = 0.0; }
		value(long double datum) { this->datum = datum; }
		long double get() const { return datum; }
		void set(long double datum) {	this->datum = datum; }
};

class current: public value {};

class voltage: public value {};

class conductance: public value {
	public:
		conductance(): value{0.0} {}
		conductance(long double conductance_value): value{conductance_value} {}
		void ode_set() {}
};

class probability_variable : public value {
	private:
		bool valid;
	public:
		probability_variable() : value{0.0} { valid = true; }
		probability_variable(long double probability_value): value{probability_value} {
			valid = (probability_value < 0 || probability_value > 1);
		}
		bool is_valid() { return valid; }
};

class na_conductance: public conductance {
	public:
		probability_variable m, h;
		na_conductance(): conductance{0.0} { m.set(0.0); h.set(0.0); }
		void ode_set(boost_citer &variables, boost_iter &dxdt, const double t) {
			double alpha_m = 	(2.5-0.1*variables[0])/(exp(2.5-0.1*variables[0])-1.0);
			double beta_m =  	4.0*exp(-variables[0]/18.0);
		  double alpha_h = 	0.07*exp(-variables[0]/20.0);
			double beta_h =  	1.0/(exp(3-0.1*variables[0])+1);

			dxdt[2]=(alpha_m*(1-variables[2])-beta_m*variables[2]);
			dxdt[3]=(alpha_h*(1-variables[3])-beta_h*variables[3]);
		}
};

class k_conductance: public conductance {
	public:
		probability_variable n;
		k_conductance(): conductance{0.0} { n.set(0.0); }
		void ode_set(boost_citer &variables, boost_iter &dxdt, const double t) {
			double alpha_n = (0.1-0.01*variables[0])/(exp(1-0.1*variables[0])-1.0);
			double beta_n = 0.125*exp(-variables[0]/80.0);

			dxdt[1]=(alpha_n*(1-variables[1])-beta_n*variables[1]);
		}
};

class leak_conductance: public conductance {
	public:
		leak_conductance(): conductance{0.0} {}
		void ode_set(boost_citer &variables, boost_iter &dxdt, const double t) {}
};

struct configuration {
	std::ofstream &stream;
	configuration(std::ofstream &file): stream(file) {}
	void operator()(const state_type &variables, const double t) {
		assert(stream.is_open());
		stream<<t;
		for(double variable: variables){
			stream<<','<<variable;
		}
		stream<<std::endl;
	}
};

class simulation_support_engine {
	public:
		template<typename t>
		static state_type stage_variables(const t& neurons) {
			return neurons.get_variables();
		}
};

class hodgkin_huxley_neuron: public neuron {
	public:
		na_conductance gna;
		k_conductance gk;
		leak_conductance gl;
		voltage ena, ek, el, v;
		current iext;
		hodgkin_huxley_neuron() {
			gna.set  (0.0); gk.set (0.0); gl.set (0.0);
			ena.set  (0.0);	ek.set (0.0);	el.set (0.0);
			iext.set (0.0);	v.set  (0.0);
		}
		state_type get_variables() const {
			state_type nvars = {v.get(), gk.n.get(), gna.m.get(), gna.h.get()};
			return nvars;
		}
		void ode_set(boost_citer &variables, boost_iter &dxdt, const double t) {
			dxdt[0] = -gna.get()*pow(variables[2],3)*variables[3]*(variables[0]-ena.get())
				-gk.get()*pow(variables[1],4)*(variables[0]-ek.get())
				-gl.get()*(variables[0]-el.get())+iext.get();
		}
		void operator()(const state_type &variables_, state_type &dxdt_,
			const double time) {
			decltype(boost::begin(variables_)) variables = boost::begin(variables_);
			decltype(boost::begin(dxdt_)) dxdt = boost::begin(dxdt_);

			ode_set(variables, dxdt, time);
			gk.ode_set(variables, dxdt, time);
			gna.ode_set(variables, dxdt, time);
			gl.ode_set(variables, dxdt, time);
		}
};

int main(int argc, char* argv[]) {
	if(argc != 2) {
		std::cout<<"Usage: "<<argv[0]<<" <outputfile>.dat"<<std::endl;
		return -1;
	}

	hodgkin_huxley_neuron neuron;
	state_type variables;

	neuron.gna.set(120.0);
	neuron.ena.set(115);
	neuron.gk.set(36);
	neuron.ek.set(-12.0);
	neuron.gl.set(0.3);
	neuron.el.set(10.6);
	neuron.iext.set(10);
	neuron.v.set(-30.0);
	neuron.gna.m.set(0.0);
	neuron.gna.h.set(0.0);
	neuron.gk.n.set(0.0);

	variables = simulation_support_engine::stage_variables(neuron);

	std::string filename = argv[1];
	std::ofstream file(filename);
	assert(file.is_open());

	using namespace boost::numeric::odeint;
	integrate_const(runge_kutta4<state_type>(), neuron, variables,
									0.0, 100.0, 0.05, configuration(file));

	file.close();
	return 0;
}
