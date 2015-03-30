/*
 core/configuration/mpi/error.hpp - special error exit point for MPI codes

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

#ifndef INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_MPI_ERROR_HPP
#define INCLUDED_INSILICO_INCLUDE_CORE_CONFIGURATION_MPI_ERROR_HPP

#include "mpi.h"

#include <iostream>
#include <cstdlib>

namespace insilico { namespace configuration { namespace mpi {

auto severe_error() -> void {
  std::cerr << "***...SIMULATION FAILED...***\n";
  MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
}

} } } // namespace insilico::configuration::mpi

#endif
