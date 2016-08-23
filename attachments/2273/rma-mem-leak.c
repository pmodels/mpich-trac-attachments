/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2015 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 131072
#define OPS_NUM 1

int main(int argc, char *argv[]) {
    int rank, nproc, i, j;
    MPI_Win win;
    MPI_Info info;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Win_create(MPI_BOTTOM, 0, sizeof(char), MPI_INFO_NULL, MPI_COMM_SELF, &win);

    MPI_Win_get_info(win, &info);

    MPI_Win_set_info(win, info);

    MPI_Info_free(&info);

    MPI_Win_free(&win);

    MPI_Finalize();

    return 0;
}

