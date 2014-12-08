/*
 network/nnet.hpp - nsim's Neuronal Network API header

 Copyright (C) 2014 Collins Assisi, Collins Assisi Lab, IISER, Pune
 Copyright (C) 2014 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

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

#ifndef INCLUDED_NNET_HPP
#define INCLUDED_NNET_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

#define NEURON 1
#define SYNAPSE 2

typedef std::vector<long double> state_type;

typedef class neuronal_network {
 public:
  static std::unordered_map<std::string, long> index_map;
  static std::unordered_map<std::string, long double> value_map;
  static std::vector<long> neuron_start_list_ids;
  static std::vector<long> neuron_end_list_ids;
  static std::vector<long> synapse_start_list_ids;
  static std::vector<long> synapse_end_list_ids;
  static std::vector<long> pre_neuron, post_neuron;
  static std::vector<std::string> var_list_ids;
  static state_type var_vals;
  static state_type get_variables();
  static long get_index(long, std::string, int);
  static long neuron_index(long id, std::string variable);
  static long double neuron_value(long id, std::string variable);
  static long synapse_index(long id, std::string variable);
  static long double synapse_value(long id, std::string variable);
  static double get_value(long index);
  static double get(long id, std::string variable, int mode);
  static std::vector<long> get_indices(std::string variable);
  static std::vector<long> get_pre_neuron_indices(long neuron_id, std::string variable);
  static void read(std::string neuron_file, std::string synapse_file="");
  static long neuron_count();
  static long synapse_count();
  void operator()(const state_type &variables, state_type &dxdt, const double time);
}nnet;

#endif
