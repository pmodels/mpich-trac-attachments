#include <mpi.h>
#include <stdio.h>

#define MPI_CHECK(MPI_STATUS, MSG) do {                                  \
    char resultString[MPI_MAX_ERROR_STRING];                             \
    int resultLength;                                                    \
                                                                         \
    if (MPI_STATUS != MPI_SUCCESS) {                                     \
        MPI_Error_string(MPI_STATUS, resultString, &resultLength);       \
        fprintf(stdout, "ior ERROR: %s, MPI %s, (%s:%d)\n",              \
                MSG, resultString, __FILE__, __LINE__);                  \
        fflush(stdout);                                                  \
        MPI_Abort(MPI_COMM_WORLD, -1);                                   \
    }                                                                    \
} while(0)

int main(int argc, char **argv)
{
    int rank;
    MPI_Request req;
    MPI_Status  stats;
    MPI_File fh;
    MPI_Status status;
    MPI_Comm newcomm;

    MPI_CHECK(MPI_Init(&argc, &argv), "MPI_Init");
    MPI_CHECK(MPI_Comm_dup(MPI_COMM_WORLD, &newcomm), "MPI_Comm_dup");
    MPI_CHECK(MPI_Comm_free(&newcomm), "MPI_Comm_free");  
    MPI_File_close(&fh);
    MPI_Finalize();
}
