#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
    int np = 1;
    int errcodes[2];
    int world_rank, world_size;
    MPI_Comm parent_comm, spawn_comm;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Comm_get_parent( &parent_comm );

    if (MPI_COMM_NULL == parent_comm && world_size != 2) {
        fprintf(stderr, "Must run with two processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (parent_comm == MPI_COMM_NULL)
        MPI_Comm_spawn((char*)"./a.out", MPI_ARGV_NULL, np, MPI_INFO_NULL, 0, MPI_COMM_SELF, &spawn_comm, errcodes);
    else
        spawn_comm = parent_comm;


    if(spawn_comm != MPI_COMM_NULL) {
        int high, intra_rank, intra_size, leader;
        MPI_Comm intra_comm, inter_comm, univ_comm;

        if(parent_comm == MPI_COMM_NULL) high = 0;
        else high = 1;

        /* Merge each intercomm between the spawned groups into an intracomm: */
        MPI_Intercomm_merge(spawn_comm, high, &intra_comm);

        MPI_Comm_rank(intra_comm, &intra_rank);
        MPI_Comm_size(intra_comm, &intra_size);

        /* Determine the leader (rank 0 & 1 of the origin world): */
        if (parent_comm == MPI_COMM_NULL) {
            leader = (world_rank+1) % 2;
            MPI_Send(&leader, 1, MPI_INT, 1, 0, intra_comm);
        } else MPI_Recv(&leader, 1, MPI_INT, 1, 0, intra_comm, MPI_STATUS_IGNORE);

        /* Create an intercomm between the two merged intracomms (and use the origin world as bridge/peer communicator): */
        MPI_Intercomm_create(intra_comm, 0, MPI_COMM_WORLD, leader, 123, &inter_comm);

        /* Try a Barrier on this intercomm: */
        MPI_Barrier(inter_comm);

        /* Merge this intercomm into a 'universe' intracomm that covers all procs: */
        MPI_Intercomm_merge(inter_comm, 0, &univ_comm);

        /* Try a Barrier on this intracomm: */
        MPI_Barrier(univ_comm);

        MPI_Comm_free(&univ_comm);
        MPI_Comm_free(&inter_comm);
        MPI_Comm_free(&intra_comm);
        MPI_Comm_free(&spawn_comm);
    }

    MPI_Finalize();
    return 0;
}
