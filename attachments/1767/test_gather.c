
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define ROOT 7
/* Following should always work for -n 256, -N 32, using longs */
#define COUNT 1048576 
#define VERIFY_CONST 100000000L

int
main(int argc, char *argv[])
{
    int rank, size, ierr;
    float f;
    int i, j, k;
    long *sendbuf;
    long *recvbuf;

    ierr = MPI_Init(&argc, &argv);
    if (ierr != MPI_SUCCESS) {
        fprintf(stderr, "ERROR: MPI_Init() failed: %d\n", ierr);
        exit(2);
    }

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (ierr != MPI_SUCCESS) {
        fprintf(stderr, "ERROR: MPI_Comm_rank() failed: %d\n", ierr);
        exit(2);
    }

    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (ierr != MPI_SUCCESS) {
        fprintf(stderr, "ERROR: MPI_Comm_size() failed: %d\n", ierr);
        exit(2);
    }

    sendbuf = malloc(COUNT * sizeof(long));
    if (sendbuf == NULL) {
        fprintf(stderr, "PE %d:ERROR: malloc of sendbuf failed\n", rank);
    }
    for (i = 0; i < COUNT; i++) {
        sendbuf[i] = (long) i + (long) rank * VERIFY_CONST;
    }

    if (rank == ROOT) {
        recvbuf = malloc(COUNT * sizeof(long) * size);
        if (recvbuf == NULL) {
            fprintf(stderr, "PE %d:ERROR: malloc of recvbuf failed\n", rank);
        }
        for (i = 0; i < COUNT * size; i++) {
            recvbuf[i] = -456789L;
        }
    }

    ierr = MPI_Gather(sendbuf, COUNT, MPI_LONG, recvbuf, COUNT, MPI_LONG, ROOT, MPI_COMM_WORLD);
    if (ierr != MPI_SUCCESS) {
        fprintf(stderr, "PE %d: ERROR: MPI_Gather() failed: %d\n", rank, ierr);
        exit(2);
    }

    if (rank == ROOT) {
        for (i = 0; i < size; i++) {
            int lerr = 0;
            for (j = 0; j < COUNT; j++) {
                if (recvbuf[i * COUNT + j] != i * VERIFY_CONST + j) {
                    printf("PE 0: mis-match error");
                    printf("  recbuf[%d * %d + %d] = ", i, COUNT, j);
                    printf("  %ld,", recvbuf[i * COUNT + j]);
                    printf("  should be %ld\n", i * VERIFY_CONST + j);
                    lerr++;
                    if (lerr > 10) {
                        j = COUNT;
		    }
                } 
            }
        }
    }

    ierr = MPI_Barrier(MPI_COMM_WORLD);

    ierr = MPI_Finalize();
    if (ierr != MPI_SUCCESS) {
        fprintf(stderr, "ERROR: MPI_Finalize() failed: %d\n", ierr);
        exit(2);
    }
}

