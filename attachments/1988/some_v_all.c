#include <stdlib.h>
#include <stdio.h>

#include "mpi.h"

int main(int argc, char **argv) {
    int rank;
    MPI_Request *reqs;
    int num_reqs = 1024;
    int outcount = 0;
    int *array_of_indices;
    int num_left;
    double start, stop;
    MPI_Status *statuses;
    int i;

    if (argc == 2) {
        num_reqs = atoi(argv[1]);
    }
    num_left = num_reqs;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    reqs = (MPI_Request *) malloc(sizeof(MPI_Request) * num_reqs);
    array_of_indices = (int *) malloc(sizeof(int) * num_reqs);
    statuses = (MPI_Status *) malloc(sizeof(MPI_Status) * num_reqs);

    MPI_Barrier(MPI_COMM_WORLD);

    start = MPI_Wtime();

    for (i = 0; i < num_reqs; i+=2) {
        MPI_Isend(NULL, 0, MPI_INT, (rank+1)%2, 0, MPI_COMM_WORLD, &reqs[i]);
        MPI_Irecv(NULL, 0, MPI_INT, (rank+1)%2, 0, MPI_COMM_WORLD, &reqs[i+1]);
    }

    while (num_left > 0) {
        MPI_Waitsome(num_reqs, reqs, &outcount, array_of_indices, statuses);
        num_left -= outcount;
    }

    stop = MPI_Wtime();

    fprintf(stdout, "[%d] WAITSOME: %f\n", rank, stop - start);

    start = MPI_Wtime();

    for (i = 0; i < num_reqs; i+=2) {
        MPI_Isend(NULL, 0, MPI_INT, (rank+1)%2, 0, MPI_COMM_WORLD, &reqs[i]);
        MPI_Irecv(NULL, 0, MPI_INT, (rank+1)%2, 0, MPI_COMM_WORLD, &reqs[i+1]);
    }

    MPI_Waitall(num_reqs, reqs, statuses);

    stop = MPI_Wtime();

    fprintf(stdout, "[%d] WAITALL: %f\n", rank, stop - start);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

    free(reqs);
    free(array_of_indices);

    return 0;
}
