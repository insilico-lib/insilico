/*
 neuron/helper/spike_list.hpp - Neuron spike activity storage
                                (past spikes list w.r.t. time)

 Copyright (C) 2015 Pranav Kulkarni, Collins Assisi Lab,
                    IISER, Pune <pranavcode@gmail.com>
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

#ifndef INCLUDED_INSILICO_NEURON_HELPER_SPIKE_LIST_HPP
#define INCLUDED_INSILICO_NEURON_HELPER_SPIKE_LIST_HPP

#include <vector>

namespace insilico { namespace engine {

std::vector<std::vector<double>> spike_list;

} } // namespace insilico::engine
 
#endif
