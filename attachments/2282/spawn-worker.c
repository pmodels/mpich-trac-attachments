#include <mpi.h>
int main(int argc, char *argv[])
{
  MPI_Comm master;
  MPI_Init(&argc, &argv);
  MPI_Comm_get_parent(&master);
  MPI_Barrier(master);
  MPI_Comm_disconnect(&master);
  MPI_Finalize();
  return 0;
}
