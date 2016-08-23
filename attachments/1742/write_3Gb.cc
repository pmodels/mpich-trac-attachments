#include <iostream>
#include "mpi.h"
#
//! \abstract This test is a demonstration that MPI IO is not correctly writing
//!           more than (2^31 - 4096) bytes even if MPI_SUCCESS is returned...
//! \author Eric Chamberland
//! \date nov 9, 2012

void abortOnError(int ierr) {
  if (ierr != MPI_SUCCESS) {
    printf("ERROR Returned by MPI: %d\n",ierr);
    char* lCharPtr = new char[MPI_MAX_ERROR_STRING];
    int lLongueur = 0;
    MPI_Error_string(ierr,lCharPtr, &lLongueur);
    printf("ERROR_string Returned by MPI: %s\n",lCharPtr);
    MPI_Abort( MPI_COMM_WORLD, 1 );
  }
}

int main (int argc, char *argv[]) 
{

  MPI_File     fh;
  long        *array;

  MPI_Init( &argc, &argv );

  int nb_proc = 0;
  MPI_Comm_size( MPI_COMM_WORLD, &nb_proc );
  if (nb_proc != 1) {
    printf( "Test code for 1 process!\n" );
    MPI_Abort( MPI_COMM_WORLD, 1 );
  }

  // We want to write 2^31 bytes but we can't write more than the limit that is strangely 2^31-4096

  int count=((1ul<<31)-8)/sizeof(long);
  //int count=((1ul<<31)-4096)/sizeof(long); // This is the REAL limit of MPI -- try it!

  array = new long[count];

  // Initialise the array
  for (int i = 0; i < count; ++i) {
    array[i] = i;
  }

  std::cout << "----------------------------------------" << std::endl;
  std::cout << "We try to write "
            << count << " long int"
            << "(" << count*sizeof(long) << " bytes)"<< std::endl;
  std::cout << "----------------------------------------" << std::endl<< std::endl;

  const char* lFileName = "2.6Gb";
  abortOnError(MPI_File_open(MPI_COMM_WORLD,
                             const_cast<char*>(lFileName),
                             MPI_MODE_CREATE | MPI_MODE_WRONLY,
                             MPI_INFO_NULL, &fh));


  abortOnError(MPI_File_write(fh, array, count, MPI_LONG, MPI_STATUS_IGNORE));

  std::cout << "Wrote everything and MPI_file_write returned OK..." <<std::endl << std::endl;

  abortOnError(MPI_File_close(&fh));

  abortOnError(MPI_File_open(MPI_COMM_WORLD,
                             const_cast<char*>(lFileName),
                             MPI_MODE_RDONLY,
                             MPI_INFO_NULL,
                             &fh));

  // Reset the array
  for (int i = 0; i < count; ++i) {
    array[i] = -1;
  }

  abortOnError(MPI_File_read(fh, array, count, MPI_LONG, MPI_STATUS_IGNORE));

  std::cout << "Readed everything and MPI_file_read returned OK..." <<std::endl<< std::endl;

  // we are here, so everything is okay: verify the array
  for (int i = 0; i < count ; ++i) {
    if (array[i] != i) {
      std::cout << "***********************************************" << std::endl;
      std::cout << "ERROR! array is WRONG at indice:" << i
                << ", the wrong value is: " << array[i]
                << std::endl
                << std::endl;
      std::cout << "This is indice " << count - i
                << " from the END of the array" << std::endl;
      std::cout << "  or offset " << (count - i)*sizeof(long)
                << " bytes from the END of the array" << std::endl;
      std::cout << "***********************************************" << std::endl<< std::endl;

      break;
    }
  }

  MPI_File_close(&fh);

  MPI_Finalize();
  return 0;
} 

