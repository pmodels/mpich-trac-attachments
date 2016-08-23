
#include <stdio.h>

#include <mpi.h>


int main(int argc, char *argv[])
{
  int sendbuf, recvbuf;
  int sendcounts[1], recvcounts[1];
  int sdispls[1], rdispls[1];
  MPI_Datatype sendtypes[1], recvtypes[1];


  MPI_Init(&argc, &argv);

  sendcounts[0] = 1;
  sdispls[0] = 0;
  MPI_Type_contiguous(0, MPI_INT, &sendtypes[0]);
  MPI_Type_commit(&sendtypes[0]);

  recvcounts[0] = 0;
  rdispls[0] = 0;
  recvtypes[0] = MPI_INT;

  MPI_Alltoallw(&sendbuf, sendcounts, sdispls, sendtypes, &recvbuf, recvcounts, rdispls, recvtypes, MPI_COMM_WORLD);
/*  MPI_Alltoallv(&sendbuf, sendcounts, sdispls, sendtypes[0], &recvbuf, recvcounts, rdispls, recvtypes[0], MPI_COMM_WORLD);*/

  MPI_Type_free(&sendtypes[0]);

  MPI_Finalize();

  return 0;
}
