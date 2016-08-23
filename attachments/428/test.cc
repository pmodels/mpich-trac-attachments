
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

struct double3
{
	double x, y, z;
};

int main(int argc, char** argv)
{
	double3 *buf, *mem;
	MPI_Datatype d3;
	MPI_Win win;
	int myrank;
	
	MPI_Init(&argc,&argv);

	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

	MPI_Type_contiguous(3, MPI_DOUBLE, &d3);
	MPI_Type_commit(&d3);

	buf = (double3* )calloc(100,sizeof(double3));
	MPI_Alloc_mem(100*sizeof(double3),MPI_INFO_NULL,&mem);

	MPI_Win_create(mem,100*sizeof(double3),sizeof(double3),MPI_INFO_NULL,
		       MPI_COMM_WORLD, &win);

	MPI_Win_fence(0,win);
	if(1 == myrank) {
		MPI_Accumulate(buf,10,d3,0,0,10,d3,MPI_SUM,win);
		/*MPI_Accumulate(buf,3*10,MPI_DOUBLE,0,0,3*10,MPI_DOUBLE,MPI_SUM,win);*/
	}

	MPI_Win_fence(0,win);

	printf(" + fence on %d completed.\n", myrank);

	free(buf);
	MPI_Type_free(&d3);
	MPI_Free_mem(mem);
	MPI_Win_free(&win);
	return MPI_Finalize();
}

