#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int ierr;
  MPI_File fh;
  MPI_Init(&argc, &argv);

  MPI_File_open(MPI_COMM_WORLD,"/tmp/datafile",
                MPI_MODE_CREATE|MPI_MODE_RDWR|MPI_MODE_DELETE_ON_CLOSE,
                MPI_INFO_NULL,&fh);

  MPI_File_set_errhandler(fh,MPI_ERRORS_RETURN);
  ierr = MPI_File_call_errhandler(fh,MPI_ERR_FILE);
  printf("ierr: %d, expected: %d\n", ierr, (int)MPI_SUCCESS);

  MPI_File_set_errhandler(fh,MPI_ERRORS_ARE_FATAL);
  MPI_File_call_errhandler(fh,MPI_ERR_FILE); /* should abort */
  MPI_File_close(&fh);

  MPI_Finalize();
  return 0;
}
