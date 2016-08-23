#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

MPI_Win MPI_WIN;
char   *MPI_BUF;
char   *LOC_BUF;

#define MB 1024*1024

void mpi_win_init(int NX, int NY, int myrank, int nprocs) {

     int      ierr=-1,bsize,i;
     MPI_Aint win_size_buf,win_size_loc;

     MPI_Type_size(MPI_DOUBLE,&bsize);
     win_size_buf=(MPI_Aint) NX*NY*bsize;
     win_size_loc=win_size_buf/nprocs;

     if(myrank==0) {
        printf("Allocating memory: win_buf=%ld (Mb), loc_buf=%ld (Mb)\n",win_size_buf/(MB),win_size_loc/(MB));
     }

     MPI_Alloc_mem(win_size_buf,MPI_INFO_NULL,&MPI_BUF);
     MPI_Alloc_mem(win_size_loc,MPI_INFO_NULL,&LOC_BUF);

     if(MPI_Win_create(MPI_BUF,win_size_buf,bsize,MPI_INFO_NULL,MPI_COMM_WORLD,&MPI_WIN)!=MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD,ierr);
     }

return;
}

void mpi_win_free(void) {

     MPI_Win_free(&MPI_WIN);
     MPI_Free_mem(MPI_BUF);
     MPI_Free_mem(LOC_BUF);


return;
}

int main(int argc, char *argv[]) {

   int rank,nprocs,i;
   int NX,NY,nx,ny,str_loc,str_buf;
   MPI_Datatype otype,ttype;

   if(argc<3) {
      printf("Usage: two dimensions (two integers) must be specified\n");
      exit(-1);
   }

   NX=strtol(argv[1],NULL,10);
   NY=strtol(argv[2],NULL,10);

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);

   if(NX<=0||NY<=0) {
      printf("Wrong dimensions are specified: NX=%d, NY=%d\n",NY,NY);
      exit(-1);
   } else if (NX%nprocs||NY%nprocs) {
      printf("The first argument should be divisible by nProcs (%d) !\n", nprocs);
      exit(-1);
   }

   mpi_win_init(NX,NY,rank,nprocs);

   nx=NX/nprocs;
   ny=NY/nprocs;

   str_loc=NX;
   str_buf=NX*nprocs;

   MPI_Type_vector(ny,nx,str_loc,MPI_DOUBLE,&otype);MPI_Type_commit(&otype);
   MPI_Type_vector(ny,nx,str_buf,MPI_DOUBLE,&ttype);MPI_Type_commit(&ttype);


   for(i=0;i<nprocs;i++) {
       MPI_Win_lock(MPI_LOCK_EXCLUSIVE,i,0,MPI_WIN);
       MPI_Get(&LOC_BUF[i*nx],1,otype,i,0,1,ttype,MPI_WIN);
       MPI_Win_unlock(i,MPI_WIN);
   }

   MPI_Barrier(MPI_COMM_WORLD);

   MPI_Type_free(&otype);
   MPI_Type_free(&ttype);

   mpi_win_free();

   MPI_Finalize();

}
