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

typedef std::vector<float> state_type;

typedef class neuronal_network {
 public:
  static std::unordered_map<std::string, int> index_map;
  static std::unordered_map<std::string, float> value_map;
  static std::vector<int> neuron_start_list_ids;
  static std::vector<int> neuron_end_list_ids;
  static std::vector<int> synapse_start_list_ids;
  static std::vector<int> synapse_end_list_ids;
  static std::vector< std::vector<int> > pre_synaptic_lists;
  static std::vector<int> pre_neuron;
  static std::vector<int> post_neuron;
  static std::vector<std::string> var_list_ids;
  static state_type var_vals;
  static state_type get_variables();

  static int neuron_index(int id, std::string variable);
  static float neuron_value(int id, std::string variable);

  static int synapse_index(int id, std::string variable);
  static float synapse_value(int id, std::string variable);

  static std::vector<int> get_indices(std::string variable);
  static std::vector<int> get_pre_neuron_indices(int neuron_id, std::string variable);
  static std::vector<int> get_pre_neuron_values(int neuron_id, std::string variable);
  static void populate_pre_synaptic_lists();

  static void read(std::string neuron_file, std::string synapse_file="");
  
  static int neuron_count();
  static int synapse_count();

  void operator()(const state_type &variables, state_type &dxdt, const double time);
}nnet;

#endif
