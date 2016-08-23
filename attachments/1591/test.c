#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "mpi.h"

void errf( MPI_Comm* comm, int* ec ) {}

int main (int argc, char **argv){
    MPI_Errhandler errh;
    MPI_Init (&argc, &argv);
    MPI_Comm_create_errhandler( (MPI_Comm_errhandler_function*)errf, &errh );
    MPI_Comm_set_errhandler( MPI_COMM_WORLD, errh );
    MPI_Comm_set_errhandler( MPI_COMM_SELF, errh );
    MPI_Finalize ();
    printf( "Done\n" );

    return 0;
}
