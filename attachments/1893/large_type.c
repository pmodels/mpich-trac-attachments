
#include <mpi.h>

static MPI_Datatype make_largexfer_type(MPI_Offset nbytes)
{
    int typechunk_size = 1024*1024; /* in bytes: TODO: figure out how big a
                                       chunk is really needed */
    int chunk_count;
    int remainder=0;
    MPI_Datatype memtype, chunktype;

    /* need to cook up a new datatype to accomodate large datatypes */
    /* first pass: chunks of 1 MiB plus an additional remainder.  Does require
     * 8 byte MPI_Aint, which should have been checked for earlier */

    if (sizeof(MPI_Aint) <= sizeof(int)) {
        return MPI_DATATYPE_NULL;
    }

    chunk_count = nbytes/typechunk_size;
    remainder = nbytes % typechunk_size;
    MPI_Type_contiguous(typechunk_size, MPI_BYTE, &chunktype);

    /* a zero remainder means we can just count contigs */
    if (remainder == 0) {
        MPI_Type_commit(&chunktype);
        memtype = chunktype;
    } else {
        /* struct type: some number of chunks plus remaining bytes tacked
         * on at end */
        int lens[] = {chunk_count, remainder};
        MPI_Aint disp[] = {0, (MPI_Aint) typechunk_size * (MPI_Aint)chunk_count};
        MPI_Datatype types[] = {chunktype, MPI_BYTE};

        MPI_Type_struct(2, lens, disp, types, &memtype);
        MPI_Type_commit(&memtype);
        MPI_Type_free(&chunktype);
    }
    return memtype;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Datatype testtype = make_largexfer_type(2346319872);
    MPIDU_Datatype_debug(testtype);
    MPI_Type_free(&testtype);
    MPI_Finalize();
}
