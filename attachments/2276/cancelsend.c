#include <mpi.h>
#include <stdio.h>
#include <assert.h>

int main(void) {
  int rank, flag;
  int a, b;
  MPI_Request requests[2];
  MPI_Status statuses[2];

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    a = 10;
    b = 20;
    MPI_Isend(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &requests[0]);
    MPI_Isend(&b, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &requests[1]);
    MPI_Cancel(&requests[1]);
    MPI_Wait(&requests[1], &statuses[1]);
    MPI_Test_cancelled(&statuses[1], &flag);
    assert(flag);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Wait(&requests[0], MPI_STATUS_IGNORE);
  } else if (rank == 1) {
    a = -1;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (a == 20)
      printf("fail! got the wrong data!\n");
  }

  MPI_Finalize();
  return 0;
}
