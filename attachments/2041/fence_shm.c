#include <stdio.h>
#include "mpi.h"

#define ELEM_PER_PROC 1

int main(int argc, char *argv[]) {
    int rank, nprocs;
    MPI_Win win;
    int *my_base;
    int origin_data = 1;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    /* run with two processes on the same node */

    MPI_Win_allocate_shared(sizeof(int)*ELEM_PER_PROC, sizeof(int), MPI_INFO_NULL,
                            MPI_COMM_WORLD, &my_base, &win);

    if (rank == 1) {
        /* Initialize window buffer */
        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 1, 0, win);
        *my_base = 0;
        MPI_Barrier(MPI_COMM_WORLD);
        printf("rank=%d my_base[0]=%d\n", rank, *my_base);
        MPI_Win_unlock(1, win);
    }

    MPI_Win_fence(0, win);

    if (rank == 0) {
        MPI_Put(&origin_data, 1, MPI_INT, 1, 0, 1, MPI_INT, win);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Win_fence(0, win);

    MPI_Win_free(&win);

    MPI_Finalize();
}
