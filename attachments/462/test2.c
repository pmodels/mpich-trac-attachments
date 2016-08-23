/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */
 #include "mpi.h"
 #include "stdio.h"

/* tests a series of puts, gets, and accumulate on 2 processes using fence
 */
/* Same as test1.c but uses derived datatypes to receive data */

/* Test case does not fail for 0 < SIZE < 18 , fails for SIZE >= 18 . Cud
   be hitting IOV_LIMIT == 16 ...*/
#define SIZE 18
#define COUNT 2

int main(int argc, char *argv[])
{
    int rank, nprocs, A[COUNT * SIZE], B[COUNT * SIZE], i;
    MPI_Win win;
    MPI_Datatype contig_2ints;
    int errs = 0;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (nprocs != 2) {
        printf("Run this program with 2 processes\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    if (rank == 0) {
        for (i=0; i< (COUNT * SIZE); i++)
            A[i] = B[i] = i;
    }
    else {
        for (i=0; i< (COUNT * SIZE); i++) {
            A[i] = (-3)*i;
            B[i] = (-4)*i;
        }
    }

    MPI_Type_contiguous(COUNT, MPI_INT, &contig_2ints);
    MPI_Type_commit(&contig_2ints);

    MPI_Win_create(B, COUNT * SIZE*sizeof(int), sizeof(int), MPI_INFO_NULL,
                   MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    if (rank == 0) {
        for (i=0; i<SIZE*COUNT; i+=COUNT)
            MPI_Put(A+i, COUNT, MPI_INT, 1, i, 1, contig_2ints, win);
/*             MPI_Put(A+i, COUNT, MPI_INT, 1, i, COUNT, MPI_INT, win); */
    }
    else {
        for (i=0; i<SIZE*COUNT; i+=COUNT)
            MPI_Get(A+i, COUNT, MPI_INT, 0, i, 1, contig_2ints, win);
/*             MPI_Get(A+i, COUNT, MPI_INT, 0, i, COUNT, MPI_INT, win); */
    }
    MPI_Win_fence(0, win);

    if (rank == 1) {
        for (i=0; i<COUNT * SIZE; i++) {
            if (A[i] != B[i]) {
                printf("Put/Get Error: A[%d]=%d, B[%d]=%d\n", i, A[i], i, B[i]);
                errs++;
            }
        }
    }

    MPI_Win_free(&win);
    MPI_Type_free(&contig_2ints);
    MPI_Finalize();
    return 0;
}
