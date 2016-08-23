/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *
 *  (C) 2012 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "mpitest.h"
#include "win_sync.h"

int main(int argc, char *argv[])
{
    int          rank, nproc;
    int          errors = 0, all_errors = 0;
    int          buf, my_buf;
    MPI_Win      win;
    MPI_Group    world_group;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    MPI_Win_create(&buf, sizeof(int), sizeof(int),
                    MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_set_errhandler(win, MPI_ERRORS_RETURN);

    if (rank == 0) {
        /* Get rank 0 into a fence epoch */
        MPI_Win_fence(0, win);
        MPI_Get(&my_buf, 1, MPI_INT, (rank+1) % nproc, 0, 1, MPI_INT, win);

        MPI_Barrier(MPI_COMM_WORLD);

    }
    else {
        /* Wait for rank 0 to enter the fence epoch */
        MPI_Barrier(MPI_COMM_WORLD);

        /* These should all fail because the target (rank 0) is in a fence epoch */
        CHECK_ERR( MPI_Win_lock(MPI_LOCK_SHARED, 0, MPI_MODE_NOCHECK, win) );
        CHECK_ERR( MPI_Get(&my_buf, 1, MPI_INT, 0, 0, 1, MPI_INT, win) );
        CHECK_ERR( MPI_Win_unlock(0, win) );

        /* Make the matching fence start call */
        MPI_Win_fence(0, win);
    }

    /* Complete the fence epoch*/
    MPI_Win_fence(0, win);

    MPI_Win_fence(0, win);
    MPI_Win_free(&win);

    MPI_Reduce(&errors, &all_errors, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0 && all_errors == 0) printf(" No Errors\n");
    MPI_Finalize();

    return 0;
}
