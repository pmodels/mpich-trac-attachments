/*
 * This test program is extracted from a PnetCDF test program that tests an
 * MPI fileview set by a data type created by concatenating two noncontiguous
 * data types. Root process's fileview consists of a file header of size
 * HEADER_EXTENT and a subarray in file body starting from VAR_BEGIN.
 * The rest processes have a fileview that consists of only the body part.
 *
 * To compile:
 * 	mpicc -g -o mpi_struct mpi_struct.c 
 *
 * To run:
 *	mpiexec -l -n 2 mpi_struct
 *
 * Expected stdout:
 *     0   1   2   3   4  10  11  12  13  14
 *     5   6   7   8   9  15  16  17  18  19
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ERR \
    if (err != MPI_SUCCESS) { \
        int errorStringLen; \
        char errorString[MPI_MAX_ERROR_STRING]; \
        MPI_Error_string(err, errorString, &errorStringLen); \
        printf("Error at line %d: %s\n",__LINE__, errorString); \
    }

#define HEADER_EXTENT 512
#define VAR_BEGIN     1024

/*----< main() >------------------------------------------------------------*/
int main(int argc, char **argv) {
    int          i, j, err, rank, nprocs, *buf, buflen;
    int          sizes[3], starts[3], counts[3], array_of_blocklengths[2];
    MPI_Aint     extent, array_of_displacements[2];
    MPI_Offset   offset;
    MPI_File     fh;
    MPI_Datatype ftype, ftype0, bodytype, array_of_types[2];
    MPI_Status   status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (nprocs != 4) {
        printf("Error: this program must run on 4 processes ... exiting\n");
	MPI_Finalize();
	return 0;
    }

    /* open the file */
    err = MPI_File_open(MPI_COMM_WORLD, "testfile",
                        MPI_MODE_CREATE | MPI_MODE_RDWR,
                        MPI_INFO_NULL, &fh);
    if (err != MPI_SUCCESS) {
        printf("Error: MPI_File_open() filename %s\n",argv[1]);
	MPI_Abort(MPI_COMM_WORLD, -1);
	exit(1);
    }
    /* fill file with zeros */
    int file_size=HEADER_EXTENT+VAR_BEGIN+(2*nprocs*5)*sizeof(int);
    char *zeros = (char*)calloc(file_size,1);
    if (rank == 0) {
        err = MPI_File_write(fh, zeros, file_size, MPI_CHAR, &status); ERR
    }

    /* create a subarray of 2 x 2 x 2 of global array of 4 x 4 x 2 */
    int array_of_sizes[3], array_of_subsizes[3], array_of_starts[3];
    array_of_sizes[0]    = 4;
    array_of_sizes[1]    = 4;
    array_of_sizes[2]    = 2;
    array_of_subsizes[0] = 2;
    array_of_subsizes[1] = 2;
    array_of_subsizes[2] = 2;
    array_of_starts[0]   = rank/2 * array_of_subsizes[0];
    array_of_starts[1]   = rank%2 * array_of_subsizes[1];
    array_of_starts[2]   = 0;
    MPI_Type_create_subarray(3, array_of_sizes, array_of_subsizes,
                             array_of_starts, MPI_ORDER_C, MPI_INT, &bodytype);
    MPI_Type_commit(&bodytype);

    for (buflen=1,i=0; i<3; i++) buflen *= array_of_subsizes[i];
    buf = (int*) malloc(buflen * sizeof(int));
    for (i=0; i<buflen; i++) buf[i] = rank+1;

    /* set the file view */
    if (rank == 0) {
        /* add header space */

        /* first block is the header */
        array_of_blocklengths[0]  = HEADER_EXTENT;
        array_of_displacements[0] = 0;
        array_of_types[0]         = MPI_BYTE;

        /* second block is the body, a subarray */
        array_of_blocklengths[1]  = 1;
        array_of_displacements[1] = VAR_BEGIN;
        array_of_types[1]         = bodytype;

        err = MPI_Type_create_struct(2, array_of_blocklengths, array_of_displacements,
                                     array_of_types, &ftype); ERR
        MPI_Type_commit(&ftype);
        err = MPI_File_set_view(fh, 0, MPI_BYTE, ftype, "native", MPI_INFO_NULL); ERR
        err = MPI_Type_free(&ftype); ERR
        offset = HEADER_EXTENT;
    }
    else {
        /* wrap bodytype with a struc type */
        array_of_blocklengths[0]  = 1;
        array_of_displacements[0] = VAR_BEGIN;
        array_of_types[0]         = bodytype;

        err = MPI_Type_create_struct(1, array_of_blocklengths, array_of_displacements,
                                     array_of_types, &ftype); ERR
        MPI_Type_commit(&ftype);

MPI_Aint lb, extent, true_lb, true_extent;
int typesize;
MPI_Type_lb(ftype, &lb);
MPI_Type_size(ftype, &typesize);
MPI_Type_extent(ftype, &extent);
MPI_Type_get_true_extent(ftype, &true_lb, &true_extent);
printf("          ftype    lb=%2ld extent=%3ld size=%2d true_lb=%2ld true_extent=%2ld\n",
lb,extent,typesize,true_lb, true_extent);
printf("Expecting ftype    lb=%2ld extent=%3ld size=%2d true_lb=%2ld true_extent=%2ld\n",
VAR_BEGIN,array_of_sizes[0]*array_of_sizes[1]*array_of_sizes[2]*sizeof(int),typesize,true_lb, true_extent);

        err = MPI_File_set_view(fh, 0, MPI_BYTE, ftype, "native", MPI_INFO_NULL); ERR
        err = MPI_Type_free(&ftype); ERR
        offset = 0;
    }
    err = MPI_Type_free(&bodytype); ERR

    /* ROMIO runs fine with this write with explicit offset, but fails with
       seek+write. Uncomment this to see.
    err = MPI_File_write_at_all(fh, offset, buf, 10, MPI_INT, &status); ERR
    */
    err = MPI_File_seek(fh, offset, MPI_SEEK_SET); ERR
    err = MPI_File_write_all(fh, buf, buflen, MPI_INT, &status); ERR

    /* read back the body part */
    err = MPI_File_set_view(fh, 0, MPI_BYTE, MPI_BYTE, "native", MPI_INFO_NULL); ERR
    int *inBuf = (int*) malloc(nprocs * buflen * sizeof(int));
    err = MPI_File_read_at(fh, VAR_BEGIN, inBuf, buflen*nprocs, MPI_INT, &status); ERR
    if (rank == 0) {
        int expected[] = {1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2,
                          3, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 3, 4, 4, 4, 4};
        for (i=0; i<4*4*2; i++)
            if (inBuf[i] != expected[i])
                printf("Unexpected data: [%2d] %d (expecting %d)\n",
                       i,inBuf[i],expected[i]);
    }
    err = MPI_File_close(&fh); ERR
    free(inBuf);
    free(buf);

    MPI_Finalize();
    return 0;
}


