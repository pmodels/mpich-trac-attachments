
#include <sys/types.h>
#include <unistd.h>
#include "mpi.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>


void sigUsr1handle (int sig)
{

  printf ("--- Signal caught\n");
  MPI_Abort (MPI_COMM_WORLD, 1);
}



int main (int argc, char* argv[])
{

  MPI_Init (&argc, &argv);
  MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
  
  char portname[MPI_MAX_PORT_NAME];
  if (MPI_Lookup_name("RandomSession", MPI_INFO_NULL, portname) == MPI_SUCCESS) {
    printf("Successfully got portname : %s\n", portname);
    MPI_Comm newComm ;
    MPI_Comm_connect (portname, MPI_INFO_NULL, 0, MPI_COMM_SELF, &newComm);
    printf ("Successfully connected to server\n");
    MPI_Errhandler_set(newComm, MPI_ERRORS_RETURN);
    int val = 99 ;
    MPI_Request req1, req ;
    MPI_Isend(&val, 1, MPI_INT, 0, 99, newComm, &req1);
    MPI_Wait(&req1, MPI_STATUS_IGNORE);


    MPI_Irecv(&val, 1, MPI_INT, 0, 99, newComm, &req);
    sleep(7);
    printf ("Wake up\n");
    MPI_Cancel(&req);
    MPI_Status stat;
    MPI_Wait(&req, &stat);
    int flag = 0 ;
    MPI_Test_cancelled(&stat, &flag);
    if (flag) {
      printf ("Test cancelled\n");
    }
    MPI_Comm_free(&newComm);
    //printf ("Disconenct\n");
    //MPI_Comm_disconnect(&newComm);
  }

  printf ("Finalize\n");
  MPI_Finalize();
}
