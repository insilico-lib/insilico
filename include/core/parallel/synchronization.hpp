/*
 parallel/synchronization.hpp - Synchronization helper for parallel execution

 Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>
 
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

#ifndef INCLUDED_INSILICO_PARALLEL_SYNCHRONIZATION_HPP
#define INCLUDED_INSILICO_PARALLEL_SYNCHRONIZATION_HPP

#include <vector>

namespace insilico {

namespace synchronization {

class event {
 public:
  // storing events specific to key-value store based structures
  static std::vector< std::string > sync_keys;
  static std::vector< double > sync_values;

  // storing events specific to specialized structures (dxdt variables in insilico)
  static const char NEURON = 'n';
  static const char SYNAPSE = 's';
  static std::vector< char > sync_type;
  static std::vector< int > sync_variable_index;

  // initialize the event
  static void initialize() {
    clear();
  }
  
  // registering event for key-value structure
  static void register_update(std::string key, double value) {
    sync_keys.push_back(key);
    sync_values.push_back(value);
  }

  // registering event for specialized structure (dxdt variables in insilico) 
  static void register_update(char type, int variable_index) {
    sync_type.push_back(type);
    sync_variable_index.push_back(variable_index);
  }

  // clear all events once synchronized
  static void clear() {
    sync_keys.clear();
    sync_values.clear();
    sync_type.clear();
    sync_variable_index.clear();
  }
};

std::vector< std::string > event::sync_keys;
std::vector< double > event::sync_values;
std::vector< char > event::sync_type;
std::vector< int > event::sync_variable_index;

} // namespace synchronization

} // namespace insilico

#endif
