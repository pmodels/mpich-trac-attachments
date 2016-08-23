#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  MPI_Request barrier;
  int rank,i,done;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Ibarrier(MPI_COMM_WORLD,&barrier);
  for (i=0,done=0; !done; i++) {
    usleep(1000);
    printf("[%d] MPI_Test: %d\n",rank,i);
    if (1) {
      MPI_Test(&barrier,&done,MPI_STATUS_IGNORE);
    } else {
      MPI_Wait(&barrier,MPI_STATUS_IGNORE);
      done = 1;
    }
  }
  MPI_Finalize();
  return 0;
}
