/*
 core/configuration/mpi.hpp - insilico's MPI Configuration header and source

 Copyright (C) 2014 Collins Assisi, Collins Assisi Lab, IISER, Pune
 Copyright (C) 2014-2015 Pranav Kulkarni, Collins Assisi Lab, IISER, Pune <pranavcode@gmail.com>

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

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_MPI_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_MPI_HPP

#include "core/engine.hpp"
#include "core/helper/file.hpp"
#include "core/helper/string.hpp"

#include "core/configuration/parser.hpp"

#include "parallel/mpi.hpp"

namespace insilico { namespace configuration {

// initialization, check and handle commandline arguments
void initialize(int argc, char **argv) {
#ifdef INSILICO_MPI_ENABLE
  mpi::configuration::initialize(argc, argv);
#endif
  if(argc < 3 || argc > 4) {
#ifdef INSILICO_MPI_ENABLE
    if(mpi::rank == MASTER) {
#endif
      std::string usage_error_msg = "[insilico::configuration::initialize] USAGE: ";
      usage_error_msg += argv[0];
      usage_error_msg += " -o<output_file.csv> -n<neuron_file.isf> -s<synapse_file.isf> -e<external_file.isfc>\n\n \
                           Options:\n\t-o   Output file\n\t-n   Neuron configuration file\n\t-s   Synapse configuration file (optional)\n \
                           \t-e   External current configuration file (optional)\n             \
                           \nNo space allowed between option and its value.\n";
      std::cerr << usage_error_msg;
#ifdef INSILICO_MPI_ENABLE
    }
    mpi::abort();
#else
    exit(1);
#endif
  }
#ifdef INSILICO_MPI_ENABLE
  if(insilico::mpi::rank == MASTER) {
#endif
    std::cerr << "[insilico::configuration::initialize] SUCCESS: Initializing with following parameters:"<<'\n'
              << "Output file: " << argv[1] << '\n' << "Neuron file: " << argv[2] << '\n';
#ifdef INSILICO_MPI_ENABLE
  }
#endif
  outstream.open(argv[1], std::ios::out);
  std::string neuron_file(argv[2]);      
  if(argc == 4) {
#ifdef INSILICO_MPI_ENABLE
    if(insilico::mpi::rank == MASTER) {
#endif
      std::cerr << "Synapse file: " << argv[3] << '\n';
#ifdef INSILICO_MPI_ENABLE
    }
#endif
    std::string synapse_file(argv[3]);
    read(neuron_file, synapse_file);
  }
  else {
    read(neuron_file);
  }
#ifdef INSILICO_MPI_ENABLE
  if(insilico::mpi::rank == MASTER) {
#endif
    std::cerr << "[insilico::configuration::read] SUCCESS: Input file read complete." << '\n';
#ifdef INSILICO_MPI_ENABLE
  }
#endif
}
                 
// close all output streams
void finalize() {
#ifdef INSILICO_MPI_ENABLE
  if(insilico::mpi::rank == MASTER) {
#endif
    std::cerr <<"[insilico::configuration::finalize] SUCCESS: Simulation complete."<<'\n';
#ifdef INSILICO_MPI_ENABLE
  }
#endif
  outstream.close();
#ifdef INSILICO_MPI_ENABLE
  mpi::configuration::finalize();
#endif
}

} } // namespace insilico::configuration

#endif
