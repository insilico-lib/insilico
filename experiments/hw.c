#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char *argv[])
{
  int rank, size;

  int i,j;
  long int sum=0;
  
  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;

  MPI_Init (&argc, &argv);

  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  for(i=0;i<100;i++)
    for(j=0;j<rank;j++)
      sum+=pow(i,j);

  MPI_Get_processor_name(processor_name, &name_len);

  // Print off a hello world message
  printf("Hi! (%s, rank %d / %d)\n", processor_name, rank, size);

  MPI_Finalize();
  return 0;
}

