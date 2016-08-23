#include <mpi.h>
#include <stdio.h>

int main(void) {
  MPI_Request request;
  MPI_Status status;
  int flag;
  MPI_Init(0,0);
  MPI_Issend(NULL,0,MPI_BYTE,0,123,MPI_COMM_SELF,&request);
  MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_SELF,&status);
  MPI_Cancel(&request);
  MPI_Request_get_status(request,&flag,&status);
  MPI_Test_cancelled(&status,&flag);
  if (flag) {
    printf("cancelled: free request\n");
    MPI_Request_free(&request);
  } else {
    printf("not cancelled: recv & wait\n");
    MPI_Recv(NULL,0,MPI_BYTE,0,123,MPI_COMM_SELF,&status);
    MPI_Wait(&request,&status);
  }
  MPI_Finalize();
  return 0;
}
