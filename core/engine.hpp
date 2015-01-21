/*
 core/engine.hpp - insilico's Simulation Engine API header and source

 Copyright (C) 2014,2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

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

#ifndef INCLUDED_ENGINE_HPP
#define INCLUDED_ENGINE_HPP

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Mandate: Use of state_type should include this file
typedef vector<double> state_type;

//
// Map based search logic can be activated by defining MAP as given below
// #define MAP
//

char key[128];

namespace insilico {

class engine {

 public:
  
  static unordered_map<string, int> index_map;
  static unordered_map<string, double> value_map;
  static vector<int> neuron_start_list_ids;
  static vector<int> neuron_end_list_ids;
  static vector<int> synapse_start_list_ids;
  static vector<int> synapse_end_list_ids;
  static vector< vector<int> > pre_synaptic_lists;
  static vector<int> pre_neuron;
  static vector<int> post_neuron;
  static vector<string> var_list_ids;
  static vector<double> var_vals;

  static vector<double> get_variables() {
    return var_vals;
  }

  static int neuron_index(int id, string variable) {
#ifdef MAP
    try {
      sprintf(key, "n%d%s", id, variable.c_str());
      return index_map[key];
    }
    catch(...) {
      cout<<"[insilico::engine] Simulator Exception:"
          <<" neuron_index method supplied with incorrect arguments."<<endl
          <<"Arguments were: [neuron_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    }
    exit(0);
#else
    int startindex = neuron_start_list_ids.at(id);
    int endindex = neuron_end_list_ids.at(id);
    for(int iter=startindex;iter<endindex;++iter) {
      if(variable.compare(var_list_ids[iter]) == 0) {
        return iter;
      }
    }
    cout<<"[insilico::engine] Simulator Exception:"
        <<" neuron_index method supplied with incorrect arguments."<<endl
        <<"Arguments were: [neuron_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    exit(0);
#endif
  }

  static double neuron_value(int id, string variable) {
    try {
      sprintf(key, "n%d%s", id, variable.c_str());
      return value_map[key];
    }
    catch(...) {
      cout<<"[insilico::engine] Simulator Exception:"
          <<" neuron_value method supplied with incorrect arguments."<<endl
          <<"Arguments were: [neuron_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    }
    exit(0);
  }

  static void neuron_value(int id, string variable, double value) {
    sprintf(key, "n%d%s", id, variable.c_str());
    value_map[key] = value;
  }

  static int synapse_index(int id, string variable) {
#ifdef MAP
    try {
      sprintf(key, "s%d%s", id, variable.c_str());
      return index_map[key];
    }
    catch(...) {
      cout<<"[insilico::engine] Simulator Exception:"
          <<" synapse_index method supplied with incorrect arguments."<<endl
          <<"Arguments were: [synapse_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    }
    exit(0);
#else
    int startindex = synapse_start_list_ids.at(id);
    int endindex = synapse_end_list_ids.at(id);
    for(int iter=startindex;iter<endindex;++iter) {
      if(variable.compare(var_list_ids[iter]) == 0) {
        return iter;
      }
    }
    cout<<"[insilico::engine] Simulator Exception:"
        <<" synapse_index method supplied with incorrect arguments."<<endl
        <<"Arguments were: [synapse_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    exit(0);
#endif
  }

  static double synapse_value(int id, string variable) {
    try {
      sprintf(key, "s%d%s", id, variable.c_str());
      return value_map[key];
    }
    catch(...) {
      cout<<"[insilico::engine] Simulator Exception:"
          <<" synapse_value method supplied with incorrect arguments."<<endl
          <<"Arguments were: [synapse_index = "<<id<<"][variable = "<<variable<<"]"<<endl;
    }
    exit(0);
  }

  static void synapse_value(int id, string variable, double value) {
    sprintf(key, "s%d%s", id, variable.c_str());
    value_map[key] = value;
  }

  static double current_value(int id, string variable) {
    sprintf(key, "s%d%s", id, variable.c_str());
    return value_map[key];
  }

  static void current_value(int id, string variable, double value) {
    sprintf(key, "s%d%s", id, variable.c_str());
    value_map[key] = value;
  }

  static vector<int> get_indices(string variable) {
    vector<int> indices;
    int neuron_size = neuron_count();
    for(int index = 0; index < neuron_size; ++index) {
      indices.push_back(neuron_index(index,variable));
    }
    if(indices.empty()) {
      cout<<"[insilico::engine] Simulation Exception:"
          <<" get_indices supplied with incorrect arguments."<<endl
          <<"Searching for indices of variable = "<<variable<<" failed."<<endl;
      exit(0);
    }
    return indices;
  }

  static vector<int> get_pre_neuron_indices(int neuron_id, string variable) {
    vector<int> indices;
    if(!pre_synaptic_lists.empty()) {
      for(vector<int>::size_type index = 0; index < pre_synaptic_lists[neuron_id].size(); ++index) {
        indices.push_back(synapse_index(pre_synaptic_lists[neuron_id][index], variable));
      }
    }
    return indices;
  }

  static vector<int> get_pre_neuron_values(int neuron_id, string variable) {
    vector<int> values;
    if(!pre_synaptic_lists.empty()) {
      for(vector<int>::size_type index = 0; index < pre_synaptic_lists[neuron_id].size(); ++index) {
        values.push_back(synapse_value(pre_synaptic_lists[neuron_id][index], variable));
      }
    }
    return values;
  }

  static void populate_pre_synaptic_lists() {
    if(!post_neuron.empty()) {
      pre_synaptic_lists.resize( *max_element(post_neuron.begin(), post_neuron.end()) + 1 );
      for(vector<int>::size_type iterator = 0; iterator < post_neuron.size(); ++iterator) {
        pre_synaptic_lists[ post_neuron[iterator] ].push_back( iterator );
      }
    }
  }
  
  static inline int neuron_count() {
    return neuron_start_list_ids.size();
  }

  static inline int synapse_count() {
    return synapse_start_list_ids.size();
  }

  void operator()(vector<double>& variables, vector<double>& dxdt, const double time);

}; // class engine

// static member definitions
unordered_map<string, int> engine::index_map;
unordered_map<string, double> engine::value_map;
vector<int> engine::neuron_start_list_ids;
vector<int> engine::neuron_end_list_ids;
vector<int> engine::synapse_start_list_ids;
vector<int> engine::synapse_end_list_ids;
vector<vector<int>> engine::pre_synaptic_lists;
vector<int> engine::pre_neuron;
vector<int> engine::post_neuron;
vector<string> engine::var_list_ids;
vector<double> engine::var_vals;

} // insilico

#endif
