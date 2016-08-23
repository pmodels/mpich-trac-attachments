/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *
 *  (C) 2003 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */
#include "mpi.h"
#include "mpitest.h"
#include <stdio.h>
#include <stdlib.h>

static char MTEST_Descrip[] = "A simple test of Comm_spawn";

int main( int argc, char *argv[] )
{
    int errs = 0, err, error=0;
    int rank, size, rsize, i;
    int np = 4;
    int errcodes[4];
    MPI_Comm      parentcomm, intercomm;
    MPI_Status    status;

    MTest_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm_get_parent( &parentcomm );

    if (parentcomm == MPI_COMM_NULL) {
	/* Create 2 more processes */
	error=MPI_Comm_spawn( argv[0], MPI_ARGV_NULL, np,
			MPI_INFO_NULL, 0, MPI_COMM_WORLD,
			&intercomm, errcodes ); 
    	if ( error != MPI_SUCCESS )
		errs++;
	}
    else 
	intercomm = parentcomm;

    /* We now have a valid intercomm */

    MPI_Comm_remote_size( intercomm, &rsize );
    printf("rsize %d\n ", rsize);
    MPI_Comm_size( intercomm, &size );
    printf("size %d\n ", size);
    MPI_Comm_rank( intercomm, &rank );
    printf("rank %d\n ", rank);

    if (parentcomm == MPI_COMM_NULL) {
	/* Master */
	if (rsize != np) {
	    errs++;
	    printf( "Did not create %d processes (got %d)\n", np, rsize );
	}
	if (rank == 0) {
	    for (i=0; i<rsize; i++) {
		MPI_Send( &i, 1, MPI_INT, i, 0, intercomm );
	    }
	    /* We could use intercomm reduce to get the errors from the 
	       children, but we'll use a simpler loop to make sure that
	       we get valid data */
	    for (i=0; i<rsize; i++) {
		MPI_Recv( &err, 1, MPI_INT, i, 1, intercomm, MPI_STATUS_IGNORE );
		errs += err;
	    }
	}
    }
    else {
	/* Child */
	if (size != np) {
	    errs++;
	    printf( "(Child) Did not create %d processes (got %d)\n", 
		    np, size );
	}
	MPI_Recv( &i, 1, MPI_INT, 0, 0, intercomm, &status );
	if (i != rank) {
	    errs++;
	    printf( "Unexpected rank on child %d (%d)\n", rank, i );
	}
	/* Send the errs back to the master process */
	MPI_Ssend( &errs, 1, MPI_INT, 0, 1, intercomm );
    }

    /* It isn't necessary to free the intercomm, but it should not hurt */
    /* Using Comm_disconnect instead of free should provide a stronger
     * test, as a high-quality MPI implementation will be able to
     * recover some resources that it should hold on to in the case 
     * of MPI_Comm_free */
    /*     MPI_Comm_free( &intercomm ); */
    MPI_Comm_disconnect( &intercomm ); 

    /* Note that the MTest_Finalize get errs only over COMM_WORLD */
    /* Note also that both the parent and child will generate "Success"
       if both call MTest_Finalize */
    if (parentcomm == MPI_COMM_NULL) {
	MTest_Finalize( errs );
    }

    MPI_Finalize();
    return 0;
}
