#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int nproc = (argc > 1 ) ? atoi(argv[1]) : 1000000;

    int ret[3] = {0, 0, 0};
    MPI_Dims_create(nproc, 3, ret);
    printf("%d, %d, %d \n", ret[0], ret[1], ret[2]);

    MPI_Finalize();
    return 0;
}
