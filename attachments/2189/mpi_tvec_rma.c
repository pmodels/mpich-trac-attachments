#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

MPI_Win MPI_WIN;
char   *MPI_BUF;
char   *LOC_BUF;

void mpi_win_init(long int NELEM) {

     int      ierr=-1,bsize;
     MPI_Aint win_size;

     MPI_Type_size(MPI_DOUBLE,&bsize);
     win_size=(MPI_Aint) NELEM*bsize;

     MPI_Alloc_mem(win_size,MPI_INFO_NULL,&MPI_BUF);
     MPI_Alloc_mem(win_size,MPI_INFO_NULL,&LOC_BUF);

     if(MPI_Win_create(MPI_BUF,win_size,bsize,MPI_INFO_NULL,MPI_COMM_WORLD,&MPI_WIN)!=MPI_SUCCESS) {
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

   long int NELEM;
   int rank;
   int NX,NY,nx,ny,stride;
   MPI_Datatype otype,ttype;

   if(argc<3) {
      printf("Usage: two dimensions (two integers) must be specified\n");
      exit(-1);
   }

   NX=strtol(argv[1],NULL,10);
   NY=strtol(argv[2],NULL,10);

   if(NX<=0||NY<=0) {
      printf("Wrong dimensions are specified: NX=%d, NY=%d\n",NY,NY);
      exit(-1);
   } else {
     printf("2D array [%d][%d] is going to be allocated\n",NX,NY);
   }

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);

   NELEM=NX*NY/2;

   mpi_win_init(NELEM);

   if(rank==0) {

      nx=NX/10;
      ny=NY/2;
      stride=NX;
      MPI_Type_vector(ny,nx,stride,MPI_DOUBLE,&otype);MPI_Type_commit(&otype);
      MPI_Type_vector(ny,nx,stride,MPI_DOUBLE,&ttype);MPI_Type_commit(&ttype);
      MPI_Win_lock(MPI_LOCK_EXCLUSIVE,1,0,MPI_WIN);
      MPI_Put(LOC_BUF,1,otype,1,0,1,ttype,MPI_WIN);
      MPI_Win_unlock(1,MPI_WIN);
      MPI_Type_free(&otype);
      MPI_Type_free(&ttype);
   }

   MPI_Barrier(MPI_COMM_WORLD);

   mpi_win_free();

   MPI_Finalize();

}