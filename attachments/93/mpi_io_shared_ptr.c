/*============================================================================
 * Parallel file I/O shared pointer bug test
 *============================================================================*/

#define MY_MPI_IO_BUG 1

/*----------------------------------------------------------------------------
 * Standard C library headers
 *----------------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#if 0
} /* Fake brace to force Emacs auto-indentation back to column 0 */
#endif
#endif /* __cplusplus */

/*============================================================================
 * Private function definitions
 *============================================================================*/

/*----------------------------------------------------------------------------
 * Output MPI error message.
 *
 * This supposes that the default MPI errorhandler is not used
 *
 * parameters:
 *   error_code <-- associated MPI error code
 *
 * returns:
 *   0 in case of success, system error code in case of failure
 *----------------------------------------------------------------------------*/

static void
_mpi_io_error_message(int error_code)
{
  char buffer[MPI_MAX_ERROR_STRING];
  int  buffer_len;

  MPI_Error_string(error_code, buffer, &buffer_len);

  printf("MPI IO error %d: %s", error_code, buffer);
}

/*----------------------------------------------------------------------------
 * Return the position of the file pointer.
 *
 * When using MPI-IO with individual file pointers, we consider the file
 * pointer to be equal to the highest value of then individual file pointers.
 *
 * parameters:
 *   fh <-- MPI IO file descriptor
 *
 * returns:
 *   current position of the file pointer
 *----------------------------------------------------------------------------*/

MPI_Offset
_mpi_file_tell(MPI_File  fh)
{
  int errcode = MPI_SUCCESS;
  MPI_Offset offset = 0, disp = 0, retval = 0;

  int rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

#if defined(MY_MPI_IO_BUG)

  printf("rank %d: will call MPI_File_get_position_shared\n", rank);

  errcode = MPI_File_get_position_shared(fh, &offset);

  if (errcode == MPI_SUCCESS) {
    MPI_File_get_byte_offset(fh, offset, &disp);
    retval = disp;
  }

  printf("rank %d: offsets: %ld %ld\n", rank, (long)offset, (long)disp);

#else

  long aux[2];
  if (rank == 0) {

    printf("root rank will call MPI_File_get_position_shared\n");

    errcode = MPI_File_get_position_shared(fh, &offset);

    if (errcode == MPI_SUCCESS) {
      MPI_File_get_byte_offset(fh, offset, &disp);
      retval = disp;
    }
    aux[0] = disp; aux[1] = retval;
  }

  MPI_Bcast(aux, 2, MPI_LONG, 0, MPI_COMM_WORLD);
  disp = aux[0];
  retval = aux[1];

  printf("rank %d: offsets: %ld %ld\n", rank, (long)offset, (long)disp);

#endif

  if (errcode != MPI_SUCCESS)
    _mpi_io_error_message(errcode);

  return retval;
}

/*============================================================================
 * Unit test
 *============================================================================*/

static void
_create_test_data(void)
{
  int i;
  FILE *f;

  char header[80];
  char footer[80];

  sprintf(header, "fvm test file");
  for (i = strlen(header); i < 80; i++)
    header[i] = '\0';

  sprintf(footer, "fvm test file end");
  for (i = strlen(footer); i < 80; i++)
    footer[i] = '\0';

  f = fopen("file_test_data", "w+");

  fwrite(header, 1, 80, f);
  fwrite(footer, 1, 80, f);

  fclose(f);
}

/*---------------------------------------------------------------------------*/

int
main (int argc, char *argv[])
{
  int rank = 0;
  int retval = MPI_SUCCESS;
  MPI_Offset offset;

  MPI_File fh = MPI_FILE_NULL;

  /* Initialization */

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
    _create_test_data();

  /* Open file */

  retval = MPI_File_open(MPI_COMM_WORLD,
                         "file_test_data",
                         MPI_MODE_RDONLY,
                         MPI_INFO_NULL,
                         &fh);

  if (retval != MPI_SUCCESS)
    _mpi_io_error_message(retval);

#if 0 /* Hides the bug, but does not make it dissapear in more
         complex cases (i.e. when MPI_File_read_shared is used
         locally and MPI_File_read_ordered is used collectively) */

  retval = MPI_File_seek_shared(fh, 1, MPI_SEEK_SET);
  if (retval != MPI_SUCCESS)
    _mpi_io_error_message(retval);
#endif

  offset = _mpi_file_tell(fh);

  /* Close file */

  retval = MPI_File_close(&fh);

  if (retval != MPI_SUCCESS)
    _mpi_io_error_message(retval);

  /* We are finished */

  MPI_Finalize();

  exit(EXIT_SUCCESS);
}
