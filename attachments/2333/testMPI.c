#include <stdio.h>
#include "mpi.h"

int main(int argc,char *argv[])
{
  MPI_Init(&argc,&argv);

  int rank  = 0;
  int nproc = 1;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&nproc);

  int coresPerNode = 16;
  int nodeID = rank/coresPerNode;
  int nodeRank = rank%coresPerNode;
  int cartDims[3] = {0,0,0};
  int cartPeriodic[3] = {0,0,0};
  int color = 1;
  if(nodeRank)
    color = 0;

  MPI_Barrier(MPI_COMM_WORLD);
  double time0 = MPI_Wtime();
  MPI_Dims_create(nproc,3,cartDims);
  MPI_Barrier(MPI_COMM_WORLD);
  double time1 = MPI_Wtime();
  double timeDims = time1 - time0;

  MPI_Comm cartesianCommunicator;
  MPI_Cart_create(MPI_COMM_WORLD,3,cartDims,cartPeriodic,1,&cartesianCommunicator);
  MPI_Barrier(MPI_COMM_WORLD);
  time0 = MPI_Wtime();
  double timeCart = time0 - time1;

  MPI_Comm splitCommunicator;
  MPI_Comm_split(MPI_COMM_WORLD,color,rank,&splitCommunicator);
  MPI_Barrier(MPI_COMM_WORLD);
  double timeSplit = MPI_Wtime() - time0;

  if(rank == 0){
    printf("# NumThreads  DimsCreate  CartCreate  CommSplit\n");
    printf("%d\t%f\t%f\t%f\n",nproc,timeDims,timeCart,timeSplit);
  }

  MPI_Finalize();
  return(0);
}
