#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

//#define c_type   int
//#define mpi_type MPI_INT

#define c_type   long
#define mpi_type MPI_LONG

//#define c_type   float
//#define mpi_type MPI_FLOAT

//#define c_type   double
//#define mpi_type MPI_DOUBLE


int main(int argc, char* argv[])
{
  c_type input = 7, output=0;
  void *buf;
  MPI_Aint size, pos;
  MPI_Init(0,0);

  MPI_Pack_external_size("external32",1,mpi_type,&size);
  buf = malloc(size);

  pos = 0;
  MPI_Pack_external("external32",
                    &input, 1,
                    mpi_type,
                    buf, size, &pos);
  pos = 0;
  MPI_Unpack_external("external32", 
                      buf, size, &pos,
                      &output, 1, mpi_type);
  free(buf);
  assert(input==output);

  MPI_Finalize();
}
