#include <mpi.h>
int main(int argc, char *argv[])
{
  char *args[] = {"a", "b", "c", (char*)0};
  MPI_Comm worker;
  MPI_Init(&argc, &argv);
  MPI_Comm_spawn("./spawn-worker",
                 args,/*MPI_ARGV_NULL,*/
                 5, MPI_INFO_NULL, 0,
                 MPI_COMM_SELF, &worker,
                 MPI_ERRCODES_IGNORE);
  MPI_Barrier(worker);
  MPI_Comm_disconnect(&worker);
  MPI_Finalize();
  return 0;
}
