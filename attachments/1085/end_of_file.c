#include "mpi.h"
#include <stdio.h>
#define IO_SIZE 10

/* test case contributed by Pascal Deveze <Pascal.Deveze@bull.net> */
/* run with two processors */
/* expected result: ....
*/

int main(int argc, char **argv) {
    MPI_File fh;
    MPI_Status status;
    int i, myid, count, errs=0, toterrs;
    unsigned char buffer[IO_SIZE];
    unsigned char cmp[IO_SIZE];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    if (argc != 2) {
	    fprintf(stderr, "usage: %s <filename>\n", argv[0]);
	    MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_CREATE |
		    MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_File_set_errhandler(fh,MPI_ERRORS_ARE_FATAL);
    for (i=0; i < IO_SIZE; i++) {
	    buffer [i] = i;
	    cmp[i] = i;
    }
    if (myid==0) MPI_File_write(fh,buffer,IO_SIZE-5,MPI_CHAR,&status);

    for (i=0; i < IO_SIZE; i++) buffer [i] = 99;
    for (i=IO_SIZE-5; i< IO_SIZE; i++) cmp[i] = 99;
    
    /* strictly speaking, this sync/barrier/sync closes one write access and
     * ensures subsequent read access sees latest data */
    MPI_File_sync(fh);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_File_sync(fh);

    MPI_File_seek(fh, 0, MPI_SEEK_SET);
    /* everybody reads the first IO_SIZE bytes from the file, but the file is
     * only IO_SIZE-5 bytes big.  */
    MPI_File_read_all(fh,buffer, IO_SIZE, MPI_BYTE, &status);


    MPI_Get_count(&status, MPI_CHAR, &count);
    if (count != IO_SIZE-5) {
	    fprintf(stderr, "%d: count was %d; expected %d\n", 
			    myid, count, IO_SIZE-5);
	    errs++;
    }

    for (i=0; i< IO_SIZE; i++) {
	    if (buffer[i] != cmp[i]) {
		    fprintf(stderr, "%d: buffer[%d] = %d; expected %d\n", 
				    myid, i, buffer[i], cmp[i]);
		    errs++;
	    }
    }

    MPI_Allreduce(&errs, &toterrs, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if (myid== 0) {
	    if (toterrs > 0) {
		    fprintf(stderr, "Found %d errors\n", toterrs);
	    } else {
		    fprintf(stdout, " No Errors\n");
	    }
    }

    MPI_File_close(&fh);
    MPI_Finalize();
    return errs;
}
