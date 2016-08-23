#include "mpi.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>

int main ( int argc, char *argv[] )
{
  int rank;
  int size;
  char hostname[255];
  int size_hostname;
  double wtime;

  MPI::Init ( argc, argv );
  size = MPI::COMM_WORLD.Get_size();
  rank = MPI::COMM_WORLD.Get_rank();
  MPI::Get_processor_name(hostname, size_hostname);
  
  if ( rank == 0 ){
    std::cout << "  print size = " << size << std::endl;
  }

  std::cout << "I am rank=" << rank 
            << " and my hostname=" << hostname << std::endl;
  
  MPI::Finalize();

  return 0;
}
