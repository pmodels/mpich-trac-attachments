/* usage: cp_subarray <filename> */
#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define ROWS                (16384)
#define COLS                (8192)
#define MPI_DATATYPE        (MPI_DOUBLE)
#define C_DATATYPE          double 
#define DIMS                (2)
#define COLL_BUFSIZE        (8192*1024)

int main(
    int argc,
    char **argv)
{
    int i = 0, nprocs = 0, mynod = 0, provided = 0, 
	c_size = 0, mpi_size = 0, iterations = 0,
        array_size[] = { 0, 0 }, array_subsize[] = { 0, 0}, 
	array_start[] = { 0, 0};
    long rows = 0l, cols = 0l, coll_bufsize = 0l, 
	 rows_view = 0l, cols_view = 0l, rows_collbuf = 0l, 
	 cols_collbuf = 0l, elts_collbuf = 0l;
    unsigned long filesize = 0l;
    double *buffer = NULL;
    MPI_File fhandle;
    MPI_Status status;
    MPI_Datatype subarray;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &mynod);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    MPI_Type_size(MPI_DATATYPE, &mpi_size);
    c_size = sizeof(C_DATATYPE);
    if (c_size != mpi_size)
    {
        fprintf(stderr, "Datatypes in MPI and C do not match\n");
        MPI_Abort(MPI_COMM_WORLD, EIO);
    }

    rows = ROWS;
    cols = COLS;
    coll_bufsize = COLL_BUFSIZE;
    elts_collbuf = coll_bufsize / mpi_size;
    rows_view = rows / nprocs;
    cols_view = cols;
    cols_collbuf = cols_view;
    rows_collbuf = elts_collbuf / cols_collbuf;
    filesize = rows * cols * mpi_size;
    array_size[0] = rows;
    array_size[1] = cols;
    array_subsize[0] = rows_view;
    array_subsize[1] = cols_view;
    array_start[0] = rows_view * mynod;
    array_start[1] = 0;

    buffer = (C_DATATYPE *) malloc(coll_bufsize);
    if (!buffer)
    {
        fprintf(stderr, "calloc error\n");
        MPI_Abort(MPI_COMM_WORLD, ENOMEM);
    }

    MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_CREATE|MPI_MODE_RDONLY, 
		    MPI_INFO_NULL, &fhandle);

    MPI_Type_create_subarray(DIMS, array_size, array_subsize, array_start,
                             MPI_ORDER_C, MPI_DATATYPE, &subarray);
    MPI_Type_commit(&subarray);
    MPI_File_set_view(fhandle, 0, MPI_DATATYPE, subarray, "native",
                      MPI_INFO_NULL);

    iterations = rows_view / rows_collbuf;

    for (i = 0; i < iterations; i++)
        MPI_File_read_all(fhandle, buffer, elts_collbuf, MPI_DATATYPE, &status);

    MPI_File_close(&fhandle);
    free(buffer);
    MPI_Finalize();

    return 0;
}
