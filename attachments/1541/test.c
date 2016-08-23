#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
  int me, *universe_sizep, flag;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &universe_sizep, &flag);

  if (!flag)
    printf("%d: This MPI does not support UNIVERSE_SIZE.\n", me);
  else
    printf("%d: UNIVERSE_SIZE = %d\n", me, *universe_sizep);

  MPI_Finalize();
  return 0;
}


