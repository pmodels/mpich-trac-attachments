/*
 * checklocal.c
 *  <FILE_DESC>
 * 	
 *  Author: Min Si
 */
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int size = 16;
    int rank, nprocs, local_rank, local_nprocs;
    MPI_Comm local_comm;

    MPI_Init(&argc, &argv);

    /* Get a communicator only containing processes with shared memory */
    MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0,
            MPI_INFO_NULL, &local_comm);

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(local_comm, &local_nprocs);
    MPI_Comm_rank(local_comm, &local_rank);

    printf("%d/%d in world, %d/%d in local\n", rank, nprocs, local_rank,
            local_nprocs);

    MPI_Comm_free(&local_comm);

    MPI_Finalize();

    return 0;
}


