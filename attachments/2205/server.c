
#include <sys/types.h>
#include <unistd.h>
#include "mpi.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>


int main (int argc, char* argv[])
{
  int provided ;
  MPI_Init_thread (&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  if (provided != MPI_THREAD_MULTIPLE) {
    printf (  " Thread support not there \n");
  }

  MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
  char portname[MPI_MAX_PORT_NAME];
  MPI_Open_port(MPI_INFO_NULL, portname);
  MPI_Publish_name("RandomSession", MPI_INFO_NULL, portname);
  
  printf ("Successfully published portname : %s\n", portname);
  MPI_Comm newComm ;
  MPI_Comm_accept (portname, MPI_INFO_NULL, 0, MPI_COMM_SELF, &newComm);
  printf ("Successfully connected to client\n");
  MPI_Errhandler_set(newComm, MPI_ERRORS_RETURN);
  int val = 0 ;
  int errcode = MPI_Recv(&val, 1, MPI_INT, 0, 99, newComm, MPI_STATUS_IGNORE);

  printf ("Recevied value %d\n", val);
  
  assert(0);
  printf ("Disconnect\n");
  MPI_Comm_disconnect(&newComm);
  //MPI_Comm_free(&newComm);

  MPI_Unpublish_name("RandomSession", MPI_INFO_NULL, portname);
  MPI_Close_port(portname);
  printf ("Finalize\n");
  MPI_Finalize();
  
  
  pthread_exit(NULL);
  return 0;

}
