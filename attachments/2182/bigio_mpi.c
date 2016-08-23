#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define ELEM_LEN 4
size_t bigcount = 536870916*2;

int main(int argc, char** argv)
{

    MPI_File fh;
    int i, mpi_rank, mpi_size;
    size_t k;
    MPI_Datatype inner_type, outer_type, mem_type;
    MPI_Datatype elem_type, file_type;
    MPI_Offset dims[2], block[2], stride[2], count[2], start[2];
    int *buf_write, *buf_read;
    int rc;

    MPI_Init (&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);

    /* create a large buffer 2*/
    buf_write = malloc (bigcount*ELEM_LEN);
    buf_read = malloc (bigcount*ELEM_LEN);
    memset(buf_read, 0, bigcount*ELEM_LEN);

    k = 0;
    for (i=0; i<bigcount ; i++) {
        buf_write[k] = k; k++;
    }

    MPI_Type_contiguous (ELEM_LEN, MPI_BYTE, &elem_type);

    dims[0] = bigcount;
    dims[1] = mpi_size;

    block[0] = dims[0];
    block[1] = dims[1]/mpi_size;
    stride[0] = 1;
    stride[1] = 1;
    count[0] = 1;
    count[1] = 1;
    start[0] = 0;
    start[1] = mpi_rank * block[1];


    /* create mem datatype */
    MPI_Type_contiguous (bigcount, elem_type, &mem_type);
    MPI_Type_commit(&mem_type);

    /* create file type */
    inner_type = elem_type;
    for(i=1 ; i>=0; --i) {
        MPI_Type_vector((int)count[i],
                        (int)block[i],
                        (int)stride[i],
                        inner_type,
                        &outer_type);

        if(i != 0) {
            int block_len = 1;
            MPI_Aint start_disp, new_extent;
            MPI_Datatype interm_type;

            start_disp = mpi_rank * ELEM_LEN;
            new_extent = mpi_size * ELEM_LEN;
            MPI_Type_create_hindexed(1, &block_len, &start_disp, outer_type, &interm_type);
            MPI_Type_free(&outer_type);
	    /* inner type created but not freed */
            MPI_Type_create_resized(interm_type, 0, new_extent, &inner_type);
            MPI_Type_free(&interm_type);
        }
        else {
            file_type = outer_type;
        }
    }

    MPI_Type_free(&inner_type);
    MPI_Type_commit(&file_type);

    MPI_Type_free(&elem_type);

    if(MPI_File_open (MPI_COMM_WORLD, argv[1], MPI_MODE_RDWR | MPI_MODE_CREATE,
                      MPI_INFO_NULL, &fh) != 0) {
        printf("Can't open file: %s\n",argv[1]);
        exit(1);
    }

    printf("SET_VIEW\n");
    if (MPI_SUCCESS != MPI_File_set_view(fh, 2144, MPI_BYTE, 
                                         file_type, "native", MPI_INFO_NULL)) {
        printf ("ERROR SET VIEW\n");
        exit(1);
    }
    printf("DONE SET_VIEW\n");

    printf("WRITING\n");
    /* write everything */
    rc =  MPI_File_write_at_all (fh, 
                                 0, 
                                 buf_write, 
                                 1, 
                                 mem_type, 
                                 MPI_STATUS_IGNORE);
    if (rc != MPI_SUCCESS){
        printf ("%d ERROR WRITE AT ALL\n", rc);
        exit(1);
    }
    printf("DONE WRITING %d\n", rc);

    if (MPI_SUCCESS != MPI_File_set_view(fh, 2144, MPI_BYTE, 
                                         file_type, "native", MPI_INFO_NULL)) {
        printf ("ERROR SET VIEW\n");
        exit(1);
    }

    printf("READING\n");
    /* write everything */
    rc = MPI_File_read_at_all (fh, 
                               0, 
                               buf_read, 
                               1, 
                               mem_type, 
                               MPI_STATUS_IGNORE);
    if (rc != MPI_SUCCESS) {
        printf ("%d ERROR READ AT ALL\n", rc);
        exit(1);
    }
    printf("DONE READING %d\n", rc);

    for (k=0 ; k<bigcount ; k++) {
        if(buf_read[k] != buf_write[k]) {
            fprintf(stderr, "Verfiy Failed at index %zu\n", k);
            assert(0);
        }
    }
    free(buf_write);
    free(buf_read);

    MPI_File_close(&fh);

    MPI_Type_free (&mem_type);
    MPI_Type_free(&file_type);

    MPI_Finalize ();
    
    return 0;
}
/* valgrind reports leaks here:
 * ==7971==    by 0x400EA8: main (bigio_mpi.c:38)
 * ==7971==    by 0x400F60: main (bigio_mpi.c:55)
 * ==7971==    by 0x400FCC: main (bigio_mpi.c:60)
 * ==7971==    by 0x40102B: main (bigio_mpi.c:73)
 * ==7971==    by 0x401060: main (bigio_mpi.c:75)
 * ==7971==    by 0x4010A7: main (bigio_mpi.c:84)
 */
