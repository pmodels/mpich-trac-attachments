/* %I%%Z% %W% %G% %U% %P% */ 
/*Restricted Rights Notice:  Use, reproduction or disclosure is subject to
restrictions set forth in Subcontract #B331593
*/
#include "mpi.h"
#include "mpi_io.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 1000
#define DATATYPE MPI_DOUBLE
#define PI 3.141592654

/* Each process writes its rank to the same file and rank 0 reads them back. */

int main(int argc, char **argv)
{
  int i, numprocs, bucket[SIZE], rootbucket[SIZE];
  char tmp_path[256], filename[256];
  double buf[SIZE];
  MPI_File fh;
  MPI_Status status;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Errhandler_set( MPI_COMM_WORLD, MPE_ERRORS_WARN );
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  
  msg[0] = '\0';
  test_info(&my_rank, 0, 0, msg, F_BGN_MSG);

  /* each process opens a separate file called filename.'mymy_rank' */
  get_path(my_rank, argv, tmp_path);
  sprintf(filename, "%s/test_file", tmp_path);
  if(my_rank == 0) MPI_File_delete(filename, MPI_INFO_NULL);
  
  for(i = 0; i < SIZE; i++) buf[i] = i * PI;
  
  ierror = MPI_File_open(MPI_COMM_SELF, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, 
                         MPI_INFO_NULL, &fh);
  CHK_IO_ERR;
  ierror = MPI_File_set_view(fh, 0, DATATYPE, DATATYPE, "native", MPI_INFO_NULL);
  CHK_IO_ERR;
  ierror = MPI_File_write_shared(fh, buf, SIZE, DATATYPE, &status);
  CHK_IO_ERR;
  ierror = MPI_File_close(&fh);
  CHK_IO_ERR;
  MPI_Barrier(MPI_COMM_WORLD);
  
  /* ----------------------------------- */
  printf("PE%d MPI_File_write_shared done\n", my_rank);
  /* ----------------------------------- */
  
  for(i = 0; i < SIZE; i++) {
    buf[i] = 0;    bucket[i] = 0;    rootbucket[i] = 0;
  }
  
  ierror = MPI_File_open(MPI_COMM_SELF, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  CHK_IO_ERR;
  ierror = MPI_File_set_view(fh, 0, DATATYPE, DATATYPE, "native", MPI_INFO_NULL);
  CHK_IO_ERR;
  ierror = MPI_File_read_shared(fh, buf, SIZE, DATATYPE, &status);
  CHK_IO_ERR;
  ierror = MPI_File_close(&fh);
  CHK_IO_ERR;
  
  printf("PE%d MPI_File_read_shared done\n", my_rank);

  /*  Local info collection first */
  for(i=0; i < SIZE; i++) bucket[(int)(buf[i] / PI + 0.5)]++;
  
  /* Then collect info from all the PEs */
  MPI_Reduce(bucket, rootbucket, SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  
  if(my_rank == 0) {
    for(i=0; i < SIZE; i++) {
      /* printf("%d:%d ", i, rootbucket[i]); */
      if(rootbucket[i] != numprocs) {
        sprintf(msg,"failed to collect right info\n");
        test_info(&my_rank, 0, 0, msg, F_ERR_MSG);
        MPI_Abort(MPI_COMM_WORLD, -1);
      }
    }
  }
  
  printf("PE%d:Test OK\n", my_rank);
  
  test_info(&my_rank, 0, 0, msg, F_END_MSG);
  MPI_Finalize();
}
