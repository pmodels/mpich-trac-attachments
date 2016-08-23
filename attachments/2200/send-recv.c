#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, i, j, flag;
    int one = 1, two = 2;
    double s, e;
    MPI_Win win;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Win_create(&one, sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_lock_all(0, win);

    MPI_Barrier(MPI_COMM_WORLD);

    for (i = 0; i < 100; i++) {
        if (rank >= 2) {
            s = MPI_Wtime();
            for (j = 0; j < 1000; j++) {
                MPI_Accumulate(&two, 1, MPI_INT, 1, 0, 1, MPI_INT, MPI_REPLACE, win);
                MPI_Win_flush(1, win);
            }
            e = MPI_Wtime();
            if (rank == 2)
                printf("loop %d took %.3f seconds\n", i, e - s);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Win_unlock_all(win);

    MPI_Win_free(&win);

    MPI_Finalize();
    return 0;
}

