#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <omp.h>
#include <mpi.h>

#define NMSGS 1000

int main(int argc, char *argv[])
{
  int provided;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  if(provided!=MPI_THREAD_MULTIPLE){
    printf("MPI_THREAD_MULTIPLE not supported\n");
    MPI_Abort(MPI_COMM_WORLD, -1);
  }

  int rank;
  volatile int rcv_count=0;
  int nthreads = omp_get_max_threads();

  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  if(rank==0){//Source
    #pragma omp parallel
    {
      char buf[NMSGS];
      MPI_Request snd_reqs[NMSGS];
      for(int i=0; i<NMSGS; i++)
        MPI_Issend(&buf[i], 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &snd_reqs[i]);

      int npend_reqs = NMSGS;
      while(npend_reqs){
        for(int i=0; i<NMSGS; i++){
          if(snd_reqs[i]!=MPI_REQUEST_NULL){
            int flag;
            MPI_Test(&snd_reqs[i], &flag, MPI_STATUS_IGNORE);
            if(flag)
              npend_reqs--;
          }
        }
      }      
    }
  }
  else if (rank==1){ //Sink
    #pragma omp parallel
    {
      char buf;
      MPI_Request rcv_req;
 
      /* Initial Recv */
      MPI_Irecv(&buf, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &rcv_req);

      while(rcv_count < nthreads*NMSGS){
        /* Progress on the Receive side*/
        int flag;
        MPI_Test(&rcv_req, &flag, MPI_STATUS_IGNORE);
        if(flag){
          /*Consume message*/
          #pragma omp critical
          rcv_count ++;
          MPI_Irecv(&buf, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &rcv_req);
        }
      }
 
      MPI_Status st;
      MPI_Cancel(&rcv_req);
      MPI_Wait(&rcv_req, &st);
      int cancelled;
      MPI_Test_cancelled(&st, &cancelled);
      assert(cancelled);
    }
  }

  int tot_nmsgs;
  MPI_Allreduce(&rcv_count, &tot_nmsgs, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);   

  if (rank == 1){
    if(tot_nmsgs == NMSGS * nthreads)
      printf("SUCCESS: correctly received %d messages\n", tot_nmsgs);
    else
      printf("FAILURE: received %d messages instead of %d\n", tot_nmsgs, NMSGS * nthreads);
  }

  MPI_Finalize();
  return 0;
}
