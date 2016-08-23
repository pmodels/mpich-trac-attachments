#include <mpi.h>

#include <stdio.h>
#include <stdlib.h>

int MPI_Waitall(int count, MPI_Request array_of_requests[], MPI_Status array_of_statuses[]) {
    int left = count;
    int done;
    int array_of_indices[50000];
    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    while (left > 0) {
        MPI_Waitsome(count, array_of_requests, &done, array_of_indices, array_of_statuses);
        left -= done;
    }

    return MPI_SUCCESS;
}

int main(int argc, char **argv)
{
    int rank, size;
    int next, prev;
    int tokens;
    int rounds;
    int rc, i;
    int *token_send_data, *token_recv_data;
    MPI_Request *reqs;
    MPI_Status *status;
    double start_time, curr_time, end_time;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3) {
        if (rank == 0) fprintf(stderr, "Usage: `./ring [t] [r]`\n\tt - the number of duplicate tokens\n\tr - the number of rounds\n");
        exit(1);
    }

    next = (rank + 1) % size;
    prev = (rank + size-1) % size;

    tokens = atoi(argv[1]);
    rounds = atoi(argv[2]);

    token_recv_data = (int *) malloc(sizeof(int) * tokens);
    token_send_data = (int *) malloc(sizeof(int) * tokens);
    reqs = (MPI_Request *) malloc(sizeof(MPI_Request) * tokens * 2);
    status = (MPI_Status *) malloc(sizeof(MPI_Status) * tokens * 2);

    MPI_Barrier(MPI_COMM_WORLD);

    start_time = MPI_Wtime();

    for (;rounds > 0; rounds--) {
        /*
        curr_time = MPI_Wtime();
        fprintf(stdout, "[%d] Rounds Left %d:\t Elapsed Time: %f\n", rank, rounds, curr_time - start_time);
        */

        for (i = 0; i < tokens; i++) {
            while (MPI_SUCCESS !=
                    MPI_Irecv(&token_recv_data[i], 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &reqs[i*2]))
                prev = (prev + size-1) % size;
            token_send_data[i] = i;
            while (MPI_SUCCESS !=
                    MPI_Isend(&token_send_data[i], 1, MPI_INT, next, 0, MPI_COMM_WORLD, &reqs[(i*2)+1]))
                next = (next + 1) % size;
        }

        do {
            rc = PMPI_Waitall(tokens * 2, reqs, status);

            for (i = 0; i < tokens; i++)
                if (token_recv_data[i] != i) fprintf(stderr, "[%d] Token %d != %d\n", rank, i, token_recv_data[i]);

            if (rc != MPI_SUCCESS) {
                for (i = 0; i < tokens * 2; i++) {
                    if (status->MPI_ERROR != MPI_SUCCESS) {
                        if (i % 2)
                            while (MPI_SUCCESS !=
                                    MPI_Isend(&token_send_data[i/2], 1, MPI_INT, next,
                                              0, MPI_COMM_WORLD, &reqs[(i*2)+1]))
                                next = (next + 1) % size;
                        else
                            while (MPI_SUCCESS !=
                                    MPI_Irecv(&token_recv_data[i/2], 1, MPI_INT, prev,
                                              0, MPI_COMM_WORLD, &reqs[i*2]))
                                prev = (prev + size-1) % size;
                    }
                }
            }
        } while (rc != MPI_SUCCESS);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    if (rank == 0) fprintf(stdout, "%f\n", end_time - start_time);

    MPI_Finalize();

    return 0;
}
