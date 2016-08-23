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

#define HEADER_EXTENT 16
#define VAR_BEGIN     32

/*----< main() >------------------------------------------------------------*/
int main(int argc, char **argv) {
    int          i, err, rank, nprocs, inbuf[1024], outbuf[1024];
    MPI_Datatype ftype, bodytype;
    MPI_Aint     lb, ub, extent, true_lb, true_extent; 
    int          position=0, typesize, outsize;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    printf("HEADER_EXTENT = %d VAR_BEGIN = %d\n",HEADER_EXTENT,VAR_BEGIN);

    /* create a subarray of 1 x 5 of global array of 1 x (5*nprocs) */
    int array_of_sizes[1], array_of_subsizes[1], array_of_starts[1];
    array_of_sizes[0]    = 5*nprocs;
    array_of_subsizes[0] = 5;
    array_of_starts[0]   = rank * 5;
    MPI_Type_create_subarray(1, array_of_sizes, array_of_subsizes,
                             array_of_starts, MPI_ORDER_C, MPI_INT, &bodytype);
    MPI_Type_commit(&bodytype);

    MPI_Type_ub(bodytype, &ub);
    MPI_Type_size(bodytype, &typesize);
    MPI_Type_get_extent(bodytype, &lb, &extent);
    printf("bodytype lb=%2ld ub=%2ld extent=%2ld size=%2d\n",lb,ub,extent,typesize);

    /* add header space */
    int array_of_blocklengths[2];
    MPI_Aint array_of_displacements[2];
    MPI_Datatype array_of_types[2];

    /* first block is the header */
    array_of_blocklengths[0]  = HEADER_EXTENT;
    array_of_displacements[0] = 0;
    array_of_types[0]         = MPI_BYTE;

    /* second block is the body, a subarray */
    array_of_blocklengths[1]  = 1;
    array_of_displacements[1] = VAR_BEGIN;
    array_of_types[1]         = bodytype;

    err = MPI_Type_create_struct(2, array_of_blocklengths,
                                 array_of_displacements,
                                 array_of_types, &ftype); ERR
    err = MPI_Type_commit(&ftype); ERR

    MPI_Type_ub(ftype, &ub);
    MPI_Type_size(ftype, &typesize);
    MPI_Type_get_extent(ftype, &lb, &extent);
    MPI_Type_get_true_extent(ftype, &true_lb, &true_extent);
    printf("ftype    lb=%2ld ub=%2ld extent=%2ld size=%2d true_lb=%2ld true_extent=%2ld\n",lb,ub,extent,typesize,true_lb, true_extent);

    for (i=0; i<1024; i++)  inbuf[i] = i;
    for (i=0; i<1024; i++) outbuf[i] = -1;

    outsize = typesize * 3; /* pack 3 ftypes */
    err = MPI_Pack(inbuf, 3, ftype, outbuf, outsize, &position, MPI_COMM_SELF); ERR

    for (i=0; i<outsize/sizeof(int); i++) {
        if (i % 10 == 0) printf("\n[%2d]: ", i);
        printf(" %2d", outbuf[i]);
    }
    printf("\n");

    err = MPI_Type_free(&ftype); ERR
    err = MPI_Type_free(&bodytype); ERR

    MPI_Finalize();
    return 0;
}


