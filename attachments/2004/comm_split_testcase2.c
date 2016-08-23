#include <mpi.h>
#include <stdio.h>

/* when deferred open is used, the processes participating in file open will
 * split the communicator. Saw some odd behavior at scale, so try to extract */

static void handle_error(int errcode, const char *str)
{
    char msg[MPI_MAX_ERROR_STRING];
    int resultlen;
    MPI_Error_string(errcode, msg, &resultlen);
    fprintf(stderr, "%s: %s\n", str, msg);
    MPI_Abort(MPI_COMM_WORLD, 1);
}


int main(int argc, char **argv)
{
    int rank, size, color, ret, buf[2];
    MPI_Comm dup_comm, new_comm;
    MPI_Info info;


    MPI_Init(&argc, &argv);

    MPI_Comm_dup(MPI_COMM_WORLD, &dup_comm);

    MPI_Comm_rank(dup_comm, &rank);
    MPI_Comm_size(dup_comm, &size);

    MPI_Info_create(&info);
    MPI_Info_set(info, "romio_no_indep_rw", "true");

    if (rank == 0) {
	buf[0] = 99; buf[1] = 100;
    }
    MPI_Bcast(buf, 2, MPI_INT, 0, dup_comm);

    if ( (rank%137) == 0)
	color=1;
    else
	color=MPI_UNDEFINED;

    /* in my experience, a single comm_split is trouble.  Others have reported
     * trouble only with the second comm_split, so consider putting this in a
     * loop */
    int i;
    for(i=0; i< 10; i++) {
	ret = MPI_Comm_split(dup_comm, color, 0, &new_comm);
	if (ret != MPI_SUCCESS) handle_error(ret, "MPI_Comm_split");

	if (new_comm != MPI_COMM_NULL) MPI_Comm_free(&new_comm);
    }
    MPI_Info_free(&info);
    MPI_Comm_free(&dup_comm);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
	fprintf(stderr, " No Errors\n");
    MPI_Finalize();
    return 0;
}
