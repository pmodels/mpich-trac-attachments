#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    MPI_Comm_dup(MPI_COMM_WORLD, NULL);

    MPI_Finalize();
    return 0;
}

