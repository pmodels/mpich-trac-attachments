#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <mpi.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FNAME "filename.inp"

int
main(int argc, char *argv[])
{
	MPI_File file;
	struct stat sbuf;
	int rc;

	MPI_Init(&argc, &argv);

	MPI_File_open(
		MPI_COMM_WORLD, FNAME, MPI_MODE_RDONLY, MPI_INFO_NULL,
		&file);
	MPI_File_close(&file);

	rc = stat(FNAME, &sbuf);
	assert(-1 == rc);
	assert(ENOENT == errno);

	MPI_Finalize();

	return 0;
}
