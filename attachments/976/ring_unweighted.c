#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

/*
   Specify a distributed graph of a bidirectional ring of the MPI_COMM_WORLD,
   i.e. everyone only talks to left and right neighbors. 
*/
int main(int argc, char *argv[])
{
    MPI_Comm    dgraph_comm;
    int         world_size, world_rank, ierr;

    int         src_sz, dest_sz;
/*    int        *degs; */
    int        *srcs, *dests;
    int         comm_topo;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

/*
    srcs     = (int *) malloc( 1 * sizeof(int) );
    srcs[0]  = world_rank;
    degs     = (int *) malloc( 1 * sizeof(int) );
    degs[0]  = 2;
    dests    = (int *) malloc( 2 * sizeof(int) );
    dests[0] = world_rank-1 <  0          ? world_size-1 : world_rank-1 ;
    dests[1] = world_rank+1 >= world_size ?            0 : world_rank+1 ;
    ierr = MPI_Dist_graph_create(MPI_COMM_WORLD, 1, srcs, degs, dests,
                                 MPI_UNWEIGHTED, MPI_INFO_NULL, 1,
                                 &dgraph_comm);
    if ( ierr != MPI_SUCCESS ) 
        fprintf(stderr, "MPI_Dist_graph_create() fails!\n");
    else
        fprintf(stderr, "MPI_Dist_graph_create() succeeds!\n");
     
    comm_topo = MPI_UNDEFINED;
    MPI_Topo_test(dgraph_comm, &comm_topo);
    if (comm_topo == MPI_DIST_GRAPH)
        fprintf(stderr, "dgraph_comm is of type MPI_DIST_GRAPH\n");
    else
        fprintf(stderr, "dgraph_comm is NOT of type MPI_DIST_GRAPH\n");
    
    MPI_Comm_free(&dgraph_comm);
    free(degs);
    free(srcs);
    free(dests);
*/
    
    src_sz   = 2;
    srcs     = (int *) malloc(src_sz * sizeof(int));
    srcs[0]  = world_rank-1 <  0          ? world_size-1 : world_rank-1 ;
    srcs[1]  = world_rank+1 >= world_size ?            0 : world_rank+1 ;
    dest_sz  = 2;
    dests    = (int *) malloc(dest_sz * sizeof(int));
    dests[0] = world_rank-1 <  0          ? world_size-1 : world_rank-1 ;
    dests[1] = world_rank+1 >= world_size ?            0 : world_rank+1 ;
    ierr = MPI_Dist_graph_create_adjacent(MPI_COMM_WORLD,
                                          src_sz, srcs, MPI_UNWEIGHTED,
                                          dest_sz, dests, MPI_UNWEIGHTED,
                                          MPI_INFO_NULL, 1, &dgraph_comm);
    if ( ierr != MPI_SUCCESS ) 
        fprintf(stderr, "MPI_Dist_graph_create_adjacent() fails!\n");
    else
        fprintf(stderr, "MPI_Dist_graph_create_adjacent() succeeds!\n");
     
    comm_topo = MPI_UNDEFINED;
    MPI_Topo_test(dgraph_comm, &comm_topo);
    if (comm_topo == MPI_DIST_GRAPH)
        fprintf(stderr, "dgraph_comm is of type MPI_DIST_GRAPH\n");
    else
        fprintf(stderr, "dgraph_comm is NOT of type MPI_DIST_GRAPH\n");
    
    MPI_Comm_free(&dgraph_comm);
    free(srcs);
    free(dests);
    MPI_Finalize();
    return 0;
}
