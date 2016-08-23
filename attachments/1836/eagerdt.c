#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef MAX_MSGS
#define MAX_MSGS 256000
#endif
int main( int argc, char *argv[] )
{
    int errs = 0;
    int rank, size, dest, source;
    int i;
    int buf[1];
    MPI_Comm      comm;
    MPI_Request   req[MAX_MSGS];

    //    MTest_Init( &argc, &argv );
    MPI_Init( &argc, &argv );

    comm = MPI_COMM_WORLD;
    MPI_Comm_rank( comm, &rank );
    MPI_Comm_size( comm, &size );
    source = 0;
    dest   = size - 1;
    
 
    MPI_Barrier( MPI_COMM_WORLD );
    if (rank == dest) {
	for (i=0; i<MAX_MSGS; i++) {
	    errs=MPI_Recv( buf, 1, MPI_INT, source, i, comm, 
		      MPI_STATUS_IGNORE );
	    if(errs!=MPI_SUCCESS) fprintf(stderr,"Recv Error %u\n",i);
	    if ((i % 1000) == 0) fprintf(stderr,"Recv %u\n",i);
	}
	fprintf(stderr,"Recv %u done\n",MAX_MSGS);
    }
    else if (rank == source ) {
	for (i=0; i<MAX_MSGS; i++) {
	  errs=MPI_Isend( buf, 1, MPI_INT, dest, i, comm, &req[i] );
	  if(errs!=MPI_SUCCESS) fprintf(stderr,"Isend Error %u\n",i);
	  if ((i % 1000) == 0) fprintf(stderr,"Send %u\n",i);
	}
	fprintf(stderr,"Send %u done\n",MAX_MSGS);
	errs=MPI_Waitall( MAX_MSGS, req, MPI_STATUSES_IGNORE );
	if(errs!=MPI_SUCCESS) fprintf(stderr,"Waitall Error %u\n",i);
	fprintf(stderr,"Waitall %u done\n",MAX_MSGS);
    }
    if(rank==source || rank == dest) 
      fprintf(stderr,"Finalize next\n");
    fflush(stderr);
    MPI_Finalize();
    if(rank==source || rank == dest)
      fprintf(stderr,"Done\n");
    return 0;
}
