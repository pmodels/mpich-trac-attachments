#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"
#include "mpixft.h"

int main(int argc, char **argv) {
    int rank, size;
    int buf1 = -1;
    MPI_Request reqs[2];
    MPI_Status stats[2];
    int rc;
    MPI_Comm world;

    MPI_Init(&argc, &argv);

    MPI_Comm_dup(MPI_COMM_WORLD, &world);

    MPI_Comm_set_errhandler(world, MPI_ERRORS_RETURN);

    MPI_Comm_rank(world, &rank);
    MPI_Comm_size(world, &size);

    if (size < 3) {
        fprintf(stderr, "Must have at least 3 processes\n");
        exit(1);
    }

    rc = MPI_Barrier(world);

    fprintf(stderr, "[%d] BARRIER 1: %d\n", rank, rc);

    if (rank == 1) {
        exit(1);
    }

    rc = MPI_Barrier(world);

    fprintf(stderr, "[%d] BARRIER 2: %d\n", rank, rc);

    if (rank == 0) {
        rc = MPI_Irecv(&buf1, 1, MPI_INT, 2, 0, world, &reqs[0]);
        if (rc != MPI_SUCCESS)
            fprintf(stderr, "[%d] IRECV FAILED TO START: %d\n", rank, rc);
        MPI_Isend(&rank, 1, MPI_INT, 2, 0, world, &reqs[1]);
        if (rc != MPI_SUCCESS)
            fprintf(stderr, "[%d] ISEND FAILED TO START: %d\n", rank, rc);
        rc = MPI_Waitall(2, reqs, stats);
        fprintf(stdout, "[%d] WAITALL: %d\n", rank, rc);

        if (2 != buf1)
            fprintf(stderr, "[%d] (%d) %d -- %d\n", rank, buf1, stats[0].MPI_ERROR, stats[1].MPI_ERROR);
        else
            fprintf(stderr, "[%d] Correct answer\n", rank);
    } else if (rank == 2) {
        rc = MPI_Irecv(&buf1, 1, MPI_INT, 0, 0, world, &reqs[0]);
        if (rc != MPI_SUCCESS)
            fprintf(stderr, "[%d] IRECV FAILED TO START: %d\n", rank, rc);
        rc = MPI_Isend(&rank, 1, MPI_INT, 0, 0, world, &reqs[1]);
        if (rc != MPI_SUCCESS)
            fprintf(stderr, "[%d] ISEND FAILED TO START: %d\n", rank, rc);
        rc = MPI_Waitall(2, reqs, stats);
        fprintf(stdout, "[%d] WAITALL: %d\n", rank, rc);

        if (0 != buf1)
            fprintf(stderr, "[%d] (%d) %d -- %d\n", rank, buf1, stats[0].MPI_ERROR, stats[1].MPI_ERROR);
        else
            fprintf(stderr, "[%d] Correct answer\n", rank);
    }

    MPI_Finalize();
}
