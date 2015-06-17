/*
 core/configuration/serial.hpp - insilico Configuration API header

 Copyright (C) 2014-2015 Pranav Kulkarni, Collins Assisi Lab,
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
 * @file core/configuration/serial.hpp
 *
 * insilico's Configuration initialization and finalization API
 */

#pragma once

#include "insilico/core/engine.hpp"
#include "insilico/core/dynamic_params.hpp"
#include "insilico/core/configuration/observer.hpp"
#include "insilico/core/configuration/parser.hpp"
#include "insilico/core/helper/file.hpp"
#include "insilico/core/helper/string.hpp"
#include "insilico/core/injector.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace insilico {
namespace configuration {

/**
 * Initialize insilico's Simulation environment.
 *
 * @param argc count for command line arguments, mostly an input param from
 *             C++ main() function.
 * @param argv pointer to the collection of string command line arguments.
 */
void initialize(int argc, char **argv) {
  // Error message for single time usage of a given command
  // line option.
  std::string error_msg = "[insilico::configuration::initialize]"
      " Supply of file more than once not allowed.\n";

  // Usage message during execution on command line interface.
  std::string usage_error_msg = "[insilico::configuration::initialize] USAGE: ";
  usage_error_msg += argv[0];
  usage_error_msg += " -o <output_file.csv> -n <neuron_file.isf> "
      "-s <synapse_file.isf> -e <external_file.isfc> -d <dynamic_params.isfd>\n"
      "\n    Options:\n"
      "\t-o   Output file\n"
      "\t-n   Neuron configuration file\n"
      "\t-s   Synapse configuration file (optional)\n"
      "\t-e   External current configuration file (optional)\n"
      "\t-d   Dynamic parameters configuration file (optional)\n\n";

  // Boolean value for checking whether the specific command
  // line option is present (true) or not (false).
  // Default is not present (false).
  bool repeat[5] = {false, false, false, false, false};

  // Construct a combined command line, separated with spaces.
  std::string sargv;
  for(int i=1; i < argc; ++i) {
    sargv += argv[i];
    sargv += ' ';
  }

  // Split the combined command line arguments using space delimiter.
  std::vector<std::string> cmds = split(sargv, ' ');

  // Check for proper number of command line arguments are
  // supplied or not.
  // Dependent on current command line features, min: 4, max: 10.
  if(cmds.size() < 4 || cmds.size() > 10) {
    std::cerr << usage_error_msg; exit(0);
  }

  // Message displayed at start of initialization process.
  std::cout << "[insilico::configuration::initialize] SUCCESS: Initializing with following parameters:\n";

  // Local variables used for initialization.
  std::string output_file, neuron_file, synapse_file,
      external_current_file, dynamic_params_file;

  // Iterate over all command line arguments now.
  for(unsigned iter = 0; iter < cmds.size(); iter+=2) {
    std::string cmd = cmds[iter];
    // For each pair of command line argument, second character of
    // first element will tell which option is being used.
    switch(cmd.at(1)) {

      case 'o': // Operation for Output file option.
        if(repeat[0]) {
          std::cerr << error_msg << '\n' << usage_error_msg;
          exit(1);
        }
        output_file = cmds[iter+1];
        std::cout << "Output file: " << output_file << '\n';
        repeat[0] = true;
        break;

        // Synapse file is dependent on Neuron file.
        // If Synapse file is given without Neuron file, simulation cannot be continued,
        // Else If only Neuron file is given, simulation can be continued,
        // Else none of the files are given, simulation cannot be continued.
        // Neuron file is mandatory.
        // Synapse file is optional.

      case 'n': // Operation for Neuron file option.
        if(repeat[1]) {
          std::cerr << error_msg << '\n' << usage_error_msg;
          exit(1);
        }
        neuron_file = cmds[iter+1];
        std::cout << "Neuron file: " << neuron_file << '\n';
        repeat[1] = true;
        break;

      case 's': // Operation for Synapse file option.
        if(!repeat[1]) {
          std::cerr << "[insilico::configuration::initialize]"
              " Synapse file cannot be supplied without Neuron file.\n"
                    << usage_error_msg;
          exit(1);
        }
        if(repeat[2]) {
          std::cerr << error_msg << usage_error_msg;
          exit(1);
        }
        synapse_file = cmds[iter+1];
        std::cout << "Synapse file: " << synapse_file << '\n';
        repeat[2] = true;
        break;

      case 'e': // Operation for External Current file option.
        if(repeat[3]) {
          std::cerr << error_msg << '\n'<<usage_error_msg;
          exit(1);
        }
        external_current_file = cmds[iter+1];
        std::cout << "External current file: " << external_current_file << '\n';
        repeat[3] = true;
        break;

      case 'd': // Operation for Dynamic Params file option.
        if(repeat[4]) {
          std::cerr << error_msg << '\n' << usage_error_msg;
          exit(1);
        }
        dynamic_params_file = cmds[iter+1];
        std::cout << "Dynamic parameters file: " << dynamic_params_file << '\n';
        repeat[4] = true;
        break;

      default: // Operation for unknown option.
        std::cerr << "Error: Unknown argument " << cmd << std::endl
                  << usage_error_msg;
        exit(1);
    }
  }

  // Check if output file is given. This file is mandatory.
  if(repeat[0]) {
    outstream.open(output_file, std::ios::out);
  }
  else {
    std::cerr << "[insilico::configuration::initialize] Output file is required.\n";
    exit(1);
  }

  // Check Synapse file.
  if(repeat[2]) {
    read(neuron_file, synapse_file);
  }
  // Check Neuron file.
  else if(repeat[1]) {
    read(neuron_file);
  }
  // None of the files are present, display an error message for missing file.
  else {
    std::cerr << "[insilico::configuration::initialize] Neuron configuration file is required.\n";
    exit(1);
  }

  // Check if External Current file is given. This file is optional.
  if(repeat[3]) {
    injector::read(external_current_file);
  }

  // Check if Dynamic Params file is given. This file is optional.
  if(repeat[4]) {
    dynamic_params::read(dynamic_params_file);
  }

  // Notification message for successful input file reading.
  std::cout << "[insilico::configuration::read] SUCCESS: Input file read complete.\n";
}

/**
 * Finalize the simulation by releasing all the required resources.
 */
void finalize() {
  // Call observer to write out observation for delayed observation.
  // i.e. if configuraion::observe::delayed(true); is part of main().
  // configuraion::observe::stored_observation()

  // Close the observation simulation output file.
  outstream.close();

  // One can add other required finalization steps here.
  // Current system status requires only this much finalization.

  // Success message to represent the successful completion of simulation
  // and gives the resources release notification.
  std::cout << "[insilico::configuration::finalize] SUCCESS: Releasing resources.\n";
}

} // namespace configuration
} // namespace insilico
