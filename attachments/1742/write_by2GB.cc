#include <iostream>
#include "mpi.h"
#
//! \abstract This test is a demonstration how to *bypass* the bug that MPI IO is not correctly writing
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

  // With this example, you can write as many bytes as you want...

  int count=((1ul<<31)-8)/sizeof(long);

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

  const unsigned long int MPIIO_MAX_BYTES_PER_TRANSACTION = (1ul<<31)-4096;

  const unsigned long int  max_count_per_bloc = MPIIO_MAX_BYTES_PER_TRANSACTION/sizeof(long);
  const unsigned long int  bytes_per_bloc     = max_count_per_bloc*sizeof(long);
  const unsigned long int  num_blocs          = count/max_count_per_bloc+(count%max_count_per_bloc ? 1 : 0);

  std::cout << "MPIIO_MAX_BYTES_PER_TRANSACTION (bytes): "<<MPIIO_MAX_BYTES_PER_TRANSACTION<<std::endl;
  std::cout << "max_count_per_bloc                     : "<<max_count_per_bloc<<std::endl;
  std::cout << "count                                  : "<<count<<std::endl;
  std::cout << "num_blocs                              : "<<num_blocs<<std::endl;
  std::cout << "max_count_per_bloc                     : "<<max_count_per_bloc<<std::endl;
  std::cout << "bytes_per_bloc                         : "<<bytes_per_bloc<<std::endl;

  // Now write by blocs of 2^31-4096 bytes
  for (unsigned long int i = 0; i < num_blocs; ++i) {
    unsigned long int count_in_this_bloc = max_count_per_bloc;
    if (i == num_blocs-1) {
      if (count*sizeof(long)%bytes_per_bloc) {
        count_in_this_bloc = count%max_count_per_bloc;
        std::cout << "last bloc! dimension: "<<count_in_this_bloc << " long int" << std::endl;
      }
    }
    std::cout << "We write "<<count_in_this_bloc << " objects from array[" << i*max_count_per_bloc << "] = " << array[i*max_count_per_bloc]<< std::endl;

    abortOnError(MPI_File_write(fh, &(array[i*max_count_per_bloc]), count_in_this_bloc, MPI_LONG, MPI_STATUS_IGNORE));

  }

  std::cout << "Wrote everything by blocs and MPI_file_write returned OK..." <<std::endl << std::endl;

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

  // ERROR if you try to read all at once...
  abortOnError(MPI_File_read(fh, array, count, MPI_LONG, MPI_STATUS_IGNORE));
/*
  // Now read by blocs of 2^31-4096 bytes
  for (unsigned long int i = 0; i < num_blocs; ++i) {
    unsigned long int count_in_this_bloc = max_count_per_bloc;
    if (i == num_blocs-1) {
      if (count*sizeof(long)%bytes_per_bloc) {
        count_in_this_bloc = count%max_count_per_bloc;
        std::cout << "last bloc! dimension: "<<count_in_this_bloc << " long int" << std::endl;
      }
    }
    std::cout << "We read "<<count_in_this_bloc << " objects from array[" << i*max_count_per_bloc << "]" << std::endl;

    abortOnError(MPI_File_read(fh, &(array[i*max_count_per_bloc]), count_in_this_bloc, MPI_LONG, MPI_STATUS_IGNORE));

  }
*/
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

