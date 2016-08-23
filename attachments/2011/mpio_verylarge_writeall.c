/*
 * This program must be ran with one MPI task and from a lustre directory.
 * It has been tested with mpich commit 3457b353765078d5a37be8b043811fc7e547bcd6
 * and the three patches attached at http://trac.mpich.org/projects/mpich/ticket/2010
 *
 * when ran on a UNIX filesystem (/tmp = ext4 it works)
 * when ran on a lustre filesystem, it crashes.
 * 
 * Copyright (c) 2014      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 */
#include <stdio.h>
#include <mpi.h>

#define N (512*1024*1024)
#define FILENAME "mpioarray.bin"

double array[N];

int main (int argc, char *argv[]) {
    int size;
    int i;
#ifndef STATUS_IGNORE
    MPI_Status status;
#endif
    MPI_File fd;
    MPI_Offset file_size = N * sizeof(double);

    for (i=0; i<N; i++)
        array[i] = i;

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        fprintf(stderr, "MPI_Init failed\n");
        return 1;
    }

    if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS) {
        fprintf(stderr, "MPI_Comm_Size failed\n");
        MPI_Abort(MPI_COMM_WORLD, 2);
    }
    if (size != 1) {
        fprintf(stderr, "This program must be ran with one MPI task\n");
        MPI_Abort(MPI_COMM_WORLD, 3);
    }

    if (MPI_File_open(MPI_COMM_WORLD, FILENAME, \
            MPI_MODE_WRONLY|MPI_MODE_CREATE, \
            MPI_INFO_NULL, &fd) != MPI_SUCCESS) {
        fprintf(stderr, "MPI_File_open failed\n");
        MPI_Abort(MPI_COMM_WORLD, 4);
    }

    if (MPI_File_set_size(fd, file_size) != MPI_SUCCESS) {
        fprintf(stderr, "MPI_File_set_size failed\n");
        MPI_Abort(MPI_COMM_WORLD, 5);
    }

#ifndef STATUS_IGNORE
    if (MPI_File_write_all(fd, array, N, MPI_DOUBLE, &status) != MPI_SUCCESS)
#else
    if (MPI_File_write_all(fd, array, N, MPI_DOUBLE, MPI_STATUS_IGNORE) != MPI_SUCCESS)
#endif
    {
        fprintf(stderr, "MPI_File_write_all failed\n");
        MPI_Abort(MPI_COMM_WORLD, 6);
    }

    if (MPI_File_close(&fd) != MPI_SUCCESS) {
        fprintf(stderr, "MPI_File_close failed\n");
        MPI_Abort(MPI_COMM_WORLD, 7);
    }

    MPI_Finalize();

    return 0;
}
