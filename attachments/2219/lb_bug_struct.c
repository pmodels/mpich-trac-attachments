/*
 * This program implements the example 4.6 of MPI-3 standard.
 *
 * Example 4.6
 * Let type1 have type map, {(double, 0), (char, 8)}, with extent 16.
 * Let B = (2, 1, 3), D = (0, 16, 26), and T = (MPI_FLOAT, type1, MPI_CHAR).
 * Then a call to MPI_TYPE_CREATE_STRUCT(3, B, D, T, newtype) returns a
 * datatype with type map: {(float, 0), (float, 4), (double, 16), (char, 24),
 * (char, 26), (char, 27), (char, 28)}.
 *
 * The correct results shown in the stdout when running on one process should
 * be:
 *   newtype    lb= 0 ub=32 extent=32 size=20 true_lb= 0 true_extent=29
 *
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

/*----< main() >------------------------------------------------------------*/
int main(int argc, char **argv) {
    int          i, err, typesize;
    MPI_Aint     lb, ub, extent, true_lb, true_extent; 
    int          array_of_blocklengths[2];
    MPI_Aint     array_of_displacements[2];
    MPI_Datatype array_of_types[2];

    MPI_Init(&argc, &argv);

    int B[3] = {2, 1, 3};
    MPI_Aint D[3] = {0, 16, 26};
    MPI_Datatype T[3], type0, type1, newtype;

    /* create type1 with type map: {(double, 0), (char, 8)}, with extent 16 */
    array_of_blocklengths[0]  = 1;
    array_of_displacements[0] = 0;
    array_of_types[0]         = MPI_DOUBLE;

    array_of_blocklengths[1]  = 1;
    array_of_displacements[1] = 8;
    array_of_types[1]         = MPI_CHAR;

    err = MPI_Type_create_struct(2, array_of_blocklengths,
                                 array_of_displacements,
                                 array_of_types, &type0); ERR
    err = MPI_Type_commit(&type0); ERR

    /* resize to make type extent 16 */
    err = MPI_Type_create_resized(type0, 0, 16, &type1);
    err = MPI_Type_commit(&type1); ERR

    /* create type0 using struct datatype creator */
    T[0] = MPI_FLOAT; T[1] = type1; T[2] = MPI_CHAR;

    err = MPI_Type_create_struct(3, B, D, T, &newtype); ERR
    err = MPI_Type_commit(&newtype); ERR

    /* check LB, UB, size, extent */
    MPI_Type_ub(newtype, &ub);
    MPI_Type_size(newtype, &typesize);
    MPI_Type_get_extent(newtype, &lb, &extent);
    MPI_Type_get_true_extent(newtype, &true_lb, &true_extent);
    printf("newtype    lb=%2ld ub=%2ld extent=%2ld size=%2d true_lb=%2ld true_extent=%2ld\n",lb,ub,extent,typesize,true_lb, true_extent);

    err = MPI_Type_free(&type0); ERR
    err = MPI_Type_free(&type1); ERR
    err = MPI_Type_free(&newtype); ERR

    MPI_Finalize();
    return 0;
}
