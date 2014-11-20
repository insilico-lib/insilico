#include <iostream>
#include <vector>
#include <cmath>
#include <mpi.h>

using namespace std;

typedef vector<double> state_type;

int main(int argc, char *argv[]) {
  int rank, size, i, j;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  double total_size = 1024;
  double proc_data_size = total_size / size;
  state_type state(total_size), temp(proc_data_size);

  for(i=0;i<proc_data_size;++i) {
    temp[i]=rank+1;
  }
  
  if(rank != 0) { // Slaves
    MPI_Send(&temp[0], proc_data_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  else { // Master
    state_type proc_temp(proc_data_size);
    for(j=0;j<proc_data_size;++j) {
      state[rank*proc_data_size+j] = temp[j];
    }
    for(i=1;i<size;++i) {
      MPI_Recv(&proc_temp[0], proc_data_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for(j=0;j<proc_data_size;++j) {
        state[i*proc_data_size+j] = proc_temp[j];
      }
    }
    for(vector<double>::size_type iter=0;iter<state.size();++iter) {
      cout<<state.at(iter)<<"\t";
    }
  }
  
  MPI_Finalize();
  return 0;
}
