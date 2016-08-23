#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>

void err_handler(int err, char *err_msg)
{
    int rank, errorclass, errorStringLen;
    char errorString[MPI_MAX_ERROR_STRING];

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Error_string(err, errorString, &errorStringLen);
    if (err_msg == NULL) err_msg = "";
    printf("rank %d: MPI error (%s) : %s\n", rank, err_msg, errorString);
}

int main(int argc, char **argv) {
    int        err, len=0, rank, get_size;
    char       buf[10], *filename=__FILE__;
    MPI_File   fh;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    err = MPI_File_open(MPI_COMM_WORLD, filename,
                        MPI_MODE_CREATE | MPI_MODE_RDWR,
                        MPI_INFO_NULL, &fh);
    if (err != MPI_SUCCESS) err_handler(err, filename);

    if (rank == 0) len = 10;

    /* mimic the case status object may not be initialized to 0 */
    memset(&status, 1, sizeof(status));

    err = MPI_File_read_all(fh, buf, len, MPI_BYTE, &status);
    if (err != MPI_SUCCESS) err_handler(err, "MPI_File_read_all");

    err = MPI_Get_count(&status, MPI_BYTE, &get_size);
    if (err != MPI_SUCCESS) err_handler(err, "MPI_Get_count");

    if (len != get_size)
        printf("Error: expecting get_size to be %d but got %d\n",len,get_size);

    MPI_File_close(&fh);

    MPI_Finalize();
    return 0;
}

