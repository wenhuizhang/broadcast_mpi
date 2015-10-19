#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void my_bcast(int data[], int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
  int world_rank;
  MPI_Comm_rank(communicator, &world_rank);
  int world_size;
  MPI_Comm_size(communicator, &world_size);

  int i;
  if (world_rank == root) {
    // If we are the root process, send our data to everyone
    for (i = 0; i < world_size; i++) {
      if (i != world_rank) {
        MPI_Send(&data[i], count, datatype, i, 0, communicator);
      }
    }
  } else {
    // If we are a receiver process, receive the data from the root
    MPI_Recv(&data[i], count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
  }
}

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int *elements = malloc(sizeof(int)*world_size);
  int i;
  // inizialize
  for (i = 0; i < world_size; ++i)
         elements[i] = i;

  for (i = world_size - 1; i > 0; --i) {
        // generate random index
        int w = rand()%i;
        // swap items
        int t = elements[i];
        elements[i] = elements[w];
        elements[w] = t;
   }

  if (world_rank == 0) {
    my_bcast(elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    my_bcast(elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received data %d from 0  process\n", world_rank, elements[i]);
  }

  MPI_Finalize();
}
