#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] )
{
    int i, dest;
    int size, rank;
    float *localA, *localB;
    int max_elem;
    MPI_Comm comm;
    MPI_Request requests[20];

    MPI_Init( &argc, &argv );
    comm = MPI_COMM_WORLD;

    MPI_Comm_size( comm, &size );
    MPI_Comm_rank( comm, &rank );

    max_elem = 20;
    localA = (float *)malloc( max_elem * size * sizeof(float) );
    localB = (float *)malloc( max_elem * size * sizeof(float) );

    for ( i=0; i<size; i++ ) {
         dest = (rank+i) % size;
         MPI_Irecv(&localB[max_elem*dest], max_elem, MPI_FLOAT, dest,
              rank+1, comm, &requests[dest]);
       }

    for ( i=0; i<size; i++ ) {
         dest = (rank+i) % size;
        MPI_Isend(&localA[max_elem*dest], max_elem, MPI_FLOAT, dest, dest+1,
                    comm, &requests[dest+size]);
       }

       MPI_Waitall(size*2,requests, MPI_STATUSES_IGNORE);
       MPI_Finalize();

}
