#include <mpi.h>
#include <iostream>
#include <windows.h>

int main( int argc, char ** argv ) {
    MPI_Init( 0, 0 );

    int rank, size;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    std::cout << "Rank: " << rank << ", size: " << size << "\n";

    if ( rank < size / 2 ) {
        TerminateProcess( GetCurrentProcess(), -2 );
    } else {
        Sleep( 1000 );
    }

    MPI_Finalize();
}
