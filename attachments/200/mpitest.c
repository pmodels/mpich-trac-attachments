#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int MPInode, MPIsize, MPInext;

#define MPIERROR(rc) MPIerror(rc, __LINE__);
int MPIerror(int rc, int line)
{
  fprintf(stderr, "MPIERROR() called on line=%d\n", line);
  char error_string[BUFSIZ];
  int length_of_error_string, error_class;
  MPI_Error_class(rc, &error_class);
  MPI_Error_string(error_class, error_string, &length_of_error_string);
  fprintf(stderr, "%3d: %s\n", MPInode, error_string);
  MPI_Error_string(rc, error_string, &length_of_error_string);
  fprintf(stderr, "%3d: %s\n", MPInode, error_string);
  return(MPI_Abort(MPI_COMM_WORLD, -1));
}

#define NREQ 10 // number of requests to keep track off
#define NSEND 1000000 // number of messages to send

void * recvThread(void* arg) {
  double buf[NREQ];
  MPI_Request req[NREQ];
  int ireq = 0;
  for(ireq = 0; ireq<NREQ; ireq++)
  {
    req[ireq] = MPI_REQUEST_NULL;
    int rc = MPI_Irecv(&buf[ireq],1,MPI_DOUBLE,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&req[ireq]);
    MPI_SUCCESS == rc || MPIERROR(rc);
  }
  do {
    int rc = MPI_Waitany(NREQ, req, &ireq, MPI_STATUS_IGNORE);
    MPI_SUCCESS == rc || MPIERROR(rc);
    double sum = 0;
    for(int i=0; i<100000; i++) sum += buf[ireq]-drand48(); // do something
    rc = MPI_Irecv(&buf[ireq],1,MPI_DOUBLE,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&req[ireq]);
    MPI_SUCCESS == rc || MPIERROR(rc);
  } while(buf[ireq] >= 0);
  int rc = MPI_Waitall(NREQ, req, MPI_STATUSES_IGNORE);
  MPI_SUCCESS == rc || MPIERROR(rc);

  printf("node %d done receiving\n", MPInode);
  return(NULL);
}

void * sendThread(void* arg) {
  double buf[NREQ];
  MPI_Request req[NREQ];
  int ireq = 0;
  for(ireq = 0; ireq<NREQ; ireq++) {
    req[ireq] = MPI_REQUEST_NULL;
  }
  for(int i=0; i<NSEND; i++) {
    int rc = MPI_Waitany(NREQ, req, &ireq, MPI_STATUS_IGNORE);
    MPI_SUCCESS == rc || MPIERROR(rc);
    buf[ireq] = drand48();
    rc = MPI_Issend(&buf[ireq],1,MPI_DOUBLE,MPInext,1,MPI_COMM_WORLD,&req[ireq]);
    MPI_SUCCESS == rc || MPIERROR(rc);
  }
  int rc = MPI_Wait(&req[ireq], MPI_STATUS_IGNORE);
  MPI_SUCCESS == rc || MPIERROR(rc);
  buf[ireq] = -1.0;
  rc = MPI_Ssend(&buf[ireq],1,MPI_DOUBLE,MPInext,1,MPI_COMM_WORLD);
  MPI_SUCCESS == rc || MPIERROR(rc);

  printf("node %d done sending\n", MPInode);
  return(NULL);
}

int main(int argc, char *argv[])
{
  int MPIrequired = MPI_THREAD_MULTIPLE;
  int MPIprovided;
  int rc = MPI_Init_thread(&argc, &argv, MPIrequired, &MPIprovided);
  if (rc != MPI_SUCCESS)
  {
    printf ("Error starting MPI program, Terminating.\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
  }
  if(MPIprovided != MPI_THREAD_MULTIPLE) {
    printf("MPI_THREAD_MULTIPLE not provided in initalization\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
  }

  MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

  MPI_Comm_rank(MPI_COMM_WORLD, &MPInode);
  MPI_Comm_size(MPI_COMM_WORLD, &MPIsize);

  MPInext = (MPInode+1) % MPIsize;

  pthread_t sendThreadID,recvThreadID; /* thread ID numbers */
  pthread_attr_t threadattr;
  pthread_attr_init(&threadattr);
  pthread_attr_setdetachstate(&threadattr, PTHREAD_CREATE_JOINABLE);

  // call threads
  if (pthread_create(&sendThreadID,&threadattr,sendThread,NULL)) {
    printf("error in creating sendThread\n");
    return(-1);
  }
  if (pthread_create(&recvThreadID,&threadattr,recvThread,NULL)) {
    printf("error in creating recvThread\n");
    return(-1);
  }

  // join threads
  pthread_join(sendThreadID,NULL);
  pthread_join(recvThreadID,NULL);

  printf("%d: Program completed successfully!\n", MPInode);

  MPI_Finalize ();
}