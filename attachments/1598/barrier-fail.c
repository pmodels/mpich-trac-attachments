#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank, size;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    MPI_Barrier(MPI_COMM_WORLD);

    printf("1\n");
    
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0 || rank == 1) {
        printf("exiting\n");
        exit(0);
    }

    printf("2\n");

    MPI_Barrier(MPI_COMM_WORLD);

    printf("3\n");

    MPI_Finalize();
    return 0;
}

