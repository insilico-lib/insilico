/*
 core/configuration/observer.hpp - insilico's Simulation output observer

 Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab,
                    IISER, Pune <pranavcode@gmail.com>

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
/**
 * @file core/configuration/observer.hpp
 *
 * insilico's Configuration settings for observing simulation result.
 */

#pragma once

#include "insilico/core/configuration/error.hpp"
#include "insilico/core/engine/serial/common.hpp"
#include "insilico/core/engine/serial/index.hpp"
#include "insilico/core/engine/serial/value.hpp"
#include "insilico/core/type.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace insilico {
namespace configuration {

/**
 * Collection of strings of all known header elements to be observed.
 */
std::vector<std::string> observation_header;

/**
 * Pre-computed collection of keys for variables and parameters
 * to be observed.
 */
std::vector<std::string> pre_computed_keys;

/**
 * Pre-computaed collection of indices for variables
 * to be observed.
 */
std::vector<int> pre_computed_indices;

/**
 * Observation output file stream.
 */
std::ofstream outstream;

/**
 * Observation file output stream buffer for step-level faeture.
 */
std::stringstream write_buffer;

/**
 * Temporary flag for checking if the one-time header is written
 * to the observation file or not.
 */
bool header_observed_flag = false;

/**
 * Holds the decision, whether the step-level interval feature
 * is active or not, default is inactive.
 */
bool step_interval_feature = false;

/**
 * Holds the value for step-level interval within
 * two consecutive observations.
 */
unsigned step_interval = 0;

/**
 * Keeps the count of intermediate steps for step interval
 * based observations.
 */
unsigned step_ctr = 0;

/**
 * Holds the value for delimiter for columns in observation file.
 */
char observer_delimiter = ',';

/**
 * Holds the boolean value for whether observation header should be
 * enabled or disabled.
 */
bool observer_header = true;

/**
 * Allows to change the column separator for observation file.
 *
 * @param delim character to differentiate the values between two
 *              columns is observation file, default delimiter is ','
 */
void observe_delimiter(const char delim) {
  observer_delimiter = delim;
}

/**
 * Allows to enable or disable observation file header.
 *
 * @param flag observation file will get header only if this
 *             flag is set true, else otherwise, and default
 *             behavior says observation header should be written
 *             to output file.
 */
void observe_header(const bool flag) {
  observer_header = flag;
}

/**
 * Sets the step-level interval for observation.
 *
 * @param interval positive interger value for number of
 *                 steps to be observed at once.
 */
void observe_step_interval(const unsigned interval) {
  step_interval = interval;
  step_interval_feature = true;
}

/**
 * Generate the observation header string to put into
 * observation file.
 */
void build_header_once() {
  char key[128];
  bool error = false;
  unsigned local_id;
  std::string var;
  for(unsigned index : pre_computed_indices) {
    local_id = engine::neuron_id_from_index(index, error);
    if(!error) {
      var = engine::variable_name_from_index(index);
      sprintf(key, "n%d%s", local_id, var.c_str());
      observation_header.push_back(key);
    }
    else {
      local_id = engine::synapse_id_from_index(index, error);
      if(!error) {
        var = engine::variable_name_from_index(index);
        sprintf(key, "s%d%s", local_id, var.c_str());
        observation_header.push_back(key);
      }
    }
  }
  for(std::string value_key : pre_computed_keys) {
    observation_header.push_back(value_key);
  }
}

/**
 * Writes the observation header to the simulation output file.
 */
void write_header_once() {
  if(observer_header && !header_observed_flag) {
    build_header_once();
    outstream << "time";
    for(std::string observe_this : observation_header) {
      outstream << observer_delimiter << observe_this;
    }
    outstream << '\n';
    header_observed_flag = true;
  }
}

/**
 * Configuration's custom entry point for observation,
 * with overloaded parenthesis operator.
 */
struct observer {

  /**
   * Entry point for Boost.odeint observation function call.
   *
   * @param variables Ref to all variables involved in simulation.
   * @param t current time in simulation.
   */
  void operator() (state_type &variables, const double t) {
    write_header_once();
    if(step_ctr >= step_interval) {
      outstream << write_buffer.str();
      write_buffer.str("");
      step_ctr = 0;
    }
    write_buffer << t;
    for(unsigned local_id : pre_computed_indices) {
      write_buffer << observer_delimiter << (variables[local_id]);
    }
    for(std::string key : pre_computed_keys) {
      write_buffer << observer_delimiter << (engine::value_map[key]);
    }
    write_buffer << '\n';
    ++step_ctr;
  }

}; // struct observer

/**
 * Generic function for observing any variable or parameter with given name,
 * where variable or parameter can belong to either Neuron or Synapse.
 *
 * @param observe_this name of variable or parameter to be observed.
 */
void observe(std::string observe_this) {
  std::vector< unsigned > neuron_indices;
  std::vector< unsigned > synapse_indices;
  std::vector< std::string > neuron_keys;
  std::vector< std::string > synapse_keys;

  neuron_indices = engine::get_neuron_indices(observe_this);
  synapse_indices = engine::get_synapse_indices(observe_this);
  neuron_keys = engine::get_neuron_value_keys(observe_this);
  synapse_keys = engine::get_synapse_value_keys(observe_this);

  // variable does not match to either variable or parameter
  if(neuron_indices.empty() && synapse_indices.empty() &&
     neuron_keys.empty() && synapse_keys.empty()) {
    std::cerr << "[insilico::configuration] Observer failed to find "
              << observe_this << ".\n";
    configuration::severe_error();
  }
  if(!neuron_indices.empty()) {
    pre_computed_indices.insert(pre_computed_indices.end(),
                                neuron_indices.begin(),
                                neuron_indices.end());
  }
  else if(!neuron_keys.empty()) {
    pre_computed_keys.insert(pre_computed_keys.end(),
                             neuron_keys.begin(),
                             neuron_keys.end());
  }
  if(!synapse_indices.empty()) {
    pre_computed_indices.insert(pre_computed_indices.end(),
                                synapse_indices.begin(),
                                synapse_indices.end());
  }
  else if(!synapse_keys.empty()) {
    pre_computed_keys.insert(pre_computed_keys.end(),
                             synapse_keys.begin(),
                             synapse_keys.end());
  }
}

/**
 * Enable the observation of variable or parameter with given name and
 * belonging to given Neuron ID.
 *
 * @param local_id Neuron ID.
 * @param observe_this name of Neuronal variable or parameter to be observed.
 */
void observe_neuron(unsigned local_id, std::string observe_this) {
  bool error_index = false;
  int neuron_index = engine::neuron_index(local_id, observe_this, error_index);
  std::string neuron_key = engine::neuron_value_key(local_id, observe_this);
  if(error_index && (neuron_key.compare("") == 0)) {
    std::cerr << "[insilico::configuration] Observer failed to find "
              << observe_this << " for Neuron " << local_id << ".\n";
    configuration::severe_error();
  }
  else if(!error_index) {
    pre_computed_indices.push_back(neuron_index);
  }
  else {
    pre_computed_keys.push_back(neuron_key);
  }
}

/**
 * Enable the observation of all variables or parameters with given name and
 * belonging to given set of Neuron IDs.
 *
 * @param local_ids collection of Neuron IDs.
 * @param observe_this name of Neuronal variables or parameters to be observed.
 */
void observe_neuron(std::vector<unsigned> local_ids, std::string observe_this) {
  for(unsigned local_id : local_ids) {
    observe_neuron(local_id, observe_this);
  }
}

/**
 * Enable the observation of variable or parameter with given name and
 * belonging to given Synapse ID.
 *
 * @param local_id Synapse ID.
 * @param observe_this name of Synaptic variable or parameter to be observed.
 */
void observe_synapse(unsigned local_id, std::string observe_this) {
  bool error_index = false;
  int synapse_index = engine::synapse_index(local_id, observe_this, error_index);
  std::string synapse_key = engine::synapse_value_key(local_id, observe_this);
  if(error_index && (synapse_key.compare("") == 0)) {
    std::cerr << "[insilico::configuration] Observer failed to find "
              << observe_this << " for Synapse " << local_id << ".\n";
    configuration::severe_error();
  }
  else if(!error_index) {
    pre_computed_indices.push_back(synapse_index);
  }
  else {
    pre_computed_keys.push_back(synapse_key);
  }
}

/**
 * Enable the observation of all variables or parameters with given name and
 * belonging to given set of Synapse IDs.
 *
 * @param local_ids collection of Synapse IDs.
 * @param observe_this name of Synaptic variables or parameters to be observed.
 */
void observe_synapse(std::vector<unsigned> local_ids, std::string observe_this) {
  for(unsigned local_id : local_ids) {
    observe_synapse(local_id, observe_this);
  }
}

} // namespace configuration
} // namespace insilico
