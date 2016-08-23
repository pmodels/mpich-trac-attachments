#include <mpi.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


int query_fn(void *extra_state, MPI_Status *status)
{
  status->MPI_SOURCE = MPI_UNDEFINED;
  status->MPI_TAG    = MPI_UNDEFINED;
  MPI_Status_set_cancelled(status, 0);
  MPI_Status_set_elements(status, MPI_BYTE, 0);
  return 0;
}
int free_fn(void *extra_state) { return 0; }
int cancel_fn(void *extra_state, int complete) { return 0; }


MPI_Request grequest;

void *do_work(void *arg)
{
  MPI_Request *req = (MPI_Request *)arg;
  puts("Starting work in thread ...");
  sleep(3);
  puts("Work in thread done !!!");
  MPI_Grequest_complete(*req);
  return NULL;
}

int main(int argc, char *argv[])
{
  int provided;
  MPI_Request request;
  pthread_t thread; 
  int flag;
  MPI_Status status;

  MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE, &provided);

  puts("Hello ...");

  MPI_Grequest_start(query_fn, free_fn, cancel_fn, NULL, &request);
  grequest = request; /* copy the handle */
  pthread_create(&thread, NULL, do_work, &grequest);

  puts("Waiting ...");
  MPI_Wait(&request, &status);
  //MPI_Waitall(1, &request, &status);

  puts("Goodbye !!!");
  MPI_Finalize();

}