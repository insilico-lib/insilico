#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>

#define MASTER  0
#define TIMES   1000

using namespace std;

typedef vector<double> state_type;

void groker(int *argc, char ***argv) {
  int rank, size, i, j;
  
  MPI_Init(argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double total_size = 1024;
  double proc_data_size = total_size / size;
  state_type state(total_size), proc_spec_state(total_size), temp(proc_data_size);
  
  if(rank == MASTER) {
    // Create or read the initial state of the system
    for(vector<double>::size_type iter=0;iter<state.size();++iter) {
      state[iter] = iter;
    }
  }
  
  for(i=0; i < TIMES; ++i) {
    if(rank == MASTER) {
      for(j = 1; j < size; ++j) {
        // Send the whole state to all Slaves
        MPI_Send(&state[j*proc_data_size], proc_data_size, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
        // Receive the whole state back from Slaves
        MPI_Recv(&state[j*proc_data_size], proc_data_size, MPI_DOUBLE, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);        
      }
      // Do your part of work Master
      for(int iter = 0; iter < proc_data_size; ++iter) {
        state[iter] += 1;
      }
    }
    else {
      // Receive the whole state from Master
      MPI_Recv(&temp[0], proc_data_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // Work on the respective part of the vector
      for(int iter = 0; iter < proc_data_size; ++iter) {
        temp[iter] += 1;
      }      
      // Send back the whole state to Master
      MPI_Send(&temp[0], proc_data_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);      
    }  
  }

  // Show end result
  if(rank == MASTER){
    for(vector<double>::size_type iter=0;iter<state.size();++iter) {
      cout<<state.at(iter)<<"\t";
    }
  }
  
  MPI_Finalize();  
}

int main(int argc, char *argv[]) {
  groker(&argc, &argv);
  return 0;
}
