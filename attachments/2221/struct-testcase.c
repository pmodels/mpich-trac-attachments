#include <stdio.h>
#include <limits.h>
#include <mpi.h>
#include <assert.h>
#include <stdlib.h>

/* utility function for creating large contiguous types: algorithim from BigMPI
 * https://github.com/jeffhammond/BigMPI */

static int type_create_contiguous_x(MPI_Count count,
        MPI_Datatype oldtype, MPI_Datatype *newtype)
{
    /* to make 'count' fit MPI-3 type processing routines (which take integer
     * counts), we construct a type consisting of N INT_MAX chunks followed by
     * a remainder.  e.g for a count of 4000000000 bytes you would end up with
     * one 2147483647-byte chunk followed immediately by a 1852516353-byte
     * chunk */
    MPI_Datatype chunks, remainder;
    MPI_Aint lb, extent, disps[2];
    int blocklens[2];
    MPI_Datatype types[2];

    /* truly stupendously large counts will overflow an integer with this math,
     * but that is a problem for a few decades from now.  Sorry, few decades
     * from now! */
    assert(count/INT_MAX == (int)(count/INT_MAX));
    int c = (int)(count/INT_MAX); /* OK to cast until 'count' is 256 bits */
    int r = count%INT_MAX;

    if (c > 0) {
	MPI_Type_vector(c, INT_MAX, INT_MAX, oldtype, &chunks);
	MPI_Type_contiguous(r, oldtype, &remainder);

	MPI_Type_get_extent(oldtype, &lb, &extent);

	blocklens[0] = 1;      blocklens[1] = 1;
	disps[0]     = 0;      disps[1]     = c*extent*INT_MAX;
	types[0]     = chunks; types[1]     = remainder;
	MPI_Type_create_struct(2, blocklens, disps, types, newtype);
	MPI_Type_free(&chunks);
	MPI_Type_free(&remainder);
    } else {
	MPI_Type_contiguous(r, oldtype, newtype);
    }

    return MPI_SUCCESS;
}

 /* like MPI_Type_create_hindexed, except array_of_lengths can be a larger datatype.
 *
 * Hindexed provides 'count' pairs of (displacement, length), but what if
 * length is longer than an integer?  We will create 'count' types, using
 * contig if length is small enough, or something more complex if not */

int ADIOI_Type_create_hindexed_x(int count,
                const MPI_Count array_of_blocklengths[],
                const MPI_Aint array_of_displacements[],
                MPI_Datatype oldtype,
                MPI_Datatype *newtype)
{
    int i, ret;
    MPI_Datatype *types;
    int *blocklens;

    types = malloc(count*sizeof(MPI_Datatype));
    blocklens = malloc(count*sizeof(int));

    for(i=0; i<count; i++) {
        blocklens[i] = 1;
        type_create_contiguous_x(array_of_blocklengths[i], oldtype,  &(types[i]));
    }

    ret = MPI_Type_create_struct(count, blocklens, array_of_displacements,
            types, newtype);
    for (i=0; i< count; i++)
        MPI_Type_free(&(types[i]));
    free(types);
    free(blocklens);

    return ret;
}

int main(int argc, char **argv)
{

    int rank;
    MPI_Datatype send_type;
    MPI_Init(&argc, &argv);
    MPI_Request req;
    MPI_Status stat;


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sendbuf[] = {750, 751, 160, 761, 620, 1071, 480, 631, 1090, 641, 950, 351};
    int recbuf[] = {-1, -1, -1, -1, -1};
    int comparebuf[] = {750, 750, 751, 160, 620};
    int nr_errors=0, tot_errors=0;

    if (rank == 0) {
#ifdef USE_HINDEX
	int 
#else
	MPI_Count
#endif
	    blocks[] = {4, 12, 4};

	MPI_Aint disps[3];
	MPI_Get_address(&(sendbuf[0]), &(disps[0]));
	MPI_Get_address(&(sendbuf[0]), &(disps[1]));
	MPI_Get_address(&(sendbuf[4]), &(disps[2]));

#ifdef USE_HINDEX
	MPI_Type_hindexed(3, blocks, disps, MPI_BYTE, &send_type);
#else
	ADIOI_Type_create_hindexed_x(3, blocks, disps, MPI_BYTE, &send_type);
#endif
	MPI_Type_commit(&send_type);
	MPIDU_Datatype_debug(send_type, 32);
	MPI_Isend(MPI_BOTTOM, 1, send_type, 1, 0, MPI_COMM_WORLD, &req);
	MPI_Type_free(&send_type);
    } else {
	MPI_Irecv(recbuf, 20, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &req);
    }

    MPI_Wait(&req, &stat);
    if (rank == 1) {
	int i;
	for (i=0; i< sizeof(comparebuf)/sizeof(comparebuf[0]); i++) {
	    if (recbuf[i] != comparebuf[i]) {
		fprintf(stderr, "index %d: expected %d saw %d\n",
			i, recbuf[i], comparebuf[i]);
		nr_errors++;
	    }
	}
    }
    MPI_Allreduce(&nr_errors, &tot_errors, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if (tot_errors == 0) printf(" No Errors\n");

    MPI_Finalize();
    return tot_errors;
}

